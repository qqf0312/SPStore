

#include <json/json.h>
#include <libblockchain/BlockChainImp.h>
#include <libblockverifier/Common.h>
#include <libdevcore/CommonData.h>
#include <libdevcore/CommonJS.h>
#include <libdevcore/FixedHash.h>
#include <libdevcore/TopicInfo.h>
#include <libdevcrypto/Common.h>
#include <liberasure-code/common/buffer.h>
#include <libethcore/ABI.h>
#include <libethcore/Block.h>
#include <libethcore/Protocol.h>
#include <libinitializer/Initializer.h>
#include <libinitializer/P2PInitializer.h>
#include <libinitializer/SecureInitializer.h>
#include <libmptstate/MPTState.h>
#include <librpc/Rpc.h>
#include <stdlib.h>
#include <sys/time.h>
#include <tbb/concurrent_hash_map.h>

#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/serialization/vector.hpp>
#include <cassert>
#include <ctime>
#include <fstream>
#include <iostream>
#include <iterator>
#include <libbloomfilter/bf/all.hpp>
#include <memory>
#include <string>
#include <libmptstate/VCGroup.h>

// #include "Eurasure-P2P.h"
// #include "Eurasure.h"
#include "Client.h"
// #ifdef __cplusplus
// extern "C" {
// #endif
// #include "pointproofs.h"
// #ifdef __cplusplus
// }
// #endif
#include "rocksdb/db.h"
#include "rocksdb/options.h"
#include "rocksdb/slice.h"

using namespace std;
using namespace dev;
using namespace dev::eth;
using namespace dev::rpc;
using namespace dev::ledger;
using namespace dev::initializer;
using namespace dev::txpool;
using namespace dev::blockchain;
using namespace rocksdb;
using namespace bf;

#define EXPECT_EQ(a, b) is_equal(a, b)
#define EXPECT_TRUE(a) is_true(a)
using namespace ceph;

#define INIT_VC_SIZE 5000
#define INIT_MAIN_VC_SIZE 64
#define PARTITION_KEY_MAPPINTO_VC_BYTES_NUM 1
#define BYTES_TO_BITS 4
#define SUB_COMMIT_NUM 16
#define EC_PER_STATE_SIZE 49
long vc_size = 0;
long ec_size = 0;
long bf_size = 0;
long original_size = 0;
long hash_function_size = 0;

static ec::EurasureP2P* p2pHandler = NULL;
string mainDecIntToHexStr(unsigned int num)
{
    string str;
    unsigned int Temp = num / 16;
    int left = num % 16;
    if (Temp > 0)
        str += mainDecIntToHexStr(Temp);
    if (left < 10)
        str += (left + '0');
    else
        str += ('A' + left - 10);
    return str;
}
void is_equal(int i, int j)
{
    if (i == j)
    {
        std::cout << "success" << std::endl;
    }
    else
    {
        std::cout << "false" << std::endl;
    }
}
void is_true(bool flag)
{
    if (flag)
    {
        std::cout << "success" << std::endl;
    }
    else
    {
        std::cout << "false" << std::endl;
    }
}
// using namespace rocksdb;
// using namespace ec;

#define THREAD_NUM 1
#define EC_SIZE 10
#define DATACOUNT 3
// pointproofs_params pointproofs_param;
// pointproofs_params sub_pointproofs_param;

NodeID mynodeId;
static inline double GetTime()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec + tv.tv_usec / 1e6;
}
// int computePostion(const std::string& posStr, int mod)
// {
//     long res = 0;
//     for (int i = 0; i < 4; ++i)
//     {
//         // res += data[i] * 2;
//         if (posStr[i] >= 'A' && posStr[i] <= 'Z')
//         {
//             res = (res * 16 + (posStr[i] - 'A') + 10) % mod;
//         }
//         if (posStr[i] >= 'a' && posStr[i] <= 'z')
//         {
//             res = (res * 16 + (posStr[i] - 'a') + 10) % mod;
//         }
//         if (posStr[i] >= '0' && posStr[i] <= '9')
//         {
//             res = (res * 16 + (posStr[i] - '0')) % mod;
//         }
//     }
//     return res % mod;
// }

int maxChunkLen(std::map<int, std::map<int, std::string>> const& states)
{
    int max_len = 0;
    for (auto it = states.begin(); it != states.end(); ++it)
    {
        int len = 0;
        for (auto sub_it = it->second.begin(); sub_it != it->second.end(); ++sub_it)
        {
            len += 1;
        }
        // std::cout << "len = " << len << std::endl;
        if (len > max_len)
            max_len = len;
    }
    return max_len;
}

void thread_tps_task(std::shared_ptr<BlockChainInterface> chainManager)
{
    int step = 10000;
    int64_t txs_num = 0;
    int64_t current_txs_num = 0;
    double starttime1 = GetTime();
    double endtime1 = 0;
    int count = 0;
    std::cout<<"tps task start"<<endl;
    ofstream outfile("./tps.txt", ios::app);
    // qqf num<1000000
    while (current_txs_num < 1000000)
    {
        current_txs_num = chainManager->totalTransactionCount().first;
        endtime1 = GetTime();
        // std::cout<<"ctn="<<current_txs_num <<" n="<<chainManager->number()<<endl;
        if (current_txs_num - txs_num >= step)
        {
            outfile << "  txs = " << current_txs_num - txs_num
                    << "  time = " << (endtime1 - starttime1)
                    << "  tps = " << (current_txs_num - txs_num) / (endtime1 - starttime1)
                    << std::endl;

            std::cout << "  txs = " << current_txs_num - txs_num
                    << "  time = " << (endtime1 - starttime1)
                    << "  tps = " << (current_txs_num - txs_num) / (endtime1 - starttime1)
                    << std::endl;
            starttime1 = GetTime();
            txs_num = current_txs_num;
        }
    }
    outfile.close();
}
void thread_ec_task(std::shared_ptr<BlockChainInterface> chainManager)
{
    auto block_num = 0;
    std::cout<<" start ec processing "<<std::endl;
    while(1){
        if(block_num < chainManager->number()){
            block_num = chainManager->number();
            dev::mptstate::MPTState::state_erasure->makeEC(block_num, 1);
            std::cout<<" make ec end "<<std::endl;
        }
    }
}

