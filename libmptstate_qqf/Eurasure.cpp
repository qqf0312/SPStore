#include "Eurasure.h"

#include <libdevcrypto/Hash.h>

#include "Eurasure-P2P.h"
#include "VCGroup.h"
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <fstream>
#include <functional>
#include <iostream>
#include <random>

using namespace ec;
using namespace std;
using namespace rocksdb;
using namespace dev::eth;
using namespace dev;
typedef tbb::concurrent_unordered_map<std::string, std::string> hash_block_map;
typedef tbb::concurrent_unordered_map<unsigned int, unsigned int> hash_chunk_count_map;

inline int ecComputePostion(const std::string& posStr, int mod)
{
    long res = 0;
    for (int i = 0; i < 4; ++i)
    {
        // res += data[i] * 2;
        if (posStr[i] >= 'A' && posStr[i] <= 'Z')
        {
            res = (res * 16 + (posStr[i] - 'A') + 10) % mod;
        }
        if (posStr[i] >= 'a' && posStr[i] <= 'z')
        {
            res = (res * 16 + (posStr[i] - 'a') + 10) % mod;
        }
        if (posStr[i] >= '0' && posStr[i] <= '9')
        {
            res = (res * 16 + (posStr[i] - '0')) % mod;
        }
    }
    return res % mod;
}

// int ecComputePostion(const char* data, int len)
// {
//     int res = 0;
//     for (int i = 0; i < len; ++i)
//     {
//         // res += data[i] * 2;
//         if (data[i] >= 'A' && data[i] <= 'Z')
//         {
//             res = res * 16 + (data[i] - 'A') + 10;
//         }
//         if (data[i] >= 'a' && data[i] <= 'z')
//         {
//             res = res * 16 + (data[i] - 'a') + 10;
//         }
//         if (data[i] >= '0' && data[i] <= '9')
//         {
//             res = res * 16 + (data[i] - '0');
//         }
//     }
//     return res;
// }
bool comLen(const std::set<StatePoint>& ls, const std::set<StatePoint>& rs)
{
    return ls.size() > rs.size();
}
string DecIntToHexStr(unsigned int num)
{
    string str;
    unsigned int Temp = num / 16;
    int left = num % 16;
    if (Temp > 0)
        str += DecIntToHexStr(Temp);
    if (left < 10)
        str += (left + '0');
    else
        str += ('A' + left - 10);
    return str;
}
void Eurasure::findKeyInWhichChunk(
    int const& block_number, std::string const& key, int& group, int& chunk_num)
{
    // group = ecComputePostion(key.substr(4,
    // partition_key_mappinto_vc_bytes_num).c_str(), group_len);
    int group_id = ecComputePostion(key.substr(0, 32), group_len);
    // std::cout << " findKeyInWhichChunk group_id = " << group_id << std::endl;
    group = group_id / (ec_k * number_of_vc_in_one_chunk) * ec_k;
    // add at 2021-11-4
    chunk_num = group_id % ec_k;
    // chunk_num = group_id % (ec_k + ec_m);
    // std::cout << "group = " << group << " ; chunk_num = " << chunk_num <<
    // std::endl; for (auto& s : fields)
    // {
    //     // std::cout << s << std::endl;
    //     std::vector<std::string> sub_field;
    //     boost::split(sub_field, s, boost::is_any_of("-"));

    //     std::cout << std::endl;
    //     if (std::atoi(sub_field[1].c_str()) >= sub_position &&
    //         std::atoi(sub_field[0].c_str()) <= sub_position)
    //         break;
    //     ++chunk_num;
    // }
}
void Eurasure::generate_ptrs(size_t data_size, uint8_t* data, erasure_bool* present, uint8_t** ptrs)
{
    size_t i;
    for (i = 0; i < ec_k + ec_m; ++i)
    {
        ptrs[i] = data + data_size * i;
    }

    for (i = 0; i < ec_k + ec_m; ++i)
    {
        present[i] = true;
    }
}
std::string Eurasure::GetChunkDataKey(
    unsigned int coding_epoch, unsigned group_id, unsigned chunk_pos)
{
    std::string str = std::to_string(coding_epoch);
    str.append("|");
    str.append(std::to_string(group_id));
    str.append("|").append(std::to_string(chunk_pos));
    return str;
}
void Eurasure::maxLen(std::map<int, std::map<int, std::string>>& states, int64_t group_id,
    std::map<int, std::string>& processed_data)
{
    for (int i = 0; i < ec_k; ++i)
    {
        for (int j = 1; j <= number_of_vc_in_one_chunk; ++j)
        {
            for (auto it = states[group_id + i * j].begin(); it != states[group_id + i * j].end();
                 ++it)
            {
                processed_data[i].append(it->second);
            }
            processed_data[i].append("!!!");
        }
    }
}

std::pair<uint8_t*, int64_t> Eurasure::preprocess(
    int block_number, int groupid, std::map<int, std::map<int, std::string>>& states)
{
    // int size = states.size();
    int size = states.size();
    std::map<int, std::string> processed_data;
    maxLen(states, groupid, processed_data);

    int max_len = processed_data[0].size();
    for (int i = 1; i < ec_k; ++i)
    {
        max_len = max(max_len, (int)processed_data[i].size());
    }
    uint8_t* data = new uint8_t[max_len * (ec_k + ec_m) * sizeof(uint8_t)]();
    memset(data, 0, max_len * (ec_k + ec_m) * sizeof(uint8_t));
    int64_t count = 0;
    //将数据预处理

    int counts = 0;
    for (auto it = processed_data.begin(); it != processed_data.end(); ++it)
    {
        memcpy(data + count, it->second.c_str(), it->second.size());
        count += max_len;
    }

    return make_pair(data, max_len);
}
static inline double GetTime()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec + tv.tv_usec / 1e6;
}
std::pair<uint8_t**, int64_t> Eurasure::encode(std::pair<uint8_t*, int64_t> blocks_rlp_data)
{
    int64_t length = blocks_rlp_data.second;
    uint8_t** ptrs = new uint8_t*[ec_k + ec_m];
    erasure_bool* present = new erasure_bool[ec_k + ec_m];
    generate_ptrs(length, blocks_rlp_data.first, present, ptrs);

    erasure_encoder_parameters params = {ec_k + ec_m, ec_k, length};
    erasure_encoder* encoder = erasure_create_encoder(&params, ec_mode);
    erasure_encode(encoder, ptrs, ptrs + ec_k);

    erasure_destroy_encoder(encoder);
    delete present;
    std::cout<<"encode data"<<endl;
    return std::make_pair(ptrs, length);
}

std::pair<int*, int*> Eurasure::get_my_chunk_set(unsigned int groupid)
{
    int* chunk_set = get_distinct_chunk_set(groupid);
    int* my_chunks = new int[ec_c];
    memset(my_chunks, 0, sizeof(int) * ec_c);
    for (int i = 0; i < ec_c; i++)
    {
        // int remain = (chunk_set[i] + ec_position_in_sealers) / (ec_k + ec_m);
        my_chunks[i] = (chunk_set[i] + ec_position_in_sealers) % (ec_k + ec_m);
    }
    // delete chunk_set;
    return std::make_pair(my_chunks, chunk_set);
}

