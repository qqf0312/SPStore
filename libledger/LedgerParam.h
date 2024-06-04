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

/**
 * @brief : concrete implementation of LedgerParamInterface
 * @file : LedgerParam.h
 * @author: yujiechen
 * @date: 2018-10-23
 */
#pragma once
#include "LedgerParamInterface.h"
#include "libblockchain/BlockChainInterface.h"
#include "libethcore/Protocol.h"
#include <libdevcore/FixedHash.h>
#include <boost/property_tree/ptree.hpp>
#include <memory>
#include <vector>

#define LedgerParam_LOG(LEVEL) \
    LOG(LEVEL) << "[g:" << std::to_string(groupId()) << "]" << LOG_BADGE("LedgerParam")

namespace dev
{
namespace ledger
{
/// forward class declaration
#define SYNC_TX_POOL_SIZE_DEFAULT 102400
#define MAX_BLOCK_RANGE_EVENT_FILTER (0)
struct TxPoolParam
{
    int64_t txPoolLimit = SYNC_TX_POOL_SIZE_DEFAULT;
};
struct ConsensusParam
{
    std::string consensusType;
    dev::h512s sealerList = dev::h512s();
    dev::h512s observerList = dev::h512s();
    int64_t maxTransactions;
    int8_t maxTTL;
    // limit ttl to 5
    int8_t ttlLimit = 5;
    /// the minimum block time
    signed minBlockGenTime = 0;
    /// unsigned intervalBlockTime;
    int64_t minElectTime;
    int64_t maxElectTime;
    /// enable dynamic block size or not
    bool enableDynamicBlockSize = true;
    /// block size increase ratio
    float blockSizeIncreaseRatio = 0.5;

    // sealers size for each RPBFT epoch, default is 10
    int64_t epochSealerNum = 10;
    // block num for each epoch, default is 10
    int64_t epochBlockNum = 10;
    // enable optimize ttl or not
    bool enableTTLOptimize;
    bool enablePrepareWithTxsHash;

    bool broadcastPrepareByTree;
    unsigned treeWidth = 3;
    unsigned prepareStatusBroadcastPercent;
    int64_t maxRequestMissedTxsWaitTime;
    int64_t maxRequestPrepareWaitTime;
};

struct AMDBParam
{
    std::string topic;
    int retryInterval = 1;
    int maxRetry = 0;
};

#define SYNC_IDLE_WAIT_DEFAULT 200
struct SyncParam
{
    /// TODO: syncParam related
    signed idleWaitMs = SYNC_IDLE_WAIT_DEFAULT;
    // enable send transactions by tree
    bool enableSendTxsByTree = false;
    // enable send block status by tree or not
    bool enableSendBlockStatusByTree = true;
    // default block status gossip interval is 1s
    int64_t gossipInterval = 1000;
    // default gossip peers is 3
    int64_t gossipPeers = 3;
    // default syncTreeWidth is 3
    int64_t syncTreeWidth = 3;
    // max queue size for block sync (default is 512 MB)
    int64_t maxQueueSizeForBlockSync = 512 * 1024 * 1024;
    // limit the peers number the txs-status gossip to
    signed txsStatusGossipMaxPeers = 5;
};

/// modification 2019.03.20: add timeStamp field to GenesisParam
struct GenesisParam
{
    std::string nodeListMark;
    uint64_t timeStamp;
};

struct EventLogFilterManagerParams
{
    int64_t maxBlockRange;
    int64_t maxBlockPerProcess;
};
struct StorageParam
{
    std::string type = "storage";
    std::string path = "data/";
    bool binaryLog = false;
    bool CachedStorage = true;
    // for amop storage
    std::string topic;
    size_t timeout;
    int maxRetry;
    // MB
    int64_t maxCapacity;

    int64_t scrollThreshold = 2000;
    // for zdb storage
    std::string dbType;
    std::string dbIP;
    uint32_t dbPort;
    std::string dbUsername;
    std::string dbPasswd;
    std::string dbName;
    std::string dbCharset;
    uint32_t initConnections;
    uint32_t maxConnections;
    int maxForwardBlock;
};
struct StateParam
{
    std::string type;
};
struct TxParam
{
    int64_t txGasLimit;
    bool enableParallel = false;
};
class LedgerParam : public LedgerParamInterface
{
public:
    TxPoolParam& mutableTxPoolParam() override { return m_txPoolParam; }
    ConsensusParam& mutableConsensusParam() override { return m_consensusParam; }
    SyncParam& mutableSyncParam() override { return m_syncParam; }
    GenesisParam& mutableGenesisParam() override { return m_genesisParam; }
    AMDBParam& mutableAMDBParam() override { return m_amdbParam; }
    std::string const& baseDir() const override { return m_baseDir; }
    void setBaseDir(std::string const& baseDir) override { m_baseDir = baseDir; }
    StorageParam& mutableStorageParam() override { return m_storageParam; }
    StateParam& mutableStateParam() override { return m_stateParam; }
    TxParam& mutableTxParam() override { return m_txParam; }
    EventLogFilterManagerParams& mutableEventLogFilterManagerParams() override
    {
        return m_eventLogFilterParams;
    }
    blockchain::GenesisBlockParam& mutableGenesisBlockParam() override
    {
        return m_genesisBlockParam;
    }
    void parseGenesisConfig(const std::string& _genesisFile);
    void parseIniConfig(const std::string& _iniFile, const std::string& _dataPath = "data/");
    void init(const std::string& _configFilePath, const std::string& _dataPath = "data/");
    const dev::GROUP_ID& groupId() const { return m_groupID; }

    blockchain::GenesisBlockParam generateGenesisMark();

private:
    void initStorageConfig(boost::property_tree::ptree const& pt);
    void initTxPoolConfig(boost::property_tree::ptree const& pt);
    void initTxExecuteConfig(boost::property_tree::ptree const& pt);
    void initConsensusConfig(boost::property_tree::ptree const& pt);
    void initConsensusIniConfig(boost::property_tree::ptree const& pt);
    void initRPBFTConsensusIniConfig(boost::property_tree::ptree const& pt);
    void initSyncConfig(boost::property_tree::ptree const& pt);
    void initEventLogFilterManagerConfig(boost::property_tree::ptree const& pt);

private:
    dev::GROUP_ID m_groupID;
    TxPoolParam m_txPoolParam;
    ConsensusParam m_consensusParam;
    SyncParam m_syncParam;
    GenesisParam m_genesisParam;
    AMDBParam m_amdbParam;
    std::string m_baseDir;
    StorageParam m_storageParam;
    StateParam m_stateParam;
    TxParam m_txParam;
    EventLogFilterManagerParams m_eventLogFilterParams;
    dev::blockchain::GenesisBlockParam m_genesisBlockParam;

private:
    std::string uriEncode(const std::string& keyWord);
};
}  // namespace ledger
}  // namespace dev