void thread_printBlockHeader_task(std::shared_ptr<BlockChainInterface> chainManager)
{
    ofstream outfile("./BlockHeader.txt", ios::app);
    int64_t block_num = 1;
    while (block_num < 201)
    {
        while (block_num > chainManager->number())
        {
            sleep(1);
        }
        auto block = chainManager->getBlockByNumber(block_num);
        auto header = block->blockHeader();
        outfile << "parentHash = " << header.parentHash() << std::endl;
        outfile << "stateRoot = " << header.stateRoot() << std::endl;
        outfile << "transactionsRoot = " << header.transactionsRoot() << std::endl;
        outfile << "receiptsRoot = " << header.receiptsRoot() << std::endl;
        outfile << "dbHash = " << header.dbHash() << std::endl;
        outfile << "number = " << header.number() << std::endl;
        outfile << "timestamp = " << header.timestamp() << std::endl;
        h512s sealers = header.sealerList();
        for (int i = 0; i < sealers.size(); i++)
        {
            outfile << "sealer " << i << "  " << sealers[i] << std::endl;
        }
        outfile << std::endl;
        block_num++;
    }
    outfile.close();
}
// class ECP2PService
// {
// public:
//     ECP2PService(std::string const& _path)
//     {
//         boost::property_tree::ptree pt;
//         boost::property_tree::read_ini(_path, pt);
//         m_secureInitializer = std::make_shared<SecureInitializer>();
//         m_secureInitializer->initConfig(pt);
//         // std::cout << "m_secureInitializer init" << std::endl;
//         m_p2pInitializer = std::make_shared<P2PInitializer>();
//         m_p2pInitializer->setSSLContext(
//             m_secureInitializer->SSLContext(SecureInitializer::Usage::ForP2P));
//         m_p2pInitializer->setKeyPair(m_secureInitializer->keyPair());
//         m_p2pInitializer->initConfig(pt);
//     }
//     P2PInitializer::Ptr p2pInitializer() { return m_p2pInitializer; }
//     ~ECP2PService()
//     {
//        ls if (m_p2pInitializer)
//         {
//             m_p2pInitializer->stop();
//         }
//     }

// private:
//     P2PInitializer::Ptr m_p2pInitializer;
//     SecureInitializer::Ptr m_secureInitializer;
// };

// void d(){
//     std::string path = "./" + to_string(m_shardId);
//     executiveContext = std::make_shared<ExecuteVMTestFixture>(path);
//     /// hardcode deploy contract

//     /// deploy compute contract & data contract
//     ifstream deployFile("./deploy.json");
//     Json::Reader deployReader;
//     Json::Value deployRoot;
//     int drs = 0;
//     if (deployReader.parse(deployFile, deployRoot))
//     {
//         drs = deployRoot.size();
//         drs = drs;
//         for (int i = 0; i < 1; i++)
//         {
//             std::string tmp = deployRoot[i].asString();
//             Transaction::Ptr tx = std::make_shared<Transaction>(
//                     jsToBytes(tmp, OnFailed::Throw), CheckTransaction::Everything);
//             auto vm = executiveContext->getExecutiveInstance();
//             auto exec =  executiveContext->getExecutive();
//             exec->setVM(vm);
//             executiveContext->executeTransaction(exec, tx);
//             executiveContext->m_vminstance_pool.push(vm);
//         }
//     }
// }