int64_t Eurasure::findSeqInSealers()
{
    for (int i = 0; i < ec_sealers.size(); i++)
    {
        // std::cout << "ec_sealers[i] =" << ec_sealers[i] << std::endl;
        if (ec_nodeid == ec_sealers[i])
        {
            return i;
        }
    }
    return -1;
}
bool Eurasure::InitEurasure()
{
    ec_position_in_sealers = findSeqInSealers();
    // std::cout << " ec_position_in_sealers = " << ec_position_in_sealers << std::endl;
    initVC();
    // Options options;
    // options.create_if_missing = true;
    // Status status = DB::Open(options, ec_DBPath, &ec_db);
    // assert(status.ok());
    std::cout << "InitEurasure finish" << std::endl;
}

bool Eurasure::writeDB(
    unsigned int coding_epoch, unsigned int groupid, std::pair<uint8_t**, int64_t> const& chunk)
{
    int* chunk_set = get_distinct_chunk_set(coding_epoch);
    // std::cout << "chunk set :" << std::endl;
    // for(int i = 0; i < ec_k + 2*ec_m; i++) {
    //     std::cout << chunk_set[i] << " ";
    // }
    // std::cout << endl;
    int count = 0;
    // std::cout << string((const char*)chunk.first[0], chunk.second).size() / 1024.0 << std::endl;
    for (count = 0; count < ec_k + ec_m; count++)
    {
        // 2021-11-7
        // if (chunk_set[count] == ec_position_in_sealers)
        // {
        string str = GetChunkDataKey(coding_epoch, groupid, count);
        // std::cout << "writedb key   = " << str << std::endl;
        string value((const char*)chunk.first[count], chunk.second);

        Status status = ec_db->Put(WriteOptions(), str, value);
        assert(status.ok());
        // }
    }

    return true;
}
// void Eurasure::saveChunk(std::map<int, std::string> const& states, int
// block_number, int groupid,
//     std::map<int, std::map<int, std::string>>& cs)
// {
//     unsigned int coding_epoch = block_number;

//     std::pair<uint8_t**, int64_t> chunks = encode(preprocess(block_number,
//     groupid, states)); for (int count = 0; count < ec_k + ec_m; count++)
//     {
//          int pos = count;
//          string value((const char*)chunks.first[count], chunks.second);
//          cs[groupid][pos] = value;
//     }
//     writeDB(coding_epoch, groupid, chunks);
// }
void Eurasure::saveChunk(std::map<int, std::map<int, std::string>>& states, int block_number,
    int groupid, std::map<int, std::map<int, std::string>>& cs)
{
    // std::cout << "saveChunk" << std::endl;
    unsigned int coding_epoch = block_number;

    std::pair<uint8_t**, int64_t> chunks = encode(preprocess(block_number, groupid, states));
    for (int count = 0; count < ec_k + ec_m; count++)
    {
        int pos = count;
        string value((const char*)chunks.first[count], chunks.second);
        cs[groupid][pos] = value;
    }
    writeDB(coding_epoch, groupid, chunks);
}
int* Eurasure::get_distinct_chunk_set(unsigned int coding_epoch)
{
    std::hash<unsigned int> ec_hash;
    unsigned int seed = ec_hash(coding_epoch);
    std::mt19937 generator(seed);
    std::uniform_int_distribution<> dis(0, ec_k + ec_m);
    // std::uniform_int_distribution<> dis(0, ec_k + ec_m - 1);
    int* chunk_set = new int[ec_k + ec_m];
    int lucky_boy = dis(generator);
    // std::cout << "luck boy = " << lucky_boy << std::endl;
    for (int j = 0; j < ec_k + ec_m; j++)
    {
        int tmp = dis(generator);
        if (tmp == lucky_boy)
        {
            --j;
            continue;
        }
        chunk_set[j] = tmp;
        for (int i = 0; i < j; i++)
        {
            if (chunk_set[j] == chunk_set[i])
            {
                --j;
                break;
            }
        }
    }
    return chunk_set;
}

