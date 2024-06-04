/*
 * @CopyRight:
 * FISCO-BCOS is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * FISCO-BCOS is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with FISCO-BCOS.  If not, see <http://www.gnu.org/licenses/>
 * (c) 2016-2018 fisco-dev contributors.
 */
/** @file BlockVerifier.cpp
 *  @author mingzhenliu
 *  @date 20180921
 */
#include "BlockVerifier.h"
#include "ExecutiveContext.h"
#include "TxDAG.h"
#include "libstorage/StorageException.h"
#include <libethcore/Exceptions.h>
#include <libethcore/PrecompiledContract.h>
#include <libethcore/TransactionReceipt.h>
#include <libexecutive/ExecutionResult.h>
#include <libstorage/Table.h>
#include <tbb/parallel_for.h>
#include <exception>
#include <thread>
#include <libbloomfilter/bf/all.hpp>
#include "rocksdb/db.h"
#include "rocksdb/options.h"
#include "rocksdb/slice.h"

// #include "lib.h"

using namespace dev;
using namespace std;
using namespace dev::eth;
using namespace dev::blockverifier;
using namespace dev::executive;
using namespace dev::storage;
using namespace bf;
using namespace rocksdb;

/*by zcy modified 2020/6/11*/
// dev::executive::Executive::Ptr BlockVerifier::getExecutiveInstance()
// {
//     x_executiveinstance_num.lock();
//     // std::cout << "queue size = " <<m_executiveinstance_pool.size()<< std::endl;
//     while (m_executiveinstance_pool.size() == 0)
//     {
//         std::cout << "wait executive" << std::endl;
//         sleep(1000);
//     }
//     dev::executive::Executive::Ptr exeptr = m_executiveinstance_pool.back();
//     m_executiveinstance_pool.pop();
//     x_executiveinstance_num.unlock();
//     return exeptr;
// }
/*modified end*/
ExecutiveContext::Ptr BlockVerifier::executeBlock(Block& block, BlockInfo const& parentBlockInfo)
{
    // return nullptr prepare to exit when g_BCOSConfig.shouldExit is true
    if (g_BCOSConfig.shouldExit)
    {
        return nullptr;
    }
    if (block.blockHeader().number() < m_executingNumber)
    {
        return nullptr;
    }
    std::lock_guard<std::mutex> l(m_executingMutex);
    if (block.blockHeader().number() < m_executingNumber)
    {
        return nullptr;
    }
    ExecutiveContext::Ptr context = nullptr;
    // context = serialExecuteBlock(block, parentBlockInfo);
    try
    {
        // modified 2020/8
        m_enableParallel = false;
        if (g_BCOSConfig.version() >= RC2_VERSION && m_enableParallel)
        {
            context = parallelExecuteBlock(block, parentBlockInfo);
        }
        else
        {
            context = serialExecuteBlock(block, parentBlockInfo);
        }
    }
    catch (exception& e)
    {
        BLOCKVERIFIER_LOG(ERROR) << LOG_BADGE("executeBlock") << LOG_DESC("executeBlock exception")
                                 << LOG_KV("blockNumber", block.blockHeader().number());
        return nullptr;
    }
    m_executingNumber = block.blockHeader().number();
    return context;
}
static inline double GetTime()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec + tv.tv_usec / 1e6;
}
ExecutiveContext::Ptr BlockVerifier::serialExecuteBlock(
    Block& block, BlockInfo const& parentBlockInfo)
{
    /* BLOCKVERIFIER_LOG(INFO) << LOG_DESC("executeBlock]Executing block")
                             << LOG_KV("txNum", block.transactions()->size())
                             << LOG_KV("num", block.blockHeader().number())
                             << LOG_KV("hash", block.header().hash().abridged())
                             << LOG_KV("height", block.header().number())
                             << LOG_KV("receiptRoot", block.header().receiptsRoot())
                             << LOG_KV("stateRoot", block.header().stateRoot())
                             << LOG_KV("dbHash", block.header().dbHash())
                             << LOG_KV("parentHash", parentBlockInfo.hash.abridged())
                             << LOG_KV("parentNum", parentBlockInfo.number)
                             << LOG_KV("parentStateRoot", parentBlockInfo.stateRoot);
 */
    uint64_t startTime = utcTime();

    ExecutiveContext::Ptr executiveContext = std::make_shared<ExecutiveContext>();
    try
    {
        m_executiveContextFactory->initExecutiveContext(
            parentBlockInfo, parentBlockInfo.stateRoot, executiveContext);
    }
    catch (exception& e)
    {
        BLOCKVERIFIER_LOG(ERROR) << LOG_DESC("[executeBlock] Error during initExecutiveContext")
                                 << LOG_KV("blkNum", block.blockHeader().number())
                                 << LOG_KV("EINFO", boost::diagnostic_information(e));

        BOOST_THROW_EXCEPTION(InvalidBlockWithBadStateOrReceipt()
                              << errinfo_comment("Error during initExecutiveContext"));
    }

    BlockHeader tmpHeader = block.blockHeader();
    block.clearAllReceipts();
    block.resizeTransactionReceipt(block.transactions()->size());


    BLOCKVERIFIER_LOG(DEBUG) << LOG_BADGE("executeBlock") << LOG_DESC("Init env takes")
                             << LOG_KV("time(ms)", utcTime() - startTime)
                             << LOG_KV("txNum", block.transactions()->size())
                             << LOG_KV("num", block.blockHeader().number());
    uint64_t pastTime = utcTime();
    Executive::Ptr executive = std::make_shared<Executive>();
    std::hash<unsigned int> block_hash;
    unsigned int seed = block_hash(block.blockHeader().number());
    std::mt19937 generator(seed);
    unsigned int values1 = generator();
    unsigned int values2 = generator();

    h256 stateRoot =
        sha256(boost::lexical_cast<string>(values1).append(boost::lexical_cast<string>(values2)));
    // Executive::Ptr executive;
    // initTestStates(block.blockHeader().number());
    try
    {
        /*by zcy modified 2020/6/11 */
        // executive = getExecutiveInstance();
        /*modified end*/
        EnvInfo envInfo(block.blockHeader(), m_pNumberHash, 0);
        envInfo.setPrecompiledEngine(executiveContext);
        executive->setEnvInfo(envInfo);
        executive->setState(executiveContext->getState());
        for (size_t i = 0; i < block.transactions()->size(); i++)
        {
            // modified 2020/7/23
            auto& tx = (*block.transactions())[i];

            TransactionReceipt::Ptr resultReceipt =
            execute(tx, OnOpFunc(), executiveContext, executive);
            // dev::eth::TransactionReceipt::Ptr resultReceipt =
                // std::make_shared<TransactionReceipt>(stateRoot, executive->gasUsed(),
                    // executive->logs(), executive->status(), tx->rlp(), executive->newAddress());
            block.setTransactionReceipt(i, resultReceipt);
            executiveContext->getState()->commit();
        }
    }
    catch (exception& e)
    {
        BLOCKVERIFIER_LOG(ERROR) << LOG_BADGE("executeBlock")
                                 << LOG_DESC("Error during serial block execution")
                                 << LOG_KV("blkNum", block.blockHeader().number())
                                 << LOG_KV("EINFO", boost::diagnostic_information(e));

        BOOST_THROW_EXCEPTION(
            BlockExecutionFailed() << errinfo_comment("Error during serial block execution"));
    }


    BLOCKVERIFIER_LOG(DEBUG) << LOG_BADGE("executeBlock") << LOG_DESC("Run serial tx takes")
                             << LOG_KV("time(ms)", utcTime() - pastTime)
                             << LOG_KV("txNum", block.transactions()->size())
                             << LOG_KV("num", block.blockHeader().number());

    // modified 2020/7/23
    // h256 stateRoot = executiveContext->getState()->rootHash();

    // set stateRoot in receipts
    // for (size_t i = 0; i < block.transactions()->size(); i++)
    // {
    //     // modified 2020/7/23
    //     auto& tx = (*block.transactions())[i];


    //     // dev::eth::TransactionReceipt::Ptr resultReceipt =
    //     // std::make_shared<TransactionReceipt>();
    //     block.setTransactionReceipt(i, resultReceipt);
    //     // executiveContext->getState()->commit();
    // }
    if (g_BCOSConfig.version() >= V2_2_0)
    {
        // when support_version is lower than v2.2.0, doesn't setStateRootToAllReceipt
        // enable_parallel=true can't be run with enable_parallel=false
        block.setStateRootToAllReceipt(stateRoot);
    }
    unsigned int values3 = generator();
    unsigned int values4 = generator();
    block.updateSequenceReceiptGas();
    block.calReceiptRoot();
    block.header().setReceiptsRoot(
        sha256(boost::lexical_cast<string>(values3).append(boost::lexical_cast<string>(values4))));

    block.header().setStateRoot(stateRoot);
    unsigned int values5 = generator();
    unsigned int values6 = generator();
    // block.header().setDBhash(executiveContext->getMemoryTableFactory()->hash());
    block.header().setDBhash(
        sha256(boost::lexical_cast<string>(values5).append(boost::lexical_cast<string>(values6))));
    //  block.header().setStateRoot(FixedHash<32>());
    //  block.header().setDBhash(FixedHash<32>());
    //  auto tmpHeader = block.header();
    // m_executiveinstance_pool.push(executive);
    /// if executeBlock is called by consensus module, no need to compare receiptRoot and stateRoot
    /// since origin value is empty if executeBlock is called by sync module, need to compare
    /// receiptRoot, stateRoot and dbHash
    if (tmpHeader.receiptsRoot() != h256() && tmpHeader.stateRoot() != h256())
    {
        if (tmpHeader != block.blockHeader())
        {
            BLOCKVERIFIER_LOG(ERROR)
                << "Invalid Block with bad stateRoot or receiptRoot or dbHash"
                << LOG_KV("blkNum", block.blockHeader().number())
                << LOG_KV("originHash", tmpHeader.hash().abridged())
                << LOG_KV("curHash", block.header().hash().abridged())
                << LOG_KV("orgReceipt", tmpHeader.receiptsRoot().abridged())
                << LOG_KV("curRecepit", block.header().receiptsRoot().abridged())
                << LOG_KV("orgTxRoot", tmpHeader.transactionsRoot().abridged())
                << LOG_KV("curTxRoot", block.header().transactionsRoot().abridged())
                << LOG_KV("orgState", tmpHeader.stateRoot().abridged())
                << LOG_KV("curState", block.header().stateRoot().abridged())
                << LOG_KV("orgDBHash", tmpHeader.dbHash().abridged())
                << LOG_KV("curDBHash", block.header().dbHash().abridged());
            BOOST_THROW_EXCEPTION(
                InvalidBlockWithBadStateOrReceipt() << errinfo_comment(
                    "Invalid Block with bad stateRoot or ReceiptRoot, orgBlockHash " +
                    block.header().hash().abridged()));
        }
    }
    BLOCKVERIFIER_LOG(INFO) << LOG_BADGE("executeBlock") << LOG_DESC("Execute block takes")
                            << LOG_KV("time(ms)", utcTime() - startTime)
                            << LOG_KV("txNum", block.transactions()->size())
                            << LOG_KV("num", block.blockHeader().number())
                            << LOG_KV("blockHash", block.headerHash())
                            << LOG_KV("stateRoot", block.header().stateRoot())
                            << LOG_KV("dbHash", block.header().dbHash())
                            << LOG_KV("transactionRoot", block.transactionRoot())
                            << LOG_KV("receiptRoot", block.receiptRoot());
    return executiveContext;
}
void BlockVerifier::initTestStates(unsigned int block_number)
{
    double starttime = GetTime();
    // int block_number = 0;

    // for (int block_number = 0; block_number < 10; block_number++) {
    std::hash<unsigned int> block_hash;
    unsigned int seed = block_hash(block_number);
    std::mt19937 generator(seed);
    uniform_int_distribution<long long> dist(1000, 2000);
    size_t state_size = dist(generator);
    // size_t state_size = 1000;
    std::cout << "block " << block_number << " state num = " << state_size << std::endl;
    basic_bloom_filter bf(0.01, 3000);
    // pointproofs_value* values = new pointproofs_value[INIT_VC_SIZE];
    // memcpy(values, template_values, sizeof(template_values) * INIT_VC_SIZE);
    // bf_db->Put(WriteOptions(), mainDecIntToHexStr(block_number)+"_num",
    // mainDecIntToHexStr(state_size));
    std::map<int, std::map<int, std::string>> position_mapinto_accounts;
    seed = block_hash(block_number);
    std::mt19937 rand_num(seed);
    rocksdb::WriteBatch batch;
    for (size_t i = 0; i < state_size; i++)
    {
        // int key = i;
        std::string key_str = sha256(boost::lexical_cast<string>(rand_num())).hex();

        // std::cout << "initTestStates key = " << key_str << std::endl;
        // std::string k = key_str;
        int value = i;
        // batch.Put(key_str, to_string(value));
        // before_db->Write(WriteOptions(), &batch);

        // before_db->Put(WriteOptions(), key_str, to_string(value));
        // original_size = original_size + key_str.length() +
        // boost::lexical_cast<string>(value).length(); free(tmp); std::cout << "key_str = " <<
        // k.substr(0, 32) << std::endl;
        bf.add(key_str.substr(32));
        // bf.add(i);
    }
}
ExecutiveContext::Ptr BlockVerifier::parallelExecuteBlock(
    Block& block, BlockInfo const& parentBlockInfo)

