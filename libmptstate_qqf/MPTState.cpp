/*
    @CopyRight:
    This file is part of FISCO-BCOS.

    FISCO-BCOS is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    FISCO-BCOS is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with FISCO-BCOS.  If not, see <http://www.gnu.org/licenses/>.
*/
/**
 * @Mpt state interface for EVM
 *
 * @file MPTState.cpp
 * @author jimmyshi
 * @date 2018-09-21
 */

#include "MPTState.h"
#include <string>
#include <unordered_map>

#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/serialization/vector.hpp>

#include <libinitializer/Initializer.h>
#include <libinitializer/P2PInitializer.h>
#include <libinitializer/SecureInitializer.h>

#include "VCGroup.h"
// write state format :<block_number + "+" + key, value>
// write chunk format :<block_number + "|" + group_id + '|' + chunk_pos, chunk data>
// write chunk merkleroot format : <block_number + "-" + column_number, root>
// write vccommit format: {
//      maincommit : block_number + "_commit"
//      subcommit  : block_number + "_subcommit" + subcommit_pos
//}
// write bloom filter bitvector format : <block_number+"bf",bitvector>
// write bloom filter hasher format : <"bf_hasher", hasher>

ec::Eurasure* dev::mptstate::MPTState::state_erasure = NULL;
static std::string DecIntToHexStr(dev::u256 const& num)
{
    std::string str;
    dev::u256 Temp = num / 16;
    dev::u256 left = num % 16;
    if (Temp > 0)
        str.append(DecIntToHexStr(Temp));
    if (left < 10)
        str += ((char)left + '0');
    else
        str += ('A' + (char)left - 10);
    return str;
}