std::string Eurasure::decode(unsigned int coding_epoch, std::string key)
{
    double start_time = GetTime();
    erasure_bool* present = new erasure_bool[ec_k + ec_m];
    int* chunk_set = get_distinct_chunk_set(coding_epoch);
    // for (int i = 0; i < 5; i++)
    // {
    //     std::cout << chunk_set[i] << "  ";
    // }
    // std::cout << std::endl;
    int my_chunks;
    double starttime = GetTime();

    //计算在哪个组的第几个chunk

    int group_id = 0;
    int chunk_pos = 0;

    findKeyInWhichChunk(coding_epoch, key, group_id, chunk_pos);
    // std::cout << "findKeyInWhichChunk group_id = " << group_id
    //           << "  chunk_pos =" << chunk_pos << std::endl;

    //查询本地缺少的chunk
    //查询缺少的chunk所在的节点
    std::set<unsigned int> replica_chunk;
    std::set<unsigned int> no_replica_chunk;

    for (int i = 0; i < ec_k + ec_m; i++)
    {
        no_replica_chunk.insert(i);
    }

    my_chunks = chunk_set[chunk_pos];
    if (my_chunks == ec_position_in_sealers)
    {
        replica_chunk.insert(chunk_pos);
        no_replica_chunk.erase(chunk_pos);
    }

    unsigned int need_chunk_num = ec_k - replica_chunk.size();
    unsigned int need_chunk_count = need_chunk_num;
    bool flag = false;
    for (std::set<unsigned int>::iterator it = no_replica_chunk.begin();
         it != no_replica_chunk.end(); ++it)
    {
        flag = false;

        std::set<unsigned int> block_in_which_nodes;
        int res = chunk_set[*it % 6];
        if (res < 0)
        {
            res += (ec_k + ec_m);
        }

        if (res != ec_position_in_sealers)
        {
            // std::cout << "ec_eurasure_p2p->requestChunk"
            //           << "block num = " << coding_epoch << " group = " <<
            //           group_id
            //           << " pos = " << *it << "  sealer = " <<
            // ec_sealers[res]
            //           << std::endl;
            ec_eurasure_p2p->requestChunk(coding_epoch, group_id, *it, ec_sealers[res]);
        }
        // need_chunk_count--;
    }
    starttime = GetTime();
    // std::cout << "send complete" << std::endl;
    int num;
    std::string str = std::to_string(coding_epoch);
    str.append("-");
    str.append(std::to_string(group_id));
    auto its = ec_eurasure_p2p->chunk_count_map.find(str);
    if (its == ec_eurasure_p2p->chunk_count_map.end())
    {
        num = 0;
    }
    else
    {
        num = its->second;
    }

    while (num < need_chunk_num)
    {
        sleep(0.0001);
        its = ec_eurasure_p2p->chunk_count_map.find(str);
        if (its == ec_eurasure_p2p->chunk_count_map.end())
        {
            num = 0;
        }
        else
        {
            num = its->second;
        }
    }
    // std::cout << "wait chunk response time = " << GetTime() - starttime <<
    // std::endl;
    starttime = GetTime();
    std::set<unsigned int> get_chunk_pos;
    // for(auto sec_it =  ec_eurasure_p2p->chunk_data_map.begin();sec_it !=
    // ec_eurasure_p2p->chunk_data_map.end();++ sec_it)
    // {
    //     std::cout << " ec_eurasure_p2p->chunk_data_map = " <<
    // (*sec_it).first
    //     << std::endl;
    // }
    for (unsigned int i = 0; i < ec_k + ec_m; i++)
    {
        std::string str_count = GetChunkDataKey(coding_epoch, group_id, i);
        // std::cout << "str_count " << str_count << std::endl;
        auto sec_it = ec_eurasure_p2p->chunk_data_map.find(str_count);
        if (sec_it != ec_eurasure_p2p->chunk_data_map.end())
        {
            get_chunk_pos.insert(i);
        }
    }
    unsigned int chunk_rec_pos = *(get_chunk_pos.begin());
    // std::cout << "chunk_rec_pos = " << chunk_rec_pos << std::endl;
    auto read_acc = ec_eurasure_p2p->chunk_data_map.find(
        GetChunkDataKey(coding_epoch, group_id, chunk_rec_pos));
    std::string chunk_data = read_acc->second;

    uint8_t** ptrs = new uint8_t*[ec_k + ec_m];

    memset(present, false, (ec_k + ec_m) * sizeof(erasure_bool));
    uint8_t* data = new uint8_t[(ec_k + ec_m) * chunk_data.size()];
    memset(data, 0, (ec_k + ec_m) * chunk_data.size() * sizeof(uint8_t));
    generate_ptrs(chunk_data.size(), data, present, ptrs);

    // present[chunk_rec_pos] = true;
    // for (int64_t j = 0; j < chunk_data.size(); j++)
    // {
    //     ptrs[chunk_rec_pos][j] = chunk_rec_data[j];
    // }
    for (int i = 0; i < ec_k + ec_m; i++)
    {
        present[i] = false;
    }

    for (auto& it : get_chunk_pos)

    {
        int seq = it;
        // std::cout << "seq = " << seq << std::endl;
        present[seq] = true;
        std::string fir = GetChunkDataKey(coding_epoch, group_id, it);
        auto data_it = ec_eurasure_p2p->chunk_data_map.find(fir);
        // for (int64_t j = 0; j < chunk_data.size(); j++)
        // {
        // ptrs[seq][j] = data_it->second[j];
        memcpy(ptrs[seq], (uint8_t*)const_cast<char*>(data_it->second.c_str()), chunk_data.size());
    }
    if (replica_chunk.size() > 0)
    {
        for (auto it = replica_chunk.begin(); it != replica_chunk.end(); ++it)
        {
            std::string local_chunk_data = "";
            readChunk(coding_epoch, group_id, *it, local_chunk_data);
            // std::string local_chunk_data = "";
            present[*it] = true;

            memcpy(ptrs[*it], (uint8_t*)const_cast<char*>(local_chunk_data.c_str()),
                chunk_data.size());
        }
    }
    present[chunk_pos] = false;
    // std::cout << "get chunk and prepare decoding time = " << GetTime() -
    // starttime << std::endl;
    start_time = GetTime();

    erasure_encoder_parameters params = {ec_k + ec_m, ec_k, chunk_data.size()};
    erasure_encoder* encoder = erasure_create_encoder(&params, ec_mode);
    erasure_recover(encoder, ptrs, present);
    uint8_t* tmp_data = new uint8_t[chunk_data.size()];
    for (int i = 0; i < chunk_data.size(); i++)
    {
        tmp_data[i] = ptrs[chunk_pos][i];
    }

    std::string strs(tmp_data, tmp_data + chunk_data.size());

    erasure_destroy_encoder(encoder);
    delete present;
    double end_time = GetTime();
    // std::cout << end_time - start_time << std::endl;
    return strs;
}
// std::string Eurasure::readOneChunkFromDB(int block_number, int group_num, int
// pos)
// {
//     std::string db_key = GetChunkDataKey(block_number, group_num, pos);
//     std::string db_value;
//     ec_db->Get(ReadOptions(), db_key, &db_value);
//     if (db_value.length() == 0)
//     {
//         std::cout << "db_key = " << db_key << std::endl;
//         std::cout << "db_value.length() = 0" << std::endl;
//         exit(0);
//     }
//     return db_value;
// }
// void Eurasure::readChunkFromDB(int block_number, int group_num, int pos,
// std::queue<std::string>& q)
// {
//     for (int i = 0; i < group_num; i++)
//     {
//         std::string db_key = GetChunkDataKey(block_number, i, pos);
//         std::string db_value;
//         ec_db->Get(ReadOptions(), db_key, &db_value);
//         if (db_value.length() == 0)
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
//         q.push(db_value);
//     }
// }
void Eurasure::readChunkFromDB(
    int block_number, int group_num, int pos, std::vector<std::string>& v)
{
    for (int i = 0; i < group_num / getK(); ++i)
    {
        std::string db_key = GetChunkDataKey(block_number, i * getK(), pos);
        std::string db_value;
        ec_db->Get(ReadOptions(), db_key, &db_value);
        if (db_value.length() == 0)
        {
            std::cout << "db_key = " << db_key << std::endl;
            std::cout << "db_value.length() = 0" << std::endl;
            exit(0);
        }
        // if (i == 8 && pos == 4)
        // {
        //     std::cout << "readChunkFromDB   hash = " <<
        //     sha256(db_value).hex() << std::endl;
        // }
        v.push_back(db_value);
    }
}
std::vector<std::string> Eurasure::readChunkAndComputeMerkleHashs(
    int block_number, int chunk_pos, int group_id)
{
    std::vector<std::string> v;
    std::vector<std::string> res;

    readChunkFromDB(block_number, group_len, chunk_pos, v);

    int pos = group_id / getK();
    res.push_back(v[group_id / getK()]);
    if (pos % 2 == 0)
    {
        if (pos == v.size() - 1)
            res.push_back("");
        else
            res.push_back(sha256(v[group_id / getK() + 1]).hex());
    }
    else
        res.push_back(sha256(v[group_id / getK() - 1]).hex());

    pos /= 2;

    std::vector<std::string> new_merkle;
    bool flag = true;
    while (new_merkle.size() > 1 || flag)
    {
        if (flag)
        {
            if (v.size() % 2 == 1)
                v.push_back("");
            flag = false;
            vector<string> result;

            for (int i = 0; i < v.size(); i += 2)
            {
                string var1 = sha256(v[i]).hex();
                string var2 = sha256(v[i + 1]).hex();
                string hash = sha256(var1 + var2).hex();
                result.push_back(hash);
            }
            new_merkle = result;
        }
        else
        {
            if (new_merkle.size() % 2 == 1)
                new_merkle.push_back("");

            vector<string> result;

            for (int i = 0; i < new_merkle.size(); i += 2)
            {
                // std::cout << "pos = " <<pos <<  "; i = " << i << std::endl;
                // std::cout << "pos & 1 == 0 && pos == i = " <<(pos & 1 == 0 &&
                // pos == i) << std::endl; std::cout << "pos & 1 == 1 && pos ==
                // i + 1 = " <<(pos & 1 == 1 && pos
                // == i + 1) << std::endl;
                if (pos % 2 == 0 && pos == i)
                {
                    res.push_back(new_merkle[i + 1]);
                }
                else
                {
                    if (pos % 2 == 1 && pos == i + 1)
                    {
                        res.push_back(new_merkle[i]);
                    }
                }

                string var1 = new_merkle[i];
                string var2 = new_merkle[i + 1];
                string hash = sha256(var1 + var2).hex();
                // printf("---readChunkAndComputeMerkleHashs hash(hash(%s),
                // hash(%s)) => %s\n",
                //     new_merkle[i].c_str(), new_merkle[i + 1].c_str(),
                //     hash.c_str());
                result.push_back(hash);
            }
            pos /= 2;
            // std::cout << "pos = " << pos << std::endl;
            new_merkle = result;
        }
    }
    // for (int i = 0; i < res.size(); ++i)
    // {
    //     if (i == 0)
    //         std::cout << "res hash = " << sha256(res[i]).hex() << std::endl;
    //     else
    //         std::cout << "res hash = " << res[i] << std::endl;
    // }
    return res;
}
// std::vector<std::string> Eurasure::readChunkAndComputeMerkleHashs(
//     int block_number, int chunk_pos, int group_id)
// {
//     // whether the chunk data is in my node or not
//     // if true ,read and compute merklehashs and find min datas and
//     merklehashs by which requester
//     // can verify chunk finally ,send them to requester
//     int max_depth = floor(log2(group_len));
//     int group_num = group_len;
//     std::vector<std::pair<std::string, std::string>> vec_key_to_value;
//     std::map<int, std::map<int, std::string>> level_to_chunk;
//     std::queue<std::string> q;
//     readChunkFromDB(block_number, group_num, chunk_pos, q);
//     int level = max_depth;

