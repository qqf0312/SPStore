#ifndef ECCOMMON_H
#define ECCOMMON_H
#include <libdevcore/Common.h>
#include <libdevcore/FixedHash.h>
#include <set>
struct ECData
{
    byte* data;
    int64_t totalcount;
    int64_t datacount;
    int64_t slides_size;
};
struct BlockInfos
{
    byte* data;
    int64_t block_number;
    int64_t block_len;
};
struct BlockBytesInfo
{
    dev::bytes data;
    int64_t block_num;
    int64_t first_block_num;
    int64_t seq;
    bool operator<(const struct BlockBytesInfo & right)const   //重载<运算符
    {
        if( this->seq == right.seq)     
            return false;
        else
        {
            return this->seq < right.seq;     
        }
    }
};
struct BlockByteInfo
{
    byte* data;
    int64_t block_num;
    int64_t first_block_num;
    int64_t datalen;
    int64_t seq;
    bool operator<(const struct BlockByteInfo & right)const   //重载<运算符
    {
        if(this->first_block_num == right.first_block_num)     //根据id去重
            return false;
        else
        {
            return this->first_block_num > right.first_block_num;     
        }
    }
};
struct BlockPartitionRange
{
    int64_t start_block_num;
    int64_t end_block_num;
    bool operator== (const BlockPartitionRange& p) const  
    {  
        return start_block_num==p.start_block_num && end_block_num==p.end_block_num;  
    }  
};
struct BlockPartitionIndex
{
    double redundancy;
    double fault_tolerance;
    int64_t totalnum;
    int64_t datanum;
    int64_t data_len;
    int64_t sealers_num;
    int64_t full_sealer_num;
    int64_t blocks_info_num;
    std::set<int> delete_seq;
    dev::h512 sealers[10];
    dev::h512 full_sealers[10];
    BlockByteInfo blocks_data_info[10];
    // dev::h512s sealers;
    // dev::h512s full_sealers;
    // std::vector<BlockByteInfo> blocks_data_info;
};
struct Cmp
{
    bool operator()(BlockPartitionRange const& a, BlockPartitionRange const& b) const
    {
        if (a.start_block_num != b.start_block_num)
            return a.start_block_num < b.start_block_num;
        else
            return a.end_block_num < b.end_block_num;
    }
};

#endif
