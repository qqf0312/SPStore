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
/** @file BlockVerifier.h
 *  @author mingzhenliu
 *  @date 20180921
 */
#pragma once

#include "BlockVerifierInterface.h"
#include "ExecutiveContext.h"
#include "ExecutiveContextFactory.h"
#include "libprecompiled/Precompiled.h"
#include <libdevcore/FixedHash.h>
#include <libdevcore/ThreadPool.h>
#include <libdevcrypto/Common.h>
#include <libethcore/Block.h>
#include <libethcore/Protocol.h>
#include <libethcore/Transaction.h>
#include <libethcore/TransactionReceipt.h>
#include <libevm/ExtVMFace.h>
#include <libexecutive/ExecutionResult.h>
#include <libexecutive/Executive.h>
#include <libmptstate/State.h>
#include <libdevcore/Guards.h>
#include <boost/function.hpp>
#include <algorithm>
#include <memory>
#include <thread>
#include "pointproofs.h"
#include <rocksdb/db.h>
#include <rocksdb/options.h>
#include <rocksdb/slice.h>

namespace dev
{
namespace eth
{
class TransactionReceipt;

}  // namespace eth

namespace executive
{
struct ExecutionResult;
}

namespace blockverifier
{
/*by zcy modified 2020/6/11*/
class ExecutiveCell
{
public:
    ExecutiveCell(dev::executive::Executive::Ptr _executiveinstance)
    {
        m_executiveinstance = _executiveinstance;
    }
    dev::executive::Executive::Ptr getExecutiveInstance()
    {
        return m_executiveinstance;
    }
private:
    dev::executive::Executive::Ptr m_executiveinstance;
};
/*modified end*/
class BlockVerifier : public BlockVerifierInterface,
                      public std::enable_shared_from_this<BlockVerifier>
{
public:
    typedef std::shared_ptr<BlockVerifier> Ptr;
    typedef boost::function<dev::h256(int64_t x)> NumberHashCallBackFunction;

    /*by zcy modified 2020/6/9 */
    BlockVerifier(bool _enableParallel = false,int _executiveinstance_num = 5) : m_enableParallel(_enableParallel),m_executiveinstance_num(_executiveinstance_num)
    {
        if (_enableParallel)
        {
            // m_threadNum = std::max(std::thread::hardware_concurrency(), (unsigned int)1);
            m_threadNum = 8;
        }
        // rocksdb::Options options;
        // options.create_if_missing = true;
        // rocksdb::Status status = rocksdb::DB::Open(options, "./beforeecstorage/", &before_db);
        // assert(status.ok());
        /*by zcy modified 2020/6/11*/
        //dev::executive::Executive::Ptr executive = std::make_shared<dev::executive::Executive>();
        // for(int i = 0;i< m_executiveinstance_num;i++)
        // {
        //     dev::executive::Executive::Ptr executive =std::make_shared<dev::executive::Executive>();
        //     // ExecutiveCell executiveinstancecell(executive);
        //     m_executiveinstance_pool.push(executive);
        // }

        /*modified end*/
    }
    /*modified end*/
    virtual ~BlockVerifier() {}

//    dev::executive::Executive::Ptr getExecutiveInstance();

    ExecutiveContext::Ptr executeBlock(dev::eth::Block& block, BlockInfo const& parentBlockInfo);
    ExecutiveContext::Ptr serialExecuteBlock(
        dev::eth::Block& block, BlockInfo const& parentBlockInfo);
    ExecutiveContext::Ptr parallelExecuteBlock(
        dev::eth::Block& block, BlockInfo const& parentBlockInfo);
    void initTestStates(unsigned int block_number);
    // void setDB(rocksdb::DB* _db){before_db = _db;}
    dev::eth::TransactionReceipt::Ptr executeTransaction(
        const dev::eth::BlockHeader& blockHeader, dev::eth::Transaction::Ptr _t);
#if 0
    std::pair<dev::executive::ExecutionResult, dev::eth::TransactionReceipt::Ptr> execute(
        dev::eth::EnvInfo const& _envInfo, dev::eth::Transaction const& _t,
        dev::eth::OnOpFunc const& _onOp,
        dev::blockverifier::ExecutiveContext::Ptr executiveContext);
#endif


    dev::eth::TransactionReceipt::Ptr execute(dev::eth::Transaction::Ptr _t,
        dev::eth::OnOpFunc const& _onOp, dev::blockverifier::ExecutiveContext::Ptr executiveContext,
        dev::executive::Executive::Ptr executive);

    void setExecutiveContextFactory(ExecutiveContextFactory::Ptr executiveContextFactory)
    {
        m_executiveContextFactory = executiveContextFactory;
    }
    ExecutiveContextFactory::Ptr getExecutiveContextFactory() { return m_executiveContextFactory; }
    void setNumberHash(const NumberHashCallBackFunction& _pNumberHash)
    {
        m_pNumberHash = _pNumberHash;
    }

private:
    ExecutiveContextFactory::Ptr m_executiveContextFactory;
    NumberHashCallBackFunction m_pNumberHash;
    bool m_enableParallel;
    unsigned int m_threadNum = -1;

    /*by zcy modified 2020/6/11 */
    //dev::executive::Executive::Ptr executive;
    // std::queue<dev::executive::Executive::Ptr> m_executiveinstance_pool;
    int m_executiveinstance_num =0;
    std::mutex x_executiveinstance_num;
    /*modified end*/
    BlockInfo m_parent_block_info;
    std::mutex m_executingMutex;
    std::atomic<int64_t> m_executingNumber = {0};
    pointproofs_params pointproofs_param;
    bool vc_flag = false;
    pointproofs_params sub_pointproofs_param;
    bool sub_vc_flag = false;
    // char seed[] = "this is a very long seed for pointproofs tests";
    uint8_t ciphersuite = 0;
    rocksdb::DB* vc_db;
     rocksdb::DB* before_db;
};

}  // namespace blockverifier

}  // namespace dev