//     std::vector<std::string> res;

//     while (q.size() > 1)
//     {
//         int q_size = q.size();
//         if (q_size % 2 != 0)
//         {
//             uint8_t tmp[2] = "0";
//             q.push(std::string((const char*)tmp));
//             q_size++;
//         }
//         std::string tmp = "";
//         std::string left, right, values;
//         for (int i = 0; i < q_size; i++)
//         {
//             int depth = max_depth;
//             int c_pos = group_id;
//             if ((depth == level) && ((i >> 1) == (group_id >> 1)))
//             {
//                 // if (group_id == 8 && chunk_pos == 4)
//                 // {
//                 //     std::cout << "readChunkAndComputeMerkleHashs   hash =
//                 "
//                 //               << sha256(q.front()).hex() << std::endl;
//                 // }
//                 int r_or_l = i & 0x01;
//                 if (level_to_chunk.find(level) != level_to_chunk.end())
//                 {
//                     level_to_chunk[level].insert(make_pair(r_or_l,
//                     q.front()));
//                 }
//                 else
//                 {
//                     std::map<int, std::string> s;
//                     s.insert(make_pair(r_or_l, q.front()));
//                     level_to_chunk.insert(make_pair(level, s));
//                 }
//             }
//             else
//             {
//                 bool flag1 = (depth != level);
//                 bool flag2 = (i) != (group_id >> (max_depth - level));
//                 bool flag3 = (i >> 1) == (group_id >> (max_depth - level +
//                 1)); if (flag1 && flag2 && flag3)
//                 {
//                     int r_or_l = i & 0x01;
//                     if (level_to_chunk.find(level) != level_to_chunk.end())
//                     {
//                         level_to_chunk[level].insert(make_pair(r_or_l,
//                         q.front()));
//                     }
//                     else
//                     {
//                         std::map<int, std::string> s;
//                         s.insert(make_pair(r_or_l, q.front()));
//                         level_to_chunk.insert(make_pair(level, s));
//                     }
//                 }
//             }
//             if (level == max_depth)
//                 tmp.append(sha256(q.front()).hex());
//             else
//                 tmp.append(q.front());
//             q.pop();
//             if (i % 2 == 1)
//             {
//                 std::string root_hash = sha256(tmp).hex();
//                 q.push(root_hash);
//                 // std::cout << "readChunkAndComputeMerkleHashs root_hash = "
//                 << root_hash <<
//                 // std::endl;
//                 tmp = "";
//             }
//         }
//         --level;
//     }

//     for (int i = max_depth; i >= 1; --i)
//     {
//         for (auto it = level_to_chunk[i].begin(); it !=
//         level_to_chunk[i].end(); ++it)
//         {
//             std::string str = boost::lexical_cast<string>(i);
//             str.append("-");
//             str.append(boost::lexical_cast<string>(it->first));
//             str.append("-");
//             str.append(it->second);
//             res.push_back(str);
//         }
//     }
//     return res;
//     std::cout << "readChunkAndComputeMerkleHashs successfully!" << std::endl;
// }

// std::string Eurasure::constructMerkleRoot(int pos, int group_num, int
// block_number)
// {
//     std::queue<std::string> q;
//     readChunkFromDB(block_number, group_num, pos, q);
//     int count = 0;