{
    BLOCKVERIFIER_LOG(INFO) << LOG_DESC("[executeBlock]Executing block")
                            << LOG_KV("txNum", block.transactions()->size())
                            << LOG_KV("num", block.blockHeader().number())
                            << LOG_KV("parentHash", parentBlockInfo.hash)
                            << LOG_KV("parentNum", parentBlockInfo.number)
                            << LOG_KV("parentStateRoot", parentBlockInfo.stateRoot);

    auto start_time = utcTime();
    auto record_time = utcTime();
    ExecutiveContext::Ptr executiveContext = std::make_shared<ExecutiveContext>();
    try
    {
        m_executiveContextFactory->initExecutiveContext(
            parentBlockInfo, parentBlockInfo.stateRoot, executiveContext);
    }
    catch (exception& e)
    {
        BLOCKVERIFIER_LOG(ERROR) << LOG_DESC("[executeBlock] Error during initExecutiveContext")
                                 << LOG_KV("EINFO", boost::diagnostic_information(e));

        BOOST_THROW_EXCEPTION(InvalidBlockWithBadStateOrReceipt()
                              << errinfo_comment("Error during initExecutiveContext"));
    }

    auto memoryTableFactory = executiveContext->getMemoryTableFactory();

    auto initExeCtx_time_cost = utcTime() - record_time;
    record_time = utcTime();

    BlockHeader tmpHeader = block.blockHeader();
    block.clearAllReceipts();
    block.resizeTransactionReceipt(block.transactions()->size());
    auto perpareBlock_time_cost = utcTime() - record_time;
    record_time = utcTime();

    shared_ptr<TxDAG> txDag = make_shared<TxDAG>();
    txDag->init(executiveContext, block.transactions(), block.blockHeader().number());


    txDag->setTxExecuteFunc([&](Transaction::Ptr _tr, ID _txId, Executive::Ptr _executive) {
    // string fout = "./nonce.txt";
    // std::ofstream out_key(fout, std::ofstream::app);
    // out_key << _tr->nonce() << std::endl;
    // out_key.close();
#if 0
        std::pair<ExecutionResult, TransactionReceipt::Ptr> resultReceipt =
            execute(envInfo, _tr, OnOpFunc(), executiveContext);
#endif
        double starttime = GetTime();
        auto resultReceipt = execute(_tr, OnOpFunc(), executiveContext, _executive);
        double endtime = GetTime();
        // std::cout << "transaction execute time = " << endtime - starttime << std::endl;
        block.setTransactionReceipt(_txId, resultReceipt);
        executiveContext->getState()->commit();
        return true;
    });
    auto initDag_time_cost = utcTime() - record_time;
    record_time = utcTime();

    auto parallelTimeOut = utcSteadyTime() + 30000;  // 30 timeout

    try
    {
        tbb::atomic<bool> isWarnedTimeout(false);
        tbb::parallel_for(tbb::blocked_range<unsigned int>(0, m_threadNum),
            [&](const tbb::blocked_range<unsigned int>& _r) {
                (void)_r;
                EnvInfo envInfo(block.blockHeader(), m_pNumberHash, 0);
                envInfo.setPrecompiledEngine(executiveContext);
                /*by zcy modified 2020/6/11*/
                Executive::Ptr executive = std::make_shared<Executive>();
                // Executive::Ptr executive = getExecutiveInstance();
                /*modified end*/
                executive->setEnvInfo(envInfo);
                executive->setState(executiveContext->getState());

                while (!txDag->hasFinished())
                {
                    if (!isWarnedTimeout.load() && utcSteadyTime() >= parallelTimeOut)
                    {
                        isWarnedTimeout.store(true);
                        BLOCKVERIFIER_LOG(INFO)
                            << LOG_BADGE("executeBlock") << LOG_DESC("Para execute block timeout")
                            << LOG_KV("txNum", block.transactions()->size())
                            << LOG_KV("blockNumber", block.blockHeader().number());
                    }

                    txDag->executeUnit(executive);
                }
                /*by zcy modified 2020/6/12*/
                // m_executiveinstance_pool.push(executive);
                /*modified end*/
            });
    }
    catch (exception& e)
    {
        BLOCKVERIFIER_LOG(ERROR) << LOG_BADGE("executeBlock")
                                 << LOG_DESC("Error during parallel block execution")
                                 << LOG_KV("EINFO", boost::diagnostic_information(e));

        BOOST_THROW_EXCEPTION(
            BlockExecutionFailed() << errinfo_comment("Error during parallel block execution"));
    }
    // if the program is going to exit, return nullptr directly
    if (g_BCOSConfig.shouldExit)
    {
        return nullptr;
    }
    auto exe_time_cost = utcTime() - record_time;
    record_time = utcTime();

    h256 stateRoot = executiveContext->getState()->rootHash();
    auto getRootHash_time_cost = utcTime() - record_time;
    record_time = utcTime();

    // set stateRoot in receipts
    block.setStateRootToAllReceipt(stateRoot);
    block.updateSequenceReceiptGas();
    auto setAllReceipt_time_cost = utcTime() - record_time;
    record_time = utcTime();

    block.calReceiptRoot();
    auto getReceiptRoot_time_cost = utcTime() - record_time;
    record_time = utcTime();

    block.header().setStateRoot(stateRoot);
    block.header().setDBhash(executiveContext->getMemoryTableFactory()->hash());
    auto setStateRoot_time_cost = utcTime() - record_time;
    record_time = utcTime();

    if (tmpHeader.receiptsRoot() != h256() && tmpHeader.stateRoot() != h256())
    {
        if (tmpHeader != block.blockHeader())
        {
            BLOCKVERIFIER_LOG(ERROR)
                << "Invalid Block with bad stateRoot or receiptRoot or dbHash"
                << LOG_KV("blkNum", block.blockHeader().number())
                << LOG_KV("originHash", tmpHeader.hash().abridged())
                << LOG_KV("curHash", block.header().hash().abridged())
                << LOG_KV("orgReceipt", tmpHeader.receiptsRoot().abridged())
                << LOG_KV("curRecepit", block.header().receiptsRoot().abridged())
                << LOG_KV("orgTxRoot", tmpHeader.transactionsRoot().abridged())
                << LOG_KV("curTxRoot", block.header().transactionsRoot().abridged())
                << LOG_KV("orgState", tmpHeader.stateRoot().abridged())
                << LOG_KV("curState", block.header().stateRoot().abridged())
                << LOG_KV("orgDBHash", tmpHeader.dbHash().abridged())
                << LOG_KV("curDBHash", block.header().dbHash().abridged());
            BOOST_THROW_EXCEPTION(InvalidBlockWithBadStateOrReceipt() << errinfo_comment(
                                      "Invalid Block with bad stateRoot or ReciptRoot"));
        }
    }
    BLOCKVERIFIER_LOG(INFO) << LOG_BADGE("executeBlock") << LOG_DESC("Para execute block takes")
                            << LOG_KV("time(ms)", utcTime() - start_time)
                            << LOG_KV("txNum", block.transactions()->size())
                            << LOG_KV("blockNumber", block.blockHeader().number())
                            << LOG_KV("blockHash", block.headerHash())
                            << LOG_KV("stateRoot", block.header().stateRoot())
                            << LOG_KV("dbHash", block.header().dbHash())
                            << LOG_KV("transactionRoot", block.transactionRoot())
                            << LOG_KV("receiptRoot", block.receiptRoot())
                            << LOG_KV("initExeCtxTimeCost", initExeCtx_time_cost)
                            << LOG_KV("perpareBlockTimeCost", perpareBlock_time_cost)
                            << LOG_KV("initDagTimeCost", initDag_time_cost)
                            << LOG_KV("exeTimeCost", exe_time_cost)
                            << LOG_KV("getRootHashTimeCost", getRootHash_time_cost)
                            << LOG_KV("setAllReceiptTimeCost", setAllReceipt_time_cost)
                            << LOG_KV("getReceiptRootTimeCost", getReceiptRoot_time_cost)
                            << LOG_KV("setStateRootTimeCost", setStateRoot_time_cost);
    return executiveContext;
}
TransactionReceipt::Ptr BlockVerifier::executeTransaction(
    const BlockHeader& blockHeader, dev::eth::Transaction::Ptr _t)
{
    ExecutiveContext::Ptr executiveContext = std::make_shared<ExecutiveContext>();
    BlockInfo blockInfo{blockHeader.hash(), blockHeader.number(), blockHeader.stateRoot()};
    try
    {
        m_executiveContextFactory->initExecutiveContext(
            blockInfo, blockHeader.stateRoot(), executiveContext);
    }
    catch (exception& e)
    {
        BLOCKVERIFIER_LOG(ERROR)
            << LOG_DESC("[executeTransaction] Error during execute initExecutiveContext")
            << LOG_KV("errorMsg", boost::diagnostic_information(e));
    }
    /*by zcy modified 2020/6/9*/
    Executive::Ptr executive = std::make_shared<Executive>();
    // Executive::Ptr executive = getExecutiveInstance();
    /*modified end*/
    EnvInfo envInfo(blockHeader, m_pNumberHash, 0);
    envInfo.setPrecompiledEngine(executiveContext);
    executive->setEnvInfo(envInfo);
    executive->setState(executiveContext->getState());
    // only Rpc::call will use executeTransaction, RPC do catch exception
    // modified 2020/7/23
    TransactionReceipt::Ptr tr = execute(_t, OnOpFunc(), executiveContext, executive);
    // dev::eth::TransactionReceipt::Ptr tr = std::make_shared<TransactionReceipt>();
    return tr;
}