void thread_insert_txs_task(dev::rpc::Rpc* rpcService)
{
    int deploy_len = 0;
    Json::Reader reader;
    Json::Value root;
    vector<std::string> transaction_cons;
    vector<Transaction::Ptr> transactions;
    vector<std::string> transaction_con;
    // ExecuteVMTestFixture::Ptr evmt = std::make_shared<ExecuteVMTestFixture>();
    int ns = 0;
    if (mynodeId == h512("6010ad7bf2a76c3563b07f40ec0a98472e34c3dec9850ef03a183fc5cfa473f1b"
                         "149f8358c5"
                         "6d9b20bbca911b6159eac01b3ffeb2930ee629eee79c95e00e1e0"))
    {
        // 部署智能合约
        std::string tmp_deploy;
        ifstream infiles("./deploysmallbank_2021.json", ios::binary);
        assert(infiles.is_open());
        if (reader.parse(infiles, root))
        {
            deploy_len = root.size();
            // std::cout<<"           desize:"<< deploy_len <<std::endl;
            for (int i = 0; i < deploy_len; i++)
            {
                tmp_deploy = root[i].asString();

                rpcService->sendRawTransaction(1, tmp_deploy);
            }
        }
        else{
            std::cout<<"no open file!!!!!!!!!!"<<std::endl;
        }

        // sleep(1);
        infiles.close();
        // sleep(5);
        // ifstream infiles2("./deploysmallbank.json", ios::binary);
        // assert(infiles2.is_open());
        // if (reader.parse(infiles2, root))
        // {
        //     deploy_len = root.size();
        //     // auto vm = evmt->getExecutiveInstance();
        //     for (int i = 0; i < deploy_len; i++)
        //     {
        //         std::string tmp = root[i].asString();

        //         rpcService->sendRawTransaction(1, tmp);
        //     }
        // }

        // // sleep(1);
        // infiles2.close();
        // }

        for (int i = 1; i < 2; i++)
        {
            std::cout << "number " << i << std::endl;
            // 注入交易
            string str1 = "tx";
            string str2 = ".json";
            string str3 = to_string(i);
            str1.append(str3);
            str1.append(str2);
            // ifstream infile(str1, ios::binary);
            ifstream infile("tx.json", ios::binary);
            assert(infile.is_open());
            std::string rlpStr;
            std::string response;
            int transaction_count = 0;
            int block_num = 0;
            int current_block_num = 0;

            double dotime = 0;

            int transaction_num_in_block = 0;

            auto t1 = GetTime();
            if (reader.parse(infile, root))
            {
		ns = 500000;    
                // ns = root.size();
                cout << "txs len  = " << ns << endl;
                // 仅执行一笔集交易，而非ns
                for (int j = 0; j < ns; j++)
                {
                    // sleep(1);
                    // if(j%1000==0){
                    //     auto t2 = GetTime();
                    //     std::cout << "inject 1000 txs cost " << t2-t1 <<std::endl;
                    //     t1=t2; 
                    // }
                    std::string tmp = root[j].asString();
                    // rpcService->sendRawTransaction(1, tmp);
                    // Transaction::Ptr tx = std::make_shared<Transaction>(
                    //     jsToBytes(tmp, OnFailed::Throw),
                    //     CheckTransaction::Everything);
                    // transactions_exe.push_back(tx);
                    // std::cout << "nonce = " << tx->nonce() << std::endl;
                    // string fout="./nonce.txt";
                    // std::ofstream out_key(fout,std::ofstream::app);
                    // out_key<< tx->nonce() << std::endl;
                    // out_key.close();
                    // transactions.push_back(tx);
                    // txs.push_back(tmp);
                    // std::cout<<"ns="<<ns<<"  i="<<i<<endl;
                    rpcService->sendRawTransaction(1, tmp);
                }
            }
            infile.close();
            // for (int i = 0; i < txs.size(); i++)
            // {
            std::cout<<"insert tx end!"<<endl;
            //     if(i %500000 == 0)
            //     {
            //         sleep(3);
            //     }
            //     // sleep(1);
            //     // cout << transaction_con[i] <<endl;
            // }
        }
    }
    // std::cout<<"!!!!!3"<<endl;
    // std::thread threads[THREAD_NUM];
    // int patch = transactions_exe.size() / THREAD_NUM;
    // for (int i = 0; i < THREAD_NUM; i++)
    // {
    //     if (i != TH
    return;
}

// void putGroupPubKeyIntoService(
//     std::shared_ptr<Service> service, boost::property_tree::ptree const& _pt)
// {
//     std::map<GROUP_ID, h512s> groupID2NodeList;
//     h512s nodelist;
//     int groupid;
//     for (auto it : _pt.get_child("group"))
//     {
//         if (it.first.find("groups.") == 0)
//         {
//             std::vector<std::string> s;
//             try
//             {
//                 boost::split(s, it.second.data(), boost::is_any_of(":"),
//                 boost::token_compress_on); if (s.size() != 2)
//                 {
//                     exit(1);
//                 }
//                 h512 node;
//                 node = h512(s[0]);
//                 nodelist.push_back(node);
//                 groupid = (int)((s[1])[0] - '0');
//             }
//             catch (std::exception& e)
//             {
//                 exit(1);
//             }
//         }
//     }
//     groupID2NodeList.insert(std::make_pair(groupid, nodelist));
//     service->setGroupID2NodeList(groupID2NodeList);
// }

// void test_ec(std::shared_ptr<ec::Eurasure> eurasure, int block_number,
//     std::map<int, std::map<int, std::string>>& states)
// {

//     for (auto it = states.begin(); it != states.end(); ++it)
//     {

//         eurasure->saveChunk(it->second, block_number, it->first);

//     }
//     eurasure->makeMerkleRoot(block_number);
//     eurasure->setCompleteCodingEpoch(block_number);
//     std::cout << "finish ec " << std::endl;
// }
// void readChunkFromDB(
//     int block_number, int group_num, int pos, std::vector<std::string>&
//     v,rocksdb::DB* test_db)
// {
//     for (int i = 0; i < group_num; i++)
//     {
//         std::string db_key =
//         mainDecIntToHexStr(block_number)+'|'+mainDecIntToHexStr(i) + "," +
//         mainDecIntToHexStr(pos); std::string db_value;
//         test_db->Get(ReadOptions(), db_key, &db_value); if (db_value.length()
//         == 0)
//         {
//             std::cout << "db_key = " << db_key << std::endl;
//             std::cout << "db_value.length() = 0" << std::endl;
//             exit(0);
//         }
//         // if (i == 8 && pos == 4)
//         // {
//         //     std::cout << "readChunkFromDB   hash = " <<
//         sha256(db_value).hex() << std::endl;
//         // }
//         v.push_back(db_value);
//     }
// }
// bool verifyChunkMerkleRoot(
//     std::vector<std::string>& level_to_chunk, int block_number, int
//     chunk_pos, int group_id,rocksdb::DB* test_db)
// {
//     std::string merkleroot;
//     string mid = "+";
//     std::string left, right, values;
//     std::string roots = "";
//     std::string key_root = mainDecIntToHexStr((unsigned int)block_number)
//                                .append(mid.append(mainDecIntToHexStr((unsigned
//                                int)chunk_pos)));
//     test_db->Get(ReadOptions(), key_root, &merkleroot);
//     std::cout << "merkleroot =" << merkleroot << std::endl;