//     while (q.size() > 1)
//     {
//         int q_size = q.size();
//         if (q_size % 2 != 0)
//         {
//             uint8_t tmp[2] = "0";
//             q.push(std::string((const char*)(char*)tmp,
//                 (const char*)(char*)tmp + strlen((const char*)(char*)tmp)));
//             q_size++;
//         }
//         if (count == 0)
//         {
//             for (int i = 0; i < q_size; i = i + 2)
//             {
//                 std::string left_hash = sha256(q.front()).hex();
//                 q.pop();
//                 std::string right_hash = sha256(q.front()).hex();
//                 q.pop();
//                 std::string root_hash = sha256(left_hash +
//                 (right_hash)).hex();
//                 // std::cout << "left_hash = " << left_hash << std::endl;
//                 // std::cout << "right_hash = " << right_hash << std::endl;
//                 // std::cout << "root_hash = " << root_hash << std::endl;
//                 q.push(root_hash);
//             }
//             count++;
//         }
//         else
//         {
//             for (int i = 0; i < q_size; i = i + 2)
//             {
//                 std::string left_hash = q.front();
//                 q.pop();
//                 std::string right_hash = q.front();
//                 q.pop();
//                 std::string root_hash = sha256(left_hash +
//                 (right_hash)).hex(); q.push(root_hash);
//                 // std::cout << "left_hash = " << left_hash << std::endl;
//                 // std::cout << "right_hash = " << right_hash << std::endl;
//                 // std::cout << "root_hash = " << root_hash << std::endl;
//             }
//             count++;
//         }
//     }
//     return std::string(q.front());
// }
std::string Eurasure::constructMerkleRoot(
    int pos, int group_num, int block_number, std::map<int, std::map<int, std::string>>& data)
{
    std::vector<string> v;
    // readChunkFromDB(block_number, group_num, pos, v);
    for (int i = 0; i < group_num; i += ec_k * number_of_vc_in_one_chunk)
    {
        // if (i == 8 && pos == 4)
        // {
        //     std::cout << "readChunkFromDB   hash = " <<
        //     sha256(db_value).hex() << std::endl;
        // }
        v.push_back(data[i][pos]);
    }

    vector<string> new_merkle;
    bool flag = true;
    while (new_merkle.size() > 1 || flag)
    {
        if (flag)
        {
            if (v.size() % 2 == 1)
                v.push_back("");
            flag = false;
            vector<string> result;

            for (int i = 0; i < v.size(); i += 2)
            {
                string var1 = sha256(v[i]).hex();
                string var2 = sha256(v[i + 1]).hex();
                string hash = sha256(var1 + var2).hex();
                // if (block_number == 1)
                //     printf("constructMerkleRoot ---hash(hash(%s), hash(%s))
                //     =>%s\n", var1.c_str(),
                //         var2.c_str(), hash.c_str());
                result.push_back(hash);
            }
            new_merkle = result;
        }
        else
        {
            if (new_merkle.size() % 2 == 1)
                new_merkle.push_back("");

            vector<string> result;

            for (int i = 0; i < new_merkle.size(); i += 2)
            {
                string var1 = new_merkle[i];
                string var2 = new_merkle[i + 1];
                string hash = sha256(var1 + var2).hex();
                // if (block_number == 1)
                //     printf("constructMerkleRoot ---hash(%s, %s) =>%s\n",
                //     var1.c_str(),
                //         var2.c_str(), hash.c_str());
                result.push_back(hash);
            }
            new_merkle = result;
        }
    }
    // std::cout << "constructMerkleRoot = " << new_merkle[0] << std::endl;
    return new_merkle[0];
}
void Eurasure::makeMerkleRoot(int block_number, std::map<int, std::map<int, std::string>>& data)
{
    int group_num = group_len;
    // int data_len = 109;
    // std::cout << "start make merkle root" << std::endl;
    for (int i = 0; i < ec_k + ec_m; i++)
    {
        std::string root = constructMerkleRoot(i, group_num, block_number, data);
        string mid = "-";
        std::string key = DecIntToHexStr((unsigned int)block_number)
                              .append(mid.append(DecIntToHexStr((unsigned int)i)));
        // std::cout << "constructMerkleRoot key = " << key << "     root = " <<
        // root << std::endl;

        ec_db->Put(WriteOptions(), key, root);
    }
    // std::cout << "make merkle root successfully" << std::endl;
}
void Eurasure::convertChunkToMap(std::string chunkdata, std::map<std::string, std::string>& kvmap)
{
    int len = chunkdata.size();
    // std::cout << "convertChunkToMap len = " << len << std::endl;
    for (int i = 0; i < len; i += one_state_with_kv_size)
    {
        // if (i + one_state_with_kv_size >= len)
        //     break;
        // std::cout << "chunkdata.substr(i, 32) = " << chunkdata.substr(i, 32)
        //           << " value = " << chunkdata.substr(i + 32, 4) << std::endl;
        kvmap[chunkdata.substr(i, 32)] = chunkdata.substr(i + 32, 4);
    }
}
bool Eurasure::verifyChunkMerkleRoot(
    std::vector<std::string>& level_to_chunk, int block_number, int chunk_pos, int group_id)
{
    std::string merkleroot;
    string mid = "-";
    std::string left, right, values;
    std::string roots = "";
    std::string key_root = DecIntToHexStr((unsigned int)block_number)
                               .append(mid.append(DecIntToHexStr((unsigned int)chunk_pos)));
    ec_db->Get(ReadOptions(), key_root, &merkleroot);
    // std::cout << "merkleroot =" << merkleroot << std::endl;

    values = level_to_chunk[0];
    // left = sha256(level_to_chunk[0]).hex();
    // right = sha256(level_to_chunk[1]).hex();
    // values = sha256(left.append(right)).hex();
    int pos = group_id / getK();
    bool flag = false;
    for (int i = 1; i < level_to_chunk.size(); ++i)
    {
        if (!flag)
        {
            if (pos % 2 == 1)
            {
                left = level_to_chunk[i];
                right = sha256(values).hex();
                values = sha256(left + (right)).hex();
                // if (block_number == 1)
                //     printf("verifyChunkMerkleRoot ---hash(%s, hash(%s))
                //     =>%s\n", left.c_str(),
                //         right.c_str(), values.c_str());
            }
            else
            {
                left = sha256(values).hex();
                right = level_to_chunk[i];
                values = sha256(left + (right)).hex();
                // if (block_number == 1)
                //     printf("verifyChunkMerkleRoot ---hash(hash(%s), %s)
                //     =>%s\n", left.c_str(),
                //         right.c_str(), values.c_str());
            }
            flag = true;
        }
        else
        {
            if (pos % 2 == 1)
            {
                left = level_to_chunk[i];
                right = values;
                values = sha256(left + (right)).hex();
                // if (block_number == 1)
                //     printf("verifyChunkMerkleRoot ---hash(%s, %s) =>%s\n",
                //     left.c_str(),
                //         right.c_str(), values.c_str());
            }
            else
            {
                left = values;
                right = level_to_chunk[i];
                values = sha256(left + (right)).hex();
                // if (block_number == 1)
                //     printf("verifyChunkMerkleRoot ---hash(%s, %s) =>%s\n",
                //     left.c_str(),
                //         right.c_str(), values.c_str());
            }
        }

        pos /= 2;
    }
    roots = values;

    // std::cout << "verifyChunkMerkleRoot  " << roots << std::endl;
    // assert(roots == merkleroot);
    // std::cout << "verifyChunkMerkleRoot successfully!" << std::endl;
    // return roots == merkleroot ? true : false;
    return true;
}
// bool Eurasure::verifyChunkMerkleRoot(
//     std::map<int, std::map<int, std::string>>& level_to_chunk, int
//     block_number, int chunk_pos)
// {
//     // read merkleroot which the chunk has been contained
//     int max_depth = floor(log2(group_len));
//     int counts = 0;
//     std::string tmp = "";
//     std::string left, right, values;
//     std::string roots = "";
//     std::string merkleroot;
//     string mid = "+";

//     std::string key_root = DecIntToHexStr((unsigned int)block_number)
//                                .append(mid.append(DecIntToHexStr((unsigned
//                                int)chunk_pos)));
//     vc_db->Get(ReadOptions(), key_root, &merkleroot);
//     // std::cout << "merkleroot =" << merkleroot << std::endl;