#if 0
std::pair<ExecutionResult, TransactionReceipt::Ptr> BlockVerifier::execute(EnvInfo const& _envInfo,
    Transaction const& _t, OnOpFunc const& _onOp, ExecutiveContext::Ptr executiveContext)
{
    auto onOp = _onOp;
#if ETH_VMTRACE
    if (isChannelVisible<VMTraceChannel>())
        onOp = Executive::simpleTrace();  // override tracer
#endif

    // Create and initialize the executive. This will throw fai rly cheaply and quickly if the
    // transaction is bad in any way.
    Executive e(executiveContext->getState(), _envInfo);
    ExecutionResult res;
    e.setResultRecipient(res);

    // OK - transaction looks valid - execute.
    try
    {
        e.initialize(_t);
        if (!e.execute())
            e.go(onOp);
        e.finalize();
    }
    catch (StorageException const& e)
    {
        BLOCKVERIFIER_LOG(ERROR) << LOG_DESC("get StorageException") << LOG_KV("what", e.what());
        BOOST_THROW_EXCEPTION(e);
    }
    catch (Exception const& _e)
    {
        // only OutOfGasBase ExecutorNotFound exception will throw
        BLOCKVERIFIER_LOG(ERROR) << diagnostic_information(_e);
    }
    catch (std::exception const& _e)
    {
        BLOCKVERIFIER_LOG(ERROR) << _e.what();
    }

    e.loggingException();

    return make_pair(
        res, std::make_shared<TransactionReceipt>(executiveContext->getState()->rootHash(false),
                 e.gasUsed(), e.logs(), e.status(), e.takeOutput().takeBytes(), e.newAddress()));
}
#endif