static inline std::string str2fixstr(std::string str)
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
static inline std::string num2str(int i)
{
    char ss[10];
    sprintf(ss, "%04d", i);
    return ss;
}
// static inline std::string DecToHex(unsigned int num)
// {
//     std::ostringstream buffer;
//     buffer << std::hex << std::uppercase << num;
//     return buffer.str();
// }
static inline int computePostion(const std::string& posStr, int mod)
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
namespace dev
{
namespace mptstate
{
void MPTState::makeEC(int block_number, int thread_number)
{
    std::map<int, std::map<int, std::string>> position_mapinto_accounts;
    // std::cout << "state_storage[block_number].size() = " << state_storage[block_number].size()
    //           << std::endl;
    int count = 0;
    for (auto it = state_storage[block_number].begin(); it != state_storage[block_number].end();
         ++it)
    {
        int pos = computePostion(it->first, state_erasure->getECGroupNum());
        // std::cout << "pos = " << pos << std::endl;
        int sub_pos = computePostion(it->first, state_erasure->getInitVCSize());
        std::string keys = it->first;
        // std::cout << "it->first = " << it->first << std::endl;
        // if (it->first == "E0C194580A962A5A5914EF13D81811AF")
        // {
        //     // std::string values = num2str(stoi(it->second));
        //     std::cout << "pos = " << pos << std::endl;
        //     std::cout << "sub pos = " << sub_pos << std::endl;
        //     std::cout << "key = " << keys << " value = " << num2str(stoi(it->second))<<
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
    // for (auto it = position_mapinto_accounts.begin(); it != position_mapinto_accounts.end();
    // ++it)
    // {
    //     std::cout << "group " << it->first << " size = " << it->second.size() << std::endl;
    // }
    // std::cout << "start ec" << std::endl;
    // std::cout << "size = " << size << std::endl;
    // tbb::parallel_for(tbb::blocked_range<int>(0, size,
    //                       state_erasure->getNumberOfVCInOneChunk() * state_erasure->getK()),
    // [&](const tbb::blocked_range<int>& _r) {
    // std::cout<< "start pos = " << _r.begin() << std::endl;
    for (int i = 0; i < size; i += state_erasure->getNumberOfVCInOneChunk() * state_erasure->getK())
    {
        state_erasure->saveChunk(position_mapinto_accounts, block_number, i, chunks);
    }

    // });
    // std::cout << "start make root" << std::endl;
    state_erasure->makeMerkleRoot(block_number, chunks);
    state_erasure->setCompleteCodingEpoch(block_number);
    chunks.clear();
    position_mapinto_accounts.clear();
    std::cout << "finish ec " << std::endl;
}

OverlayDB MPTState::openDB(
    boost::filesystem::path const& _path, h256 const& _genesisHash, WithExisting _we)
{
    // initEC();

    std::cout << "MPTState::openDB init success, path:" << _path << std::endl;
    return State::openDB(_path, _genesisHash, _we);
}

bool MPTState::addressInUse(Address const& _address) const
{
    return m_state.addressInUse(_address);
}

bool MPTState::accountNonemptyAndExisting(Address const& _address) const
{
    return m_state.accountNonemptyAndExisting(_address);
}

bool MPTState::addressHasCode(Address const& _address) const
{
    std::cout<<"MPTState::addressHasCode"<<std::endl;
    return m_state.addressHasCode(_address);
}

u256 MPTState::balance(Address const& _id) const
{
    return m_state.balance(_id);
}

void MPTState::addBalance(Address const& _id, u256 const& _amount)
{
    m_state.addBalance(_id, _amount);
}

void MPTState::subBalance(Address const& _addr, u256 const& _value)
{
    m_state.subBalance(_addr, _value);
}

void MPTState::setBalance(Address const& _addr, u256 const& _value)
{
    m_state.setBalance(_addr, _value);
}

void MPTState::transferBalance(Address const& _from, Address const& _to, u256 const& _value)
{
    m_state.transferBalance(_from, _to, _value);
}

h256 MPTState::storageRoot(Address const& _contract) const
{
    return m_state.storageRoot(_contract);
}

u256 MPTState::storage(Address const& _contract, u256 const& _memory)
{
    std::cout<<"MPTState::Storage"<<std::endl;
    state_storage[block_number][DecIntToHexStr(_memory).substr(0, 32)];
    return m_state.storage(_contract, _memory);
}

void MPTState::setStorage(Address const& _contract, u256 const& _location, u256 const& _value)
{
    std::cout<<"MPTState::setStorage"<<std::endl;
    // std::cout<< toString(_value).size() << "   "<< str2fixstr(toString(_value)).size()<<
    // std::endl;
    // std::cout << "toHex(toString(_location)).substr(0, 32) = " <<
    // toHex(toString(_location)).substr(0, 32) << std::endl;
    std::hash<unsigned int> block_hash;
    unsigned int seed = block_hash(state_erasure->getRandCount());
    std::mt19937 rand_num(seed);
    std::string key = sha256(boost::lexical_cast<std::string>(rand_num())).hex();
    if (state_storage.find(block_number) == state_storage.end())
    {
        dev::StringMap key_to_value;

        // key_to_value[DecIntToHexStr(_location).substr(0, 32)] = str2fixstr(toString(_value));
        // key_to_value[DecIntToHexStr(_location).substr(0, 32)] =
        //     std::to_string(1);
        
        key_to_value[key.substr(0, 32)] = str2fixstr(toString(_value));
        state_storage[block_number] = key_to_value;
    }
    else
    {
        // state_storage[block_number][DecIntToHexStr(_location).substr(0, 32)] =
        //     str2fixstr(toString(_value));
        state_storage[block_number][key.substr(0, 32)] = std::to_string(1);
    }
    m_state.setStorage(_contract, _location, _value);
}

void MPTState::clearStorage(Address const& _contract)
{
    m_state.clearStorage(_contract);
}

void MPTState::createContract(Address const& _address)
{
    std::cout<<"MPTState::createContract"<<std::endl;   
    m_state.createContract(_address);
}

void MPTState::setCode(Address const& _address, bytes&& _code)
{
    m_state.setCode(_address, std::move(_code));
}

void MPTState::kill(Address _a)
{
    m_state.kill(_a);
}

bytes const MPTState::code(Address const& _addr) const
{
    return m_state.code(_addr);
}

h256 MPTState::codeHash(Address const& _contract) const
{
    return m_state.codeHash(_contract);
}

bool MPTState::frozen(Address const& _contract) const
{
    (void)_contract;
    return false;
}

size_t MPTState::codeSize(Address const& _contract) const
{
    return m_state.codeSize(_contract);
}

void MPTState::incNonce(Address const& _id)
{
    m_state.incNonce(_id);
}

void MPTState::setNonce(Address const& _addr, u256 const& _newNonce)
{
    m_state.setNonce(_addr, _newNonce);
}

u256 MPTState::getNonce(Address const& _addr) const
{
    return m_state.getNonce(_addr);
}

h256 MPTState::rootHash(bool) const
{
    return m_state.rootHash();
}

void MPTState::commit()
{
    m_state.commit();
}
void MPTState::getHasherFromDB(bf::hasher& h)
{
    std::string hashers = "";
    state_erasure->getDBHandler()->Get(rocksdb::ReadOptions(), "bf_hasher", &hashers);
    std::stringstream ifs(hashers);
    boost::archive::binary_iarchive ia(ifs);
    ia >> h;
    ifs.str("");
    ifs.clear();
}
bool MPTState::initVC()
{
    rocksdb::Options options;
    options.create_if_missing = true;
    rocksdb::DB* vc_db;
    rocksdb::Status status = rocksdb::DB::Open(options, "./vcstorage/", &vc_db);
    assert(status.ok());
    VCGroup::initTemplate(vc_db, subcommit_num, init_vc_size);
    bf::basic_bloom_filter bloom_filter(false_positive_rate, max_state_size);
    hashers = bloom_filter.hasher_function();
    std::stringstream ofs;
    boost::archive::binary_oarchive oa(ofs);
    oa << hashers;
    state_erasure->getDBHandler()->Put(rocksdb::WriteOptions(), "bf_hasher", ofs.str());
    ofs.str("");
    ofs.clear();
    state_erasure->setVCDB(vc_db);
    // std::cout << "init ec finished" << std::endl;
}
void MPTState::dbCommit(h256 const&, int64_t)
{
    std::cout << "MPTState::dbCommit"
               << "   block number = " << block_number 
               << "   state_storage ="<< state_storage[block_number].size() <<std::endl;
    // if (block_number >= 3)
    // {
    //     VCGroup* vcGroup = new VCGroup(block_number);
    //     if (hashers.get_function_num() == 0)
    //     {
    //         getHasherFromDB(hashers);
    //     }
    //     bf::basic_bloom_filter bf(hashers, max_state_size);
    //     for (auto it = state_storage[block_number].begin(); it != state_storage[block_number].end();
    //          it++)
    //         bf.add(it->first.substr(0, 32));
    //     std::stringstream ofs;
    //     boost::archive::binary_oarchive oa(ofs);
    //     oa << bf.storage();
    //     // std::cout << "bf bitvector " << DecToHex(block_number).append("bf") << std::endl;
    //     // std::cout << "write bf_bit_vector = " <<  ofs.str() << std::endl;
    //     state_erasure->getDBHandler()->Put(
    //         rocksdb::WriteOptions(), DecToHex(block_number).append("bf"), ofs.str());
    //     ofs.str("");
    //     ofs.clear();
    //     vcGroup->commitBlock(state_storage[block_number]);
    //     // std::cout << "vcGroup->commitBlock" << std::endl;
    //     // makeEC(block_number, 4);
    //     // std::cout << "makeEC" << std::endl;
    //     // tbb::parallel_for(tbb::blocked_range<int>(0, 1),
    //     //     [&](const tbb::blocked_range<int>& _r) { makeEC(block_number, 4); });
    // }

    ++block_number;
    // m_state.db().commit();
    // state_erasure->accRandCount();
    state_storage[block_number].clear();
}

void MPTState::setRoot(h256 const& _root)
{
    m_state.setRoot(_root);
}

u256 const& MPTState::accountStartNonce() const
{
    return m_state.accountStartNonce();
}

u256 const& MPTState::requireAccountStartNonce() const
{
    return m_state.requireAccountStartNonce();
}

void MPTState::noteAccountStartNonce(u256 const& _actual)
{
    m_state.noteAccountStartNonce(_actual);
}

size_t MPTState::savepoint() const
{
    return m_state.savepoint();
}

void MPTState::rollback(size_t _savepoint)
{
    m_state.rollback(_savepoint);
}

void MPTState::clear()
{
    m_state.cacheClear();
}

bool MPTState::checkAuthority(Address const&, Address const&) const
{
    return true;
}

State& MPTState::getState()
{
    return m_state;
}

MPTState& MPTState::getMPTState()
{
    return (*this);
}


}  // namespace mptstate
}  // namespace dev