//     for (int j = max_depth; j >= 1; --j)
//     {
//         if (j == max_depth)
//         {
//             left = sha256(level_to_chunk[j][0]).hex();
//             right = sha256(level_to_chunk[j][1]).hex();
//             values = sha256(left.append(right)).hex();
//             int r_o_l = level_to_chunk.find(j - 1)->second.begin()->first;
//             level_to_chunk[j - 1].insert(make_pair((r_o_l + 1) % 2, values));
//         }
//         else
//         {
//             left = level_to_chunk[j][0];
//             right = level_to_chunk[j][1];
//             values = sha256(left.append(right)).hex();
//             if (j != 1)
//             {
//                 int r_o_l = level_to_chunk.find(j -
//                 1)->second.begin()->first; level_to_chunk[j -
//                 1].insert(make_pair((r_o_l + 1) % 2, values));
//             }
//             else
//                 roots = values;
//         }
//     }
//     // std::cout << "verifyChunkMerkleRoot  " << roots << std::endl;
//     assert(roots == merkleroot);
//     std::cout << "verifyChunkMerkleRoot successfully!" << std::endl;
//     return roots == merkleroot ? true : false;
// }
std::string Eurasure::localReadState(
    unsigned int block_num, unsigned int group_id, unsigned int chunk_pos, std::string key)
{
    double start_time = GetTime();
    std::string db_key = GetChunkDataKey(block_num, group_id, chunk_pos);
    // std::cout << "db_key = " << db_key << std::endl;
    std::string db_value;
    ec_db->Get(ReadOptions(), db_key, &db_value);
    // std::cout << "db value size = " << db_value.size() << std::endl;
    // std::cout << "read chunk from db" << std::endl;
    int chunk_position = ecComputePostion(key, group_len) % number_of_vc_in_one_chunk;
    // std::cout << "chunk_position = " << chunk_position << std::endl;
    std::vector<std::string> s;
    boost::split(s, db_value, boost::is_any_of("!!!"), boost::token_compress_on);
    std::map<std::string, std::string> kvmap;
    // for (int i = 0; i < s.size(); ++i) {
    //     std::cout << "s.size = " << s[i].size() << std::endl;
    // }
    convertChunkToMap(s[chunk_position], kvmap);
    // std::pair<std::string, std::string> data = getKVAndProof(db_value, key);
    // if (checkState(key, data.first, data.second))
    //     return data.first;
    // else
    // std::cout << "before    " << std::endl;

    VCGroup* vcgroup = new VCGroup(block_num);
    // std::cout << "readtime = " << GetTime() - start_time << std::endl;
    return vcgroup->getProof(kvmap, key);
}
std::string Eurasure::remoteReadState(
    unsigned int block_num, std::string key, NodeAddr const& target_nodeid)
{
    // std::cout << "remote read" << std::endl;
    double starttime = GetTime();
    ec_eurasure_p2p->requestState(block_num, key, target_nodeid);
    std::string str = std::to_string(block_num).append(key);
    while (ec_eurasure_p2p->state_map.count(str) == 0)
    {
        sleep(0.0001);
    }
    std::string state_data = ec_eurasure_p2p->state_map[str];
    // std::cout << "state_data = " << state_data << std::endl;
    // std::pair<std::string, std::string> data = getKVAndProof(state_data,
    // key); if (checkState(key, data.first, data.second))
    //     return data.first;
    // else
    return state_data;
}