//     values = level_to_chunk[0];
//     // left = sha256(level_to_chunk[0]).hex();
//     // right = sha256(level_to_chunk[1]).hex();
//     // values = sha256(left.append(right)).hex();
//     int pos = group_id;
//     bool flag =false;
//     for (int i = 1; i < level_to_chunk.size(); ++i)
//     {
//         if (!flag)
//         {
//             if (pos % 2 == 1)
//             {
//                 left = level_to_chunk[i];
//                 right = sha256(values).hex();
//                 values = sha256(left.append(right)).hex();
//             }
//             else
//             {
//                 left = sha256(values).hex();
//                 right = level_to_chunk[i];
//                 values = sha256(left.append(right)).hex();
//             }
//             flag = true;
//         }
//         else
//         {
//             if (pos % 2 == 1)
//             {
//                 left = level_to_chunk[i];
//                 right = values;
//                 values = sha256(left.append(right)).hex();
//                 // printf("verifyChunkMerkleRoot ---hash(hash(%s), hash(%s))
//                 =>%s\n", left.c_str(),
//                     right.c_str(), values.c_str());
//             }
//             else
//             {
//                 left = values;
//                 right = level_to_chunk[i];
//                 values = sha256(left.append(right)).hex();
//                 // printf("verifyChunkMerkleRoot ---hash(hash(%s), hash(%s))
//                 =>%s\n", left.c_str(),
//                     right.c_str(), values.c_str());
//             }
//         }

//         pos /= 2;
//     }
//     roots = values;

//     std::cout << "verifyChunkMerkleRoot  " << roots << std::endl;
//     assert(roots == merkleroot);
//     std::cout << level_to_chunk[0] << std::endl;
//     std::cout << "verifyChunkMerkleRoot successfully!" << std::endl;
//     return roots == merkleroot ? true : false;
// }
// std::vector<std::string> readChunkAndComputeMerkleHashs(
//     int block_number, int chunk_pos, int group_id,rocksdb::DB* test_db)
// {
//     std::vector<std::string> v;
//     std::vector<std::string> res;

//     readChunkFromDB(block_number, 16, chunk_pos, v,test_db);

//     int pos = group_id;
//     // std::cout << "start pos = " << pos << std::endl;
//     res.push_back(v[group_id]);
//     if (pos % 2 == 0)
//     {
//         if (pos == v.size() - 1)
//             res.push_back("");
//         else
//             res.push_back(sha256(v[group_id + 1]).hex());
//     }
//     else
//         res.push_back(sha256(v[group_id - 1]).hex());

//     pos /= 2;

//     std::vector<std::string> new_merkle;
//     bool flag = true;
//     while (new_merkle.size() > 1 || flag)
//     {
//         std::cout << "pos = " << pos << "  new_merkle.size() = " <<
//         new_merkle.size() << std::endl; if (flag)
//         {
//             if (v.size() % 2 == 1)
//                 v.push_back("");
//             flag = false;
//             vector<string> result;

//             for (int i = 0; i < v.size(); i += 2)
//             {
//                 string var1 = sha256(v[i]).hex();
//                 string var2 = sha256(v[i + 1]).hex();
//                 string hash = sha256(var1 + var2).hex();
//                 result.push_back(hash);
//             }
//             new_merkle = result;
//         }
//         else
//         {
//             if (new_merkle.size() % 2 == 1)
//                 new_merkle.push_back("");

//             vector<string> result;

//             for (int i = 0; i < new_merkle.size(); i += 2)
//             {
//                 // std::cout << "pos = " <<pos <<  "; i = " << i <<
//                 std::endl;
//                 // std::cout << "pos & 1 == 0 && pos == i = " <<(pos & 1 == 0
//                 && pos == i) << std::endl;
//                 // std::cout << "pos & 1 == 1 && pos == i + 1 = " <<(pos & 1
//                 == 1 && pos == i + 1) << std::endl; if (pos %2  == 0 && pos
//                 == i)
//                 {
//                     res.push_back(new_merkle[i + 1]);
//                 }
//                 else
//                 {
//                     if (pos %2 == 1 && pos == i + 1)
//                     {
//                         res.push_back(new_merkle[i]);
//                     }
//                 }

//                 string var1 = new_merkle[i];
//                 string var2 = new_merkle[i + 1];
//                 string hash = sha256(var1 + var2).hex();
//                 printf("---readChunkAndComputeMerkleHashs hash(hash(%s),
//                 hash(%s)) => %s\n",
//                     new_merkle[i].c_str(), new_merkle[i + 1].c_str(),
//                     hash.c_str());
//                 result.push_back(hash);
//             }
//             pos /= 2;
//             std::cout << "pos = " << pos << std::endl;
//             new_merkle = result;
//         }
//     }
//     // for(int i = 0;i < res.size(); ++i)
//     // {
//     //     if(i == 0)
//     //     std::cout << "res hash = " << sha256(res[i]).hex() << std::endl;
//     //     else
//     //     std::cout << "res hash = " << res[i] << std::endl;
//     // }
//     return res;
// }
// std::string constructMerkleRoot(int pos, int group_num, int
// block_number,rocksdb::DB* test_db)
// {
//     std::vector<string> v;
//     readChunkFromDB(block_number, group_num, pos, v,test_db);

//     vector<string> new_merkle;
//     bool flag = true;
//     while (new_merkle.size() > 1 || flag)
//     {
//         if (flag)
//         {
//             if (v.size() % 2 == 1)
//                 v.push_back("");
//             flag = false;
//             vector<string> result;

//             for (int i = 0; i < v.size(); i += 2)
//             {
//                 string var1 = sha256(v[i]).hex();
//                 string var2 = sha256(v[i + 1]).hex();
//                 string hash = sha256(var1 + var2).hex();
//                 printf("constructMerkleRoot ---hash(hash(%s), hash(%s))
//                 =>%s\n", var1.c_str(),
//                     var2.c_str(), hash.c_str());
//                 result.push_back(hash);
//             }
//             new_merkle = result;
//         }
//         else
//         {
//             if (new_merkle.size() % 2 == 1)
//                 new_merkle.push_back("");

