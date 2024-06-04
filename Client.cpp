#include "Client.h"
#include <libdevcrypto/Hash.h>

#include <libmptstate/MPTState.h>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/serialization/vector.hpp>
#include <fstream>
#include <functional>
#include <libbloomfilter/bf/all.hpp>
#include <random>
#include <string>

static inline double GetTime()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec + tv.tv_usec / 1e6;
}
static std::string decIntToHexStr(dev::u256 const& num)
{
    std::string str;
    dev::u256 Temp = num / 16;
    dev::u256 left = num % 16;
    if (Temp > 0)
        str.append(decIntToHexStr(Temp));
    if (left < 10)
        str += ((char)left + '0');
    else
        str += ('A' + (char)left - 10);
    return str;
}
// static inline std::string DecToHex(unsigned int num)
// {
//     std::ostringstream buffer;
//     buffer << std::hex << std::uppercase << num;
//     return buffer.str();
// }
// inline std::string DecToHex(unsigned int num)
// {
//     std::ostringstream buffer;
//     buffer << std::hex << std::uppercase << num;
//     return buffer.str();
// }
bool Client::initClient()
{
    m_mptstate = m_dbInitializer->stateFactory()->getMPTState();
}
bool Client::findKeyInBloomFilter(std::string key, int block_height)
{
    double start_time = GetTime();
    std::string block_num_str = DecToHex(block_height);
    std::string bf_hasher;
    dev::mptstate::MPTState::state_erasure->getDBHandler()->Get(
        rocksdb::ReadOptions(), "bf_hasher", &bf_hasher);
    std::string bf_bit_vector;
    // std::cout << "client bf bitvector " <<
    // DecToHex(block_height).append("bf") << std::endl;

    dev::mptstate::MPTState::state_erasure->getDBHandler()->Get(
        rocksdb::ReadOptions(), DecToHex(block_height).append("bf"), &bf_bit_vector);
    // std::cout << "read bf_bit_vector = " << bf_bit_vector << std::endl;
    std::stringstream ifs(bf_hasher);
    boost::archive::binary_iarchive ia(ifs);
    bf::hasher _hasher;
    ia >> _hasher;
    ifs.str("");
    ifs.clear();
    if(bf_bit_vector == ""){
        std::cout << "bf is empty" << std::endl;
        return false;
    }
    std::stringstream ifs_bit(bf_bit_vector);
    boost::archive::binary_iarchive ia_bit(ifs_bit);
    bf::bitvector _bitvector;
    ia_bit >> _bitvector;
    ifs_bit.str("");
    ifs_bit.clear();
    bf::basic_bloom_filter obfc(_hasher, _bitvector);
    if (obfc.lookup(key.substr(0, 32)))
    {
        // std::cout << "bf lookup time = " << GetTime() - start_time << std::endl;
        // std::cout << key << " in block height " << block_height << std::endl;
        return true;
    }
    else
    {
        // std::cout << "bf lookup time = " << GetTime() - start_time << std::endl;
        // std::cout << "not find " << key << "in block height " << block_height
        // << std::endl;
        return false;
    }
}
void Client::getState(std::string key, int block_height, std::string& data)
{
    std::string kvproof;
    kvproof = dev::mptstate::MPTState::state_erasure->getState(block_height, key.substr(0, 32));
    std::cout << "kvproof = " << kvproof << std::endl;
    // if (data.length() >= 183) {
    //     data = dev::mptstate::MPTState::state_erasure->getKVAndProof(
    //         kvproof, key.substr(0, 32), block_height);
    //     std::cout << "read value = " << data << std::endl;
    // } else {
    // data = kvproof;
    // std::cout << "read value = " << kvproof << std::endl;
    // }
}
void Client::readKey(std::string path)
{
    // std::ifstream infiles(path);
    // assert(infiles.is_open());

    std::string line;
    int count = 100;
    // std::cout << "start read key" << std::endl;
    // while (infiles >> line)
    // {
    //     if (key_set_map.find(count) == key_set_map.end())
    //     {
    //         std::set<std::string> s;
    //         s.insert(decIntToHexStr(dev::u256(line)));
    //         key_set_map.emplace(make_pair(count, s));
    //     }
    //     else
    //     {
    //         key_set_map[count].emplace(decIntToHexStr(dev::u256(line)));
    //     }
    // }

    for (; count >= 1; --count)
    {
        std::hash<unsigned int> block_hash;
        unsigned int seed = block_hash(count);
        std::mt19937 generator(seed);
        std::uniform_int_distribution<long long> dist(1000, 1000);
        size_t state_size = dist(generator);
        seed = block_hash(count);
        std::mt19937 rand_num(seed);
        // std::string proof(49, '9');
        for (size_t i = 0; i < state_size; i++)
        {
            // int key = i;
            std::string key_str;

            if (count % 5 == 0)
                key_str =
                    "0a634a3406f274792a18b173f8e7a251f8abaf56d1840bdec54ce15a9d1633061d6fa9a7b4b5d5"
                    "420675bf359046afd61a3bba11d44ad32fc0c3ace6fb416f1d";
            else
                key_str = dev::sha256(boost::lexical_cast<std::string>(rand_num())).hex();

            // std::cout << "initTestStates key = " << key_str << std::endl;
            // std::string k = key_str;
            if (key_set_map.find(count) == key_set_map.end())
            {
                std::set<std::string> s;
                s.insert(key_str);
                key_set_map[count] = s;
            }
            else
            {
                key_set_map[count].insert(key_str);
            }
        }
    }

    // std::cout << "read key finish" << std::endl;
}
// static inline double GetTime() {
//     struct timeval tv;
//     gettimeofday(&tv, NULL);
//     return tv.tv_sec + tv.tv_usec / 1e6;
// }
void Client::work()
{
    // std::cout << "start work " << std::endl;
    int i = 0;
    int maxs = 100;
    std::hash<unsigned int> block_hash;
    unsigned int seed = block_hash(rand());
    std::mt19937 generator(seed);
    std::uniform_int_distribution<long long> dist(1, maxs);
    while (i++ < 1000)
    {
        size_t j = dist(generator);
        int count = 0;
        for (; j >= 1; --j)
        {
            int block_height = j;
            //     continue;
            // while (block_height >= m_chainManager->number())
            // {
            //     std::cout << "m_chainManager->number() = " <<
            //     m_chainManager->number()
            //               << "!!!block_height = " << block_height << std::endl;
            //     sleep(1);
            // }
            // int count = 1;
            // for (; count <= 100; ++count)
            // {
            //     std::hash<unsigned int> block_hash;
            //     unsigned int seed = block_hash(count);
            //     std::mt19937 generator(seed);
            //     std::uniform_int_distribution<long long> dist(2000, 2000);
            //     size_t state_size = dist(generator);
            //     seed = block_hash(count);
            //     std::mt19937 rand_num(seed);
            //     // std::string proof(49, '9');
            //     for (size_t i = 0; i < state_size; i++)
            //     {
            //         // int key = i;
            //         std::string key_str =
            //         dev::sha256(boost::lexical_cast<std::string>(rand_num())).hex();

            //         // std::cout << "initTestStates key = " << key_str << std::endl;
            //         // std::string k = key_str;
            //         if (key_set_map.find(count) == key_set_map.end())
            //         {
            //             std::set<std::string> s;
            //             s.insert(key_str);
            //             key_set_map.emplace(make_pair(count, s));
            //         }
            //         else
            //         {
            //             key_set_map[count].emplace(key_str);
            //         }
            //     }
            // }
            // std::cout << "block_height = " << it->first << std::endl;

            // int count = 8;
            // int thread_num = 4;


            // tbb::parallel_for(tbb::blocked_range<int>(1, count + 1, (count) / thread_num),
            //     [&](const tbb::blocked_range<int>& _r) {
            //         // std::cout<<"range begin:"<<_r.begin()<<std::endl;
            //         double start_time = GetTime();
            //         int size = 0;
            //         for (int pos = _r.begin(); pos != _r.end(); ++pos)
            //         {
            //             std::cout << "pos = " << pos << std::endl;
            //             std::hash<unsigned int> block_hash;
            //             unsigned int seed = block_hash(pos);
            //             std::mt19937 generator(seed);
            //             std::uniform_int_distribution<long long> dist(2000, 2000);
            //             size_t state_size = dist(generator);
            //             seed = block_hash(pos);
            //             std::mt19937 rand_num(seed);
            //             // std::string proof(49, '9');
            //             for (size_t i = 0; i < state_size; i++)
            //             {
            //                 // int key = i;
            //                 std::string key_str =
            //                     dev::sha256(boost::lexical_cast<std::string>(rand_num())).hex();

            //                 if (findKeyInBloomFilter(key_str.substr(0, 32), pos))
            //                 {
            //                     std::string data = "";
            //                     getState(key_str, pos, data);
            //                 }
            //             }
            //             size += state_size;

            //         }
            //          double end_time = GetTime();
            //           std::cout << size / (end_time - start_time) << std::endl;
            //     });


            
            // for (auto sub_it = key_set_map[j].begin(); sub_it != key_set_map[j].end(); ++sub_it)
            // {
            // std::cout << "client key = " << (*sub_it) << std::endl;
            ++count;
            std::string cli =
                "0a634a3406f274792a18b173f8e7a251f8abaf56d1840bdec54ce15a9d1633061d6fa9a7b4b5d5"
                "420675bf359046afd61a3bba11d44ad32fc0c3ace6fb416f1d";
            // if (findKeyInBloomFilter((*sub_it).substr(0, 32), block_height))
            if (findKeyInBloomFilter(cli.substr(0, 32), block_height))
            {
                int group_id = 0;
                int chunk_pos = 0;
                // dev::mptstate::MPTState::state_erasure->findKeyInWhichChunk(
                //     block_height, (*sub_it), group_id, chunk_pos);
                std::string data = "";
                // dev::mptstate::MPTState::state_erasure->readChunk(
                //     block_height, group_id, chunk_pos);
                // if (count % 5 != 0)
                double start_time = GetTime();
                getState(cli, block_height, data);
                double end_time = GetTime();
                break;
                // std::cout << "getstate time = " << end_time - start_time << std::endl;
                // else
                // dev::mptstate::MPTState::state_erasure->decode(block_height,
                //                                                (*sub_it));
                // test decoding
                // std::string sub_chunk =
                //     dev::mptstate::MPTState::state_erasure->decode(block_height,
                //                                                    (*sub_it));
                // if (data.compare(sub_chunk) != 0)
                // {
                //     std::cout << "chunk is incorrect" << std::endl;
                // }
                // data = dev::mptstate::MPTState::state_erasure->getKVAndProof(
                //     sub_chunk, (*sub_it), block_height);
                // double end_time = GetTime();
                // if (1 / (end_time - start_time) >= 1000)
                // std::cout << 1 / (end_time - start_time) << std::endl;
                // if (count % 5 != 0) {
                //     // std::cout << "key = " << (*sub_it) << " in block "
                //     //           << block_height << "   value = " << data
                //     //           << std::endl;
                // }

                // ++count;
            }
            // }
            
            // std::cout << " actual: " << count / (end_time - start_time) << std::endl;
            // std::cout << " actual: " << it->second.size() / (end_time - start_time) << std::endl;
        }
        std::cout  << count << std::endl;
        // std::string key =
        // "13473E93F3A9A6E47351A566A44FB32320A88BDA7B26E1C5CCDFEC66B2EAC0DA";
        // std::string key = "E0C194580A962A5A5914EF13D81811AF";
        // if (findKeyInBloomFilter(key, block_height))
        // {

        //     std::string data;
        //     getState((key), block_height, data);
        //     std::cout << "key = " << (key) << " in block " << block_height <<
        //     "   value = "
        //     << data
        //               << std::endl;
        // }
        // else
        // {
        //     block_height++;
        // }
        // }
    }
}