std::string Eurasure::getState(unsigned int block_num, std::string key)
{
    // std::string strs(49 * 3, 'k');
    if (block_num > complete_coding_epoch)
    {
        // while (block_num >= ec_blockchain->number())
        // {
        //     sleep(0.1);
        // }
        // std::string _key = DecIntToHexStr(block_num);
        // std::string db_key = _key.append(key);
        // std::string db_value;
        // // std::cout << "before_db read key=" << db_key << std::endl;

        // before_ec_db->Get(ReadOptions(), db_key, &db_value);
        // std::cout << "db_value = " << db_value << std::endl;
        return "";
    }
    else
    {
        int group_id;
        int chunk_pos;
        findKeyInWhichChunk(block_num, key, group_id, chunk_pos);
        // std::cout << "group_id = " << group_id << std::endl;
        // std::cout << "chunk_pos = " << chunk_pos << std::endl;
        // int* chunk_set = get_distinct_chunk_set(block_num);
        // std::set<int> state_in_which_nodes;
        // int quotient[2] = {0, 1};

        // for (int i = 0; i < ec_c; i++)
        // {
        //     for (int j = 0; j < 2; j++)
        //     {
        //         int res = (ec_k + ec_m) * quotient[j] + chunk_pos -
        //         chunk_set[i]; if (res < 0)
        //         {
        //             res += (ec_k + ec_m);
        //         }
        //         state_in_which_nodes.insert(res);
        //     }
        // }

        // if (chunk_set[chunk_pos] == ec_position_in_sealers)
        // {
        // std::cout << "local read state" << std::endl;
        std::string out = localReadState(block_num, group_id, chunk_pos, key);
        //     delete chunk_set;
        //     return getKVAndProof(out, key, block_num);
        // }
        // else
        // {
        //     // std::cout << "remote read state   "
        //     //           << ec_sealers[chunk_set[chunk_pos]] << std::endl;
        //     std::string out = remoteReadState(block_num, key, ec_sealers[chunk_set[chunk_pos]]);
        //     delete chunk_set;
        //     return getKVAndProof(out, key, block_num);
        // }
        return out;
    }
}
void Eurasure::readChunk(unsigned int coding_epoch, std::string key, std::string& out)
{
    int group_id;
    int chunk_pos;
    findKeyInWhichChunk(coding_epoch, key, group_id, chunk_pos);
    readChunk(coding_epoch, group_id, chunk_pos, out);
}
void Eurasure::readChunk(
    unsigned int coding_epoch, unsigned group_id, unsigned chunk_pos, std::string& out)
{
    int* chunk_set = get_distinct_chunk_set(coding_epoch);
    int res = chunk_set[chunk_pos];
    if (res < 0)
    {
        res += (ec_k + ec_m);
    }
    // 2021-11-7
    if (ec_nodeid != ec_sealers[res])
    {
        ec_eurasure_p2p->requestChunk(coding_epoch, group_id, chunk_pos, ec_sealers[res]);
        while (ec_eurasure_p2p->chunk_data_map.find(GetChunkDataKey(
                   coding_epoch, group_id, chunk_pos)) == ec_eurasure_p2p->chunk_data_map.end())
            sleep(0.1);
        auto read_acc = ec_eurasure_p2p->chunk_data_map.find(
            GetChunkDataKey(coding_epoch, group_id, chunk_pos));
        out = read_acc->second;
    }
    else
    {
        std::vector<std::string> request_chunk_and_merkle_hash =
            readChunkAndComputeMerkleHashs(coding_epoch, chunk_pos, group_id);
        if (!verifyChunkMerkleRoot(
                request_chunk_and_merkle_hash, coding_epoch, chunk_pos, group_id))
        {
            std::cout << "the chunks is incorrect!" << std::endl;
        }
        int max_depth = floor(log2(getGroupLen()));
        int state_in_chunk_pos = group_id & 0x01;
        out = request_chunk_and_merkle_hash[0];
    }
    // std::string db_key = GetChunkDataKey(coding_epoch, group_id, chunk_pos);
    // Status status = ec_db->Get(ReadOptions(), db_key, &out);
    // assert(status.ok());
}
void Eurasure::getVCCommit(int block_number, int pos, std::string& output) {}
std::string Eurasure::getKVAndProof(std::string& data, std::string const& key, int block_number)
{
    int len = data.length();
    // std::cout << "getKVAndProof len = " << len << std::endl;
    int group_id = ecComputePostion(key, group_len);
    int chunk_pos = ecComputePostion(key, init_size);
    std::string maincommit = "";
    std::string maincommit_proof = "";
    std::string subcommit = "";
    std::string subcommit_proof = "";
    // std::string keys = data.substr(0, len-3*proof_and_commit_len-4);
    // std::string values = data.substr(32, 4);
    subcommit_proof = data.substr(len - 3 * proof_and_commit_len, proof_and_commit_len);
    subcommit = data.substr(len - 2 * proof_and_commit_len, proof_and_commit_len);
    maincommit_proof = data.substr(len - proof_and_commit_len, proof_and_commit_len);
    // std::cout << "maincommit_proof len = " << maincommit_proof.size() <<
    // std::endl; subcommit = data.substr(len - 2 * proof_and_commit_len,
    // proof_and_commit_len); std::cout << "subcommit len = " <<
    // subcommit.size() << std::endl;

    // subcommit_proof = data.substr(len - 3 * proof_and_commit_len,
    // proof_and_commit_len); std::cout << "subcommit_proof len = " <<
    // subcommit_proof.size() << std::endl;

    // std::cout << DecIntToHexStr(block_number) + "_commit" << std::endl;
    Status s = vc_db->Get(ReadOptions(), DecIntToHexStr(block_number) + "_commit", &maincommit);
    assert(s.ok());
    std::string value = "";
    // std::cout << "main commit = " << maincommit << std::endl;
    // std::cout << " getKVAndProof key = " << keys << "        value = " <<
    // values
    //           << std::endl;
    // if (checkState(key, data, maincommit, maincommit_proof, group_id, subcommit, subcommit_proof,
    //         chunk_pos, value))
    std::cout << checkState(key, data, maincommit, maincommit_proof, group_id, subcommit,
                     subcommit_proof, chunk_pos, value)
              << std::endl;
    return data;
    // if (checkState(key, data, maincommit, maincommit_proof, group_id,
    // subcommit,
    //                subcommit_proof, chunk_pos, value)) {
    //     std::cout << "this data is correct." << std::endl;
    // return value;
    // } else {
    //     std::cout << "this data is incorrect. start decode" << std::endl;
    //     // std::string sub_chunk = decode(block_number, key);

    //     // return getKVAndProof(sub_chunk, key, block_number);
    // }

    return "";
}
bool Eurasure::checkState(std::string const& key, std::string const& data,
    std::string const& maincommit, std::string const& maincommit_proof, int main_position,
    std::string const& subcommit, std::string const& subcommit_proof, int sub_position,
    std::string& value)
{
    int kv_len = data.size() - 3 * proof_and_commit_len;
    // std::cout << "kv_len = " << kv_len << std::endl;
    int count = 0;
    // while (count < kv_len) {
    //     if (!strcmp(key.c_str(), data.substr(count, 32).c_str())) {
    //         value = data.substr(count + 32, 4);
    //     }
    //     count += one_state_with_kv_size;
    // }
    // return true;
    // do verify
    if (VCGroup::verify(data.substr(0, kv_len), maincommit, maincommit_proof, main_position,
            subcommit, subcommit_proof, sub_position))
    {
        int count = 0;
        while (count < kv_len)
        {
            if (!strcmp(key.c_str(), data.substr(count, 32).c_str()))
            {
                value = data.substr(count + 32, 4);
            }
            count += one_state_with_kv_size;
        }
        return true;
    }
    else
        return false;
}
inline std::string ecDecToHex(unsigned int num)
{
    std::ostringstream buffer;
    buffer << std::hex << std::uppercase << num;
    return buffer.str();
}
static inline std::string ecstr2fixstr(std::string str)
{
    int len = str.length();
    if (len < 4)
    {
        std::string tmp(4 - len, ' ');
        tmp.append(str);
        return tmp;
    }
    return str.substr(len - 4, 4);
}
void Eurasure::initTestStates(unsigned int block_number)
{
    double starttime = GetTime();
    std::map<int, dev::StringMap> state_storage;
    if (block_number >= 1)
    {
        VCGroup* vcGroup = new VCGroup(block_number);

        std::hash<unsigned int> block_hash;
        unsigned int seed = block_hash(block_number);
        std::mt19937 generator(seed);
        uniform_int_distribution<long long> dist(4000,4000);
        size_t state_size = dist(generator);
        seed = block_hash(block_number);
        std::mt19937 rand_num(seed);
        // std::string proof(49, '9');
        // state_storage[block_number] = dev::StringMap();
        // tmp_states_size += state_size;
        for (size_t i = 0; i < state_size; i++)
        {
            // int key = i;
            std::string key_str;

            if (block_number % 5 == 0)
                key_str =
                    "0a634a3406f274792a18b173f8e7a251f8abaf56d1840bdec54ce15a9d1633061d6fa9a7b4b5d5"
                    "420675bf359046afd61a3bba11d44ad32fc0c3ace6fb416f1d";
            else
                key_str = sha256(boost::lexical_cast<string>(rand_num())).hex();

            int value = 1;
            // int pos = ecComputePostion(key_str.substr(0, 32), getECGroupNum());
            // int sub_pos = ecComputePostion(key_str.substr(0, 32), getInitVCSize());

            if (state_storage.find(block_number) == state_storage.end())
            {
                dev::StringMap key_to_value;


                key_to_value[key_str.substr(0, 32)] = ecstr2fixstr(toString(value));
                state_storage[block_number] = key_to_value;
            }
            else
            {
                if (state_storage[block_number].find(key_str.substr(0, 32)) ==
                    state_storage[block_number].end())
                    state_storage[block_number][key_str.substr(0, 32)] =
                        ecstr2fixstr(toString(value));
                else
                    state_storage[block_number][key_str.substr(0, 32)].append(
                        ecstr2fixstr(toString(value)));
            }
        }
        // int len = state_storage[block_number].size();
        // bf::basic_bloom_filter bf(hashers, max_state_size);
        // for (auto it = state_storage[block_number].begin(); it != state_storage[block_number].end();
        //      it++)
        //     bf.add(it->first.substr(0, 32));
        // std::stringstream ofs;
        // boost::archive::binary_oarchive oa(ofs);
        // oa << bf.storage();
        // getDBHandler()->Put(
        //     rocksdb::WriteOptions(), ecDecToHex(block_number).append("bf"), ofs.str());
        // ofs.str("");
        // ofs.clear();

        vcGroup->commitBlock(state_storage[block_number]);

        makeEC(block_number, 4);
    }

    std::hash<unsigned int> block_hash;
    unsigned int seed = block_hash(block_number);
    std::mt19937 rand_num(seed);
    VCGroup* vcGroup = new VCGroup(block_number);
    for (int i = 0; i < 100; ++i)
    {
        std::string keys = sha256(boost::lexical_cast<string>(rand_num())).hex();
        // std::cout << keys << std::endl;
        double start_time = GetTime();
        std::string out = vcGroup->getProof(state_storage[block_number], keys.substr(0, 32));
        double end_time = GetTime(); 
        std::cout  << end_time- start_time << std::endl;
    }

    
    // //   VCGroup::verify(out.substr(0,36),vcGroup->)
    // getKVAndProof(out, keys, block_number);
}
void Eurasure::mpt_test()
{
    rocksdb::Options options;
    options.create_if_missing = true;

    rocksdb::DB* mpt_db;
    std::string dbpath = "./mptstorage/";
    rocksdb::Status status = rocksdb::DB::Open(options, dbpath, &mpt_db);
    assert(status.ok());


    long count = 40000000;
    double length = pow(count, 1 / (double)16);
    int len = length > (int)length ? (int)length + 1 : (int)length;

    std::hash<unsigned int> block_hash;
    unsigned int seed = block_hash(1);
    std::mt19937 generator(seed);
    std::uniform_int_distribution<long long> dist(2000, 3000);
    size_t state_size = dist(generator);
    seed = block_hash(1);
    std::mt19937 rand_num(seed);
    std::vector<int> v({67, 68, 69, 70, 71, 115, 147, 179, 211, 243, 275, 308, 340, 372, 404, 436,
        468, 500, 532, 564});
    std::vector<int> tmp({20714, 50999, 1176199, 68388, 1, 356385, 148486, 105805, 106669, 114688,
        118541, 114839, 99821, 76988, 52199, 33517, 25559, 28859, 55096, 747650});
    std::vector<std::pair<int, int>> vs;
    for (int i = 0; i < v.size(); ++i)
    {
        vs.push_back(make_pair(tmp[i], v[i]));
    }
    for (int i = 1; i < v.size(); ++i)
    {
        vs[i].first += vs[i - 1].first;
    }
    std::hash<unsigned int> block_hash2;
    unsigned int seeds = block_hash2(2);
    std::mt19937 generators(seeds);
    std::uniform_int_distribution<long long> dists(1, 3501403);

    for (int i = 0; i < state_size; ++i)
    {
        int size = dists(generators);
        for (int j = 0; j < vs.size(); ++j)
        {
            if (size <= vs[j].first)
            {
                size = vs[j].second;
                break;
            }
        }
        mpt_db->Put(rocksdb::WriteOptions(),
            sha256(boost::lexical_cast<string>(rand_num())).hex().substr(32), std::string(size, i));
    }
    mpt_db->Close();

    seed = block_hash(1);
    std::mt19937 rand_nums(seed);
    status = rocksdb::DB::Open(options, dbpath, &mpt_db);
    assert(status.ok());
    double start_time = GetTime();
    std::string value;
    for (int i = 0; i < len; ++i)
    {
        mpt_db->Get(rocksdb::ReadOptions(),
            sha256(boost::lexical_cast<string>(rand_nums())).hex().substr(32), &value);
        value = sha256(value).hex();
    }

    mpt_db->Get(rocksdb::ReadOptions(),
        sha256(boost::lexical_cast<string>(rand_nums())).hex().substr(32), &value);
    double end_time = GetTime();
    std::cout << end_time - start_time << std::endl;
    mpt_db->Close();
}
// void Eurasure::initTestStates(unsigned int block_number)
// {
//     std::hash<unsigned int> block_hash;
//     unsigned int seed = block_hash(block_number);
//     std::mt19937 generator(seed);
//     std::uniform_int_distribution<long long> dist(2000, 3000);
//     size_t state_size = dist(generator);
//     seed = block_hash(block_number);
//     std::mt19937 rand_num(seed);
//     int height = 4;
//     // mpt_node size
//     unsigned int node_seed = block_hash(block_number);
//     std::mt19937 node_generator(node_seed);
//     std::normal_distribution<> dist_normal{(double)400, 0.0};
//     size_t node_size = dist_normal(node_generator);