//             vector<string> result;

//             for (int i = 0; i < new_merkle.size(); i += 2)
//             {
//                 string var1 = new_merkle[i];
//                 string var2 = new_merkle[i + 1];
//                 string hash = sha256(var1 + var2).hex();
//                 printf(" constructMerkleRoot ---hash(hash(%s), hash(%s)) =>
//                 %s\n",
//                     new_merkle[i].c_str(), new_merkle[i + 1].c_str(),
//                     hash.c_str());
//                 result.push_back(hash);
//             }
//             new_merkle = result;
//         }
//     }
//     return new_merkle[0];
// }
// void merkletest(rocksdb::DB* test_db)
// {
//     int data_num = 16;
//     vector<string> v(data_num, "");
//     for (int i = 0; i < 1000; ++i)
//     {
//         for (int k = 0; k < data_num * 5; ++k)
//         {
//             // v[i] = to_string(i);
//             // std::string s1= to_string(i / 5);
//             // std::string s2 ="+";
//             // std::string s3 =
//             test_db->Put(WriteOptions(),
//                 mainDecIntToHexStr(i)+'|'+mainDecIntToHexStr(k / 5) + "," +
//                 mainDecIntToHexStr(k % 5), mainDecIntToHexStr(k));
//         }
//         for (int j = 0; j < 4 + 1; j++)
//         {
//             std::string root = constructMerkleRoot(j, data_num, i,test_db);
//             string mid = "+";
//             std::string key = mainDecIntToHexStr((unsigned int)i)
//                                   .append(mid.append(mainDecIntToHexStr((unsigned
//                                   int)j)));
//             // std::cout << "constructMerkleRoot key = " << key << "     root
//             = " << root <<
//             // std::endl;

//             test_db->Put(WriteOptions(), key, root);
//         }

//         for(int k = 0; k < data_num * 5; ++k)
//         {
//             std::vector<std::string> v = readChunkAndComputeMerkleHashs(i,k %
//             5,k / 5,test_db); verifyChunkMerkleRoot(v,i,k % 5,k / 5,test_db);
//         }
//     }
// }

class ECP2PService
{
public:
    ECP2PService(std::string const& _path)
    {
        boost::property_tree::ptree pt;
        boost::property_tree::read_ini(_path, pt);
        m_secureInitializer = std::make_shared<dev::initializer::SecureInitializer>();
        m_secureInitializer->initConfig(pt);
        // std::cout << "m_secureInitializer init" << std::endl;
        m_p2pInitializer = std::make_shared<dev::initializer::P2PInitializer>();
        m_p2pInitializer->setSSLContext(
            m_secureInitializer->SSLContext(dev::initializer::SecureInitializer::Usage::ForP2P));
        m_p2pInitializer->setKeyPair(m_secureInitializer->keyPair());
        m_p2pInitializer->initConfig(pt);
    }
    dev::initializer::P2PInitializer::Ptr p2pInitializer() { return m_p2pInitializer; }
    ~ECP2PService()
    {
        std::cout << "~ECP2PService" << std::endl;
        if (m_p2pInitializer)
        {
            m_p2pInitializer->stop();
        }
    }

private:
    dev::initializer::P2PInitializer::Ptr m_p2pInitializer;
    dev::initializer::SecureInitializer::Ptr m_secureInitializer;
};
void putGroupPubKeyIntoService(
    std::shared_ptr<Service> service, boost::property_tree::ptree const& _pt)
{
    std::map<GROUP_ID, h512s> groupID2NodeList;
    h512s nodelist;
    int groupid;
    for (auto it : _pt.get_child("group"))
    {
        if (it.first.find("groups.") == 0)
        {
            std::vector<std::string> s;
            try
            {
                boost::split(s, it.second.data(), boost::is_any_of(":"), boost::token_compress_on);
                if (s.size() != 2)
                {
                    exit(1);
                }
                h512 node;
                node = h512(s[0]);
                nodelist.push_back(node);
                groupid = (int)((s[1])[0] - '0');
            }
            catch (std::exception& e)
            {
                exit(1);
            }
        }
    }
    groupID2NodeList.insert(std::make_pair(groupid, nodelist));
    service->setGroupID2NodeList(groupID2NodeList);
}
void readConfig(std::string config_path, boost::property_tree::ptree& pt)
{
    boost::property_tree::read_ini(config_path, pt);
}
std::shared_ptr<dev::p2p::Service> initP2PService(boost::property_tree::ptree& pt)
{
    ECP2PService* ecP2Pservice = new ECP2PService("./configec.ini");
    auto Ecp2pService = ecP2Pservice->p2pInitializer()->p2pService();
    putGroupPubKeyIntoService(Ecp2pService, pt);
    Ecp2pService->start();

    return Ecp2pService;
}
void getNodeList(dev::h512s& sealers, std::string config_path)
{
    boost::property_tree::ptree pt;
    readConfig(config_path, pt);

    for (auto it : pt.get_child("consensus"))
    {
        if (it.first.find("node.") == 0)
        {
            // std::cout << it.second.data() << std::endl;
            std::string data = it.second.data();
            boost::to_lower(data);
            // Uniform lowercase nodeID
            dev::h512 nodeID(data);
            sealers.push_back(nodeID);
        }
    }
}
bool initEC(std::shared_ptr<dev::blockchain::BlockChainInterface> _blockmanager)
{
    boost::property_tree::ptree ecpt;
    readConfig("./configec.ini", ecpt);
    auto service = initP2PService(ecpt);
    GROUP_ID ecId = std::stoi(ecpt.get<std::string>("group.group_id", "group"));
    auto nodeIdstr = asString(contents("conf/node.nodeid"));
    PROTOCOL_ID ECId = getGroupProtoclID(ecId, ProtocolID::ECProtocol);
    NodeID nodeId = NodeID(nodeIdstr.substr(0, 128));

    // dev::h512s NodeLists;
    // getNodeList(NodeLists, "./conf/group.1.genesis");
    h512s NodeLists = {
        h512("6010ad7bf2a76c3563b07f40ec0a98472e34c3dec9850ef03a183fc5cfa473f1b"
             "149f8358c56d9b20bbca"
             "911b6159eac01b3ffeb2930ee629eee79c95e00e1e0"),
        h512("fc31df672016011f4390b13ae8418f2366ffef30bfc341c88772bf7e039728cbe"
             "86916cef3ce5196b94df"
             "c1e4f3d89b7f2d255970ff0630a9f9ba2d20b7a0838"),
        h512("88ddaf6a89af6cc5afecccc40c48fb73ced7adf310385fe80b23952d76b9b9805"
             "cee27a32c4efddda4b65"
             "5ace355a92eaa9a8c5caca78c3fa7a873f73bf5aba7"),
        h512("28fd87f50f9a5c2d9936d4c51c53e5ce637b087c40dca953f7df3210a7b8954d4"
             "66979fb74a25598f7987"
             "9876ebc985bc6b9ce10f62861a425ecce62c3fa4e2d"),
        h512("3fe3cd4d10fd6b862c9f6439de941bc676930661beb9e15bf692f0b34d45b7b24"
             "fd1ed7a9e469dd2604aa"
             "d9921da0bdf6b700f9c09fb124c7bb2588dbdf101a7"),
        h512("0a634a3406f274792a18b173f8e7a251f8abaf56d1840bdec54ce15a9d1633061"
             "d6fa9a7b4b5d5420675b"
             "f359046afd61a3bba11d44ad32fc0c3ace6fb416f1d"),
        h512("1947f78c8d2ffb2c60d2c1823e73fe431612a7f96d8cbf6a489b2c0e33b3aa01f"
             "f89e94bfbf799aaffade"
             "e4ce9dab6d47956484e75bcbed768bc5c477d8b8e6b"),
        h512("507a49f9591cfe2cfe2e122b19774f3a66072bd4c513854740b1143091e134ef6"
             "38334d6ba95dac487729"
             "f71e4cdb685222649c90120fc79d61129ba761299dd"),
    };
    // std::cout << "NodeLists size = " << NodeLists.size() << std::endl;
    p2pHandler = new ec::EurasureP2P(nodeId, service, ECId);
    int64_t k = 2;
    int64_t m = 2;
    int64_t c = 1;
    std::cout << "k = " << k << "; m = " << m << "; c = " << c << std::endl;
    rocksdb::Options options;
    options.create_if_missing = true;

    rocksdb::DB* ec_db;
    std::string dbpath = "./ecstorage/";
    rocksdb::Status status = rocksdb::DB::Open(options, dbpath, &ec_db);
    assert(status.ok());

    // state_erasure = std::make_shared<ec::Eurasure>(
    //     k, m, c, nodeId, NodeLists, dbpath, p2pHandler, subcommit_num);
    int group_len = 64;
    dev::mptstate::MPTState::state_erasure =
        new ec::Eurasure(k, m, c, nodeId, NodeLists, dbpath, p2pHandler, group_len, _blockmanager);
    p2pHandler->setEurasure(dev::mptstate::MPTState::state_erasure);
    dev::mptstate::MPTState::state_erasure->setDBHandler(ec_db);
    dev::mptstate::MPTState::state_erasure->InitEurasure();
    std::cout << "init erasure finish" << std::endl;
}
void GenerateECFromKV(int kv_number, int rounds, int dimen = 1){
    int block_number = 100;
    int a[5] = {5000,10000,50000,100000,200000};
    int _k=1;
    if(dimen == 2){
        _k = dev::mptstate::MPTState::state_erasure->getK();
        a[4] = a[4]/_k;
        cout<<"2D coding"<<std::endl;
    }
    for(int i=0; i<rounds; i++){
        dev::mptstate::MPTState::state_erasure->makeECFromKV(block_number + i, a[4]);
    }
    sleep(5);
    for(int j=0; j<1; j++){
        auto search = dev::mptstate::MPTState::state_erasure->_search;
        ofstream ofile("./DecodingTime.txt",ios::app);
        ofile << "---k=" <<  dev::mptstate::MPTState::state_erasure->getK()
            << ",m=" <<  dev::mptstate::MPTState::state_erasure->getM()
            << "---" <<std::endl;
        for(int k=0; k<rounds; k++){
            if(dev::mptstate::MPTState::state_erasure->getPosition() == 0){
                auto start_decoding = GetTime();
                dev::mptstate::MPTState::state_erasure->decode(block_number + k, search);
                //decode(block_number,_search);
                auto end_decoding = GetTime();
                ofile << "Decoding state number = " << a[4]*_k << "/" << a[4]
                    <<" ; Cost time = " << end_decoding - start_decoding << "s" << std::endl;
            }
        }
        ofile << "------------"<<std::endl;
        ofile.close();
    }
    
    sleep(100);
}

int main(int argc, char* argv[])
{
    // VCGroup::testvc();
    // return 0;
    // rocksdb::Options options;
    // options.create_if_missing = true;
    // rocksdb::DB* vc_db;
    // rocksdb::Status status = rocksdb::DB::Open(options, "./vcstorage/",
    // &vc_db); assert(status.ok()); VCGroup::initTemplate(vc_db, 256,256);
    // double start_time = GetTime();
    // int block_numer = 1000;
    // for(int i =1;i<=block_numer;++i)
    // {
    //     dev::mptstate::MPTState::state_erasure->initTestStates(i);
    // }
    // return 0;
    // double end_time = GetTime();
    // // std::cout << "time = " <<end_time  - start_time << " tps = " <<
    // 4000*block_numer /
    // ((end_time  - start_time)*1.0) << std::endl; return 0; tests();
    // rocksdb::Options options; options.create_if_missing = true; rocksdb::DB*
    // vc_db; rocksdb::Status status = rocksdb::DB::Open(options,
    // "./vcstorage/", &vc_db); merkletest(vc_db); return 0;

    std::shared_ptr<dev::initializer::Initializer> initialize =
        std::make_shared<dev::initializer::Initializer>();
    initialize->init("./config.ini");

    // boost::property_tree::ptree ecpt;
    // boost::property_tree::read_ini("./configec.ini", ecpt);
    // ECP2PService ecP2Pservice("./configec.ini");
    // auto Ecp2pService = ecP2Pservice.p2pInitializer()->p2pService();
    // putGroupPubKeyIntoService(Ecp2pService, ecpt);
    // Ecp2pService->start();
    // GROUP_ID ecId = std::stoi(ecpt.get<std::string>("group.group_id",
    // "group"));
    auto nodeIdstr = asString(contents("conf/node.nodeid"));
    // PROTOCOL_ID ECId = getGroupProtoclID(ecId, ProtocolID::ECProtocol);

    NodeID nodeId = NodeID(nodeIdstr.substr(0, 128));
    // std::cout << "nodeid = " << nodeId << std::endl;
    mynodeId = nodeId;
    // ec::EurasureP2P* p2pHandler = new ec::EurasureP2P(nodeId, Ecp2pService,
    // ECId);

    // auto secureInitializer = initialize->secureInitializer();
    auto ledgerManager = initialize->ledgerInitializer()->ledgerManager();
    auto chainManager = ledgerManager->blockChain(1);
    auto consensusP2Pservice = initialize->p2pInitializer()->p2pService();
    auto rpcService = new dev::rpc::Rpc(ledgerManager, consensusP2Pservice);
    auto dbInitializer = ledgerManager->ledger(1)->getDBInitializer();
    // qqf 我尝试取消对consensumanager的注释
    // auto consensusManager = ledgerManager->consensus(1);
    auto blockVerifierManager = ledgerManager->blockVerifier(1);
    initEC(chainManager);

    // dev::mptstate::MPTState::state_erasure->decode(20000,)
    
    // 通过KV.txt来生成chunk
    // sleep(2);
    // std::thread g_thread;
    // g_thread = std::thread(GenerateECFromKV, 30000, 40, 2);
    // g_thread.join();
    // GenerateECFromKV(30000,40);

    // for (int i = 100; i >=1; --i)
    // {
    //     dev::mptstate::MPTState::state_erasure->initTestStates(i);
    // }
    // std::cout << "init end" << std::endl;
    // std::cout << "all size = "
    //           << dev::mptstate::MPTState::state_erasure->tmp_states_size / 1024.0 / 1024.0
    //           << std::endl;
    // std::cout << "state_size = " <<
    // dev::mptstate::MPTState::state_erasure->tmp_states_size << std::endl;

    // Client* client;
    // if (nodeId == h512("0a634a3406f274792a18b173f8e7a251f8abaf56d1840bdec54ce15a9d1633061"
    //                    "d6fa9a7b4b"
    //                    "5d5420675bf359046afd61a3bba11d44ad32fc0c3ace6fb416f1d"))
    // {
    // if (nodeId == h512("6010ad7bf2a76c3563b07f40ec0a98472e34c3dec9850ef03a183fc5cfa473f1b149f8358c5"
    //                    "6d9b20bbca911b6159eac01b3ffeb2930ee629eee79c95e00e1e0"))
    // {
    //     client = new Client(dbInitializer, chainManager);
    //     client->initClient();
    //     // client->readKey("./block_key.log");
    //     // client->readKey("./block.log");
    // }

    // int64_t k = 4;
    // int64_t m = 1;
    // int64_t c = 1;
    // std::cout << "k=" << k << "; m = " << m << "c = " << c << std::endl;
    // std::string dbpath = "./ecstorage/";
    // rocksdb::Options options;
    // options.create_if_missing = true;
    // rocksdb::DB* vc_db;
    // rocksdb::Status status = rocksdb::DB::Open(options, "./vcstorage/",
    // &vc_db); assert(status.ok()); rocksdb::DB* before_ec_db; status =
    // rocksdb::DB::Open(options, "./beforeecstorage/", &before_ec_db);
    // assert(status.ok());
    // rocksdb::DB* bf_db;
    // status = rocksdb::DB::Open(options, "./bfstorage/", &bf_db);
    // assert(status.ok());
    // // blockVerifierManager->setDB(before_ec_db);
    // int group_size = 16;
    // std::shared_ptr<ec::Eurasure> eurasure =
    // std::make_shared<ec::Eurasure>(k, m, c, nodeId,
    //     NodeLists, dbpath, p2pHandler, vc_db, before_ec_db, chainManager,
    //     group_size);
    // p2pHandler->setEurasure(eurasure);
    // eurasure->InitEurasure();
    int count = 1;
    // basic_bloom_filter bf(0.01, 3000);
    std::thread insert_thread;
    // if (nodeId ==
    // h512("6010ad7bf2a76c3563b07f40ec0a98472e34c3dec9850ef03a183fc5cfa473f1b149f8358c5"
    //                    "6d9b20bbca911b6159eac01b3ffeb2930ee629eee79c95e00e1e0"))
    // {
    insert_thread = std::thread(thread_insert_txs_task, rpcService);
    // }
    // std::cout<<"!!!!!2"<<endl;
    std::thread ec_thread;
    ec_thread = std::thread(thread_ec_task, chainManager);
    // dev::mptstate::MPTState::state_erasure->initVC();
    // for(int i =1;i<=1;++i)
    // {
    //     dev::mptstate::MPTState::state_erasure->initTestStates(i);
    // }
    // for (int i = 0; i < 5000; ++i)
    //     dev::mptstate::MPTState::state_erasure->mpt_test();
    // std::cout << "end !!!!!!!!!0" << std::endl;

    ec_thread.join();
    insert_thread.join();

    // sleep(10);
    // chainManager->getTransactionReceiptByHash(h256("2d1201e01cb74b4cff14f2d05cb3580c07235bf6ea8c259abe57aac870ba8381"));
    // 6010ad7bf2a7
    if (nodeId == h512("1010ad7bf2a76c3563b07f40ec0a98472e34c3dec9850ef03a183fc5cfa473f1b149f8358c5"
                       "6d9b20bbca911b6159eac01b3ffeb2930ee629eee79c95e00e1e0"))
    {
        auto client = new Client(dbInitializer, chainManager);
        client->initClient();
        sleep(15);
        int blk_num = 3;
        if(auto b = client->findKeyInBloomFilter("AD692F687445A9744D58B6CB820FFC28", blk_num)){
            std::cout << "---Key in "<< blk_num << "---" << std::endl;
        }
        else{
            std::cout << "---Key NOT in "<< blk_num << "---" << std::endl;
        }
        std::string m;

        client->getState("9DEAB240A21FF5B9BE2D45BB893D577B",3,m);
        // client->readKey("./block_key.log");
        // client->readKey("./block.log");
    }
    
    while (1)
    {
        // dev::mptstate::MPTState::state_erasure->getP2PHandle()->sendHeart(nodeId.hex(),h512("0a634a3406f274792a18b173f8e7a251f8abaf56d1840bdec54ce15a9d1633061d6fa9a7b4b5d5420675bf359046afd61a3bba11d44ad32fc0c3ace6fb416f1d"));
        
        
        if (nodeId == h512("0a634a3406f274792a18b173f8e7a251f8abaf56d1840bdec54"
                           "ce15a9d1633061d6fa9a7b4b5d5420675bf359046afd61a3bba"
                           "11d44ad32fc0c3ace6fb416f1d"))
        {
            // if (nodeId == h512("6010ad7bf2a76c3563b07f40ec0a98472e34c3dec9850ef03a183fc5cfa473f1b14"
            //                    "9f8358c56d9b20bbca911b6159eac01b3ffeb2930ee629eee79c95e00e1e0"))
            // {
                // while(count > chainManager->number()){sleep(0.01);} std::cout << "work" <<
                // std::endl;
                // client->work();
            }
            // break;

            // int block_number = chainManager->number();
            // if (count < block_number)
            // {
            //     // if (count == 2)
            //     // {
            //     auto states =
            //         initTestStates(eurasure, bf_db, count,
            //         bf.hasher_function(), before_ec_db);
            //     test_ec(eurasure, count, states);
            //     int sizes = count;
            //     // if (count > 3)
            //     // {
            //     //     int sizes = count - 3;
            //     std::hash<unsigned int> block_hash;
            //     unsigned int seed = block_hash(sizes);
            //     std::mt19937 rand_num(seed);
            //     sha256(boost::lexical_cast<string>(rand_num())).hex();
            //     std::string key =
            //     sha256(boost::lexical_cast<string>(rand_num())).hex();

            //     std::string block_num_str = mainDecIntToHexStr(sizes);
            //     std::string bf_str;
            //     bf_db->Get(ReadOptions(), block_num_str, &bf_str);
            //     std::stringstream ifs(bf_str);
            //     boost::archive::binary_iarchive ia(ifs);
            //     bitvector b1;
            //     ia >> b1;
            //     std::cout << "block num = " << sizes << "key = " << key <<
            //     std::endl; basic_bloom_filter obfc(bf.hasher_function(), b1);
            //     if (obfc.lookup(key.substr(0,32)))
            //     {
            //         std::string str = eurasure->getState(sizes, key);
            //         // std::cout << "str = " << str << std::endl;
            //         if (str.length() >= 183)
            //             std::cout << "read value = " <<
            //             eurasure->getKVAndProof(str, key, sizes)
            //                       << std::endl;
            //         else
            //         {
            //             std::cout << "read value = " << str << std::endl;
            //         }
            //         if (nodeId ==
            //         h512("6010ad7bf2a76c3563b07f40ec0a98472e34c3dec9850ef03a183fc5cfa473f"
            //                            "1b149f8358c5"
            //                            "6d9b20bbca911b6159eac01b3ffeb2930ee629eee79c95e00e1e0"))
            //         {
            //             std::string chunk_data = eurasure->decode(sizes,
            //             key);
            //             // std::cout << "decode data = " << chunk_data <<
            //             std::endl;

            //             auto t = eurasure->getKVAndProof(chunk_data,
            //             key,sizes); std::cout << "decode value = " << t <<
            //             std::endl;
            //         }
            //         // }
            //         // }
            //     }
            //     else
            //     {
            //         std::cout << "state  " << key << "  not in block " <<
            //         block_number << std::endl;
            //     }
            //     // }
            //     count++;
            //     sleep(1);
            // }
        }

        // tps_thread.join();
        // // block_thread.join();
        //  while (1)
        // {
        // }

        // if (nodeId ==
        // h512("6010ad7bf2a76c3563b07f40ec0a98472e34c3dec9850ef03a183fc5cfa473f1b149f8358c5"
        //                    "6d9b20bbca911b6159eac01b3ffeb2930ee629eee79c95e00e1e0"))
        // {
        // insert_thread.join();
        // }

        return 0;
    }