dev::eth::TransactionReceipt::Ptr BlockVerifier::execute(dev::eth::Transaction::Ptr _t,
    dev::eth::OnOpFunc const& _onOp, dev::blockverifier::ExecutiveContext::Ptr executiveContext,
    Executive::Ptr executive)
{
    auto onOp = _onOp;
#if ETH_VMTRACE
    if (isChannelVisible<VMTraceChannel>())
        onOp = Executive::simpleTrace();  // override tracer
#endif
    // Create and initialize the executive. This will throw fairly cheaply and quickly if the
    // transaction is bad in any way.
    executive->reset();


    // OK - transaction looks valid - execute.
    try
    {
        executive->initialize(_t);

        bool flag = executive->execute();
        
        if (!flag)
        {
            // double starttime = GetTime();
            executive->go(onOp);
        }
        executive->finalize();
    }
    catch (StorageException const& e)
    {
        BLOCKVERIFIER_LOG(ERROR) << LOG_DESC("get StorageException") << LOG_KV("what", e.what());
        BOOST_THROW_EXCEPTION(e);
    }
    catch (Exception const& _e)
    {
        // only OutOfGasBase ExecutorNotFound exception will throw
        BLOCKVERIFIER_LOG(ERROR) << diagnostic_information(_e);
    }
    catch (std::exception const& _e)
    {
        BLOCKVERIFIER_LOG(ERROR) << _e.what();
    }

    executive->loggingException();
    dev::eth::TransactionReceipt::Ptr tr = std::make_shared<TransactionReceipt>(
        executiveContext->getState()->rootHash(false), executive->gasUsed(), executive->logs(),
        executive->status(), executive->takeOutput().takeBytes(), executive->newAddress());
    // std::cout << "TransactionReceipt()time = " << GetTime() - starttime3 << std::endl;

    return tr;
}