//     std::vector<std::string> leaf;
//     int size = 0;
//     for (size_t i = 0; i < 4815; i++)
//     {
//         leaf.push_back(sha256(boost::lexical_cast<string>(rand_num())).hex());
//     }
//     std::map<std::string, std::string> m;
//     std::string str = "";
//     std::string value = "";

//     for (int i = 0; i < 4815; i++)
//     {
//         state_storage[block_number].insert(make_pair(leaf[i].substr(0, 32), "1234"));
//         // size += 36;
//     }

//     // std::cout << "size = " << size << std::endl;
//     // tmp_states_size += size;
//     makeEC(block_number, 4);
// }

// void Eurasure::getHasherFromDB(bf::hasher& h)
// {
//     std::string hashers = "";
//     getDBHandler()->Get(rocksdb::ReadOptions(), "bf_hasher", &hashers);
//     std::stringstream ifs(hashers);
//     boost::archive::binary_iarchive ia(ifs);
//     ia >> h;
//     ifs.str("");
//     ifs.clear();
// }

void Eurasure::makeEC(int block_number, int thread_number)
{
    std::map<int, std::map<int, std::string>> position_mapinto_accounts;
    // std::cout << "state_storage[block_number].size() = " <<
    // state_storage[block_number].size()
    //           << std::endl;
    // std::cout << "block number = " << block_number << std::endl;
    int count = 0;
    int state_cnt = state_storage.size();
    for (auto it = state_storage[block_number].begin(); it != state_storage[block_number].end();
         ++it)
    {
        int pos = ecComputePostion(it->first, getECGroupNum());
        // std::cout << "key = " << it->first << "; pos = " << pos << std::endl;
        int sub_pos = ecComputePostion(it->first, getInitVCSize());
        std::string keys = it->first;
        // if(keys.substr(0,32) ==
        // "000d74c9e4c882b18ac739c58473e5ad5871eac3c2278c7e986f233209284e9e")
        // {
        //     std::cout<< "pos = " << pos << " ; sub_pos = " << sub_pos <<
        //     std::endl;
        // }
        if (position_mapinto_accounts.count(pos) != 0)
        {
            if (position_mapinto_accounts[pos].count(sub_pos) != 0)
            {
                position_mapinto_accounts[pos][sub_pos] =
                    position_mapinto_accounts[pos][sub_pos].append(keys.append(it->second));
            }
            else
            {
                position_mapinto_accounts[pos].insert(make_pair(sub_pos, keys.append(it->second)));
            }
        }
        else
        {
            std::map<int, std::string> ms;
            std::string keys = it->first;
            ms.insert(make_pair(sub_pos, keys.append(it->second)));
            position_mapinto_accounts.insert(make_pair(pos, ms));
        }
        ++count;
    }
    // std::cout << "count = " << count << std::endl;
    std::map<int, std::map<int, std::string>> chunks;
    int size = position_mapinto_accounts.size();
    // for (auto it = position_mapinto_accounts.begin(); it !=
    // position_mapinto_accounts.end();
    // ++it)
    // {
    //     std::cout << "group " << it->first << " size = " << it->second.size()
    //     << std::endl;
    // }
    // std::cout << "start ec" << std::endl;
    // std::cout << "size = " << size << std::endl;
    // tbb::parallel_for(tbb::blocked_range<int>(0, size,
    //                       state_erasure->getNumberOfVCInOneChunk() *
    //                       state_erasure->getK()),
    // [&](const tbb::blocked_range<int>& _r) {
    // std::cout<< "start pos = " << _r.begin() << std::endl;
    for (int i = 0; i < size; i += getNumberOfVCInOneChunk() * getK())
    {
        saveChunk(position_mapinto_accounts, block_number, i, chunks);
    }

    // });
    // std::cout << "start make root" << std::endl;
    makeMerkleRoot(block_number, chunks);
    setCompleteCodingEpoch(block_number);
    chunks.clear();

    position_mapinto_accounts.clear();
    std::cout << "finish ec !" << state_cnt << std::endl;
}

bool Eurasure::initVC()
{
    rocksdb::Options options;
    options.create_if_missing = true;
    rocksdb::DB* vc_db;
    rocksdb::Status status = rocksdb::DB::Open(options, "./vcstorage/", &vc_db);
    assert(status.ok());
    VCGroup::initTemplate(vc_db, ec_group_num, init_size);
    bf::basic_bloom_filter bloom_filter(false_positive_rate, max_state_size);
    hashers = bloom_filter.hasher_function();
    std::stringstream ofs;
    boost::archive::binary_oarchive oa(ofs);
    oa << hashers;
    getDBHandler()->Put(rocksdb::WriteOptions(), "bf_hasher", ofs.str());
    ofs.str("");
    ofs.clear();
    setVCDB(vc_db);
    std::cout << "init ec finished" << std::endl;
}