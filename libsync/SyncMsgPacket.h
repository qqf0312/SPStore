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
 * @brief : Sync packet decode and encode
 * @author: jimmyshi
 * @date: 2018-10-18
 */

#pragma once
#include "Common.h"
#include <Common.h>  // namespace ec
#include <libdevcore/RLP.h>
#include <libnetwork/Common.h>
#include <libp2p/P2PMessageFactory.h>
#include <libp2p/Service.h>
namespace dev
{
namespace sync
{
class SyncMsgPacket
{
public:
    using Ptr = std::shared_ptr<SyncMsgPacket>;
    SyncMsgPacket()
    {
        /// TODO:
        /// 1. implement these packet with SyncMsgPacketFactory
        /// 2. modify sync and PBFT related packet from reference to pointer
        if (g_BCOSConfig.version() >= dev::RC2_VERSION)
        {
            m_p2pFactory = std::make_shared<dev::p2p::P2PMessageFactoryRC2>();
        }
        else if (g_BCOSConfig.version() <= dev::RC1_VERSION)
        {
            m_p2pFactory = std::make_shared<dev::p2p::P2PMessageFactory>();
        }
    }
    /// Extract data by decoding the message
    bool decode(
        std::shared_ptr<dev::p2p::P2PSession> _session, std::shared_ptr<dev::p2p::P2PMessage> _msg);

    /// encode is implement in derived class
    /// basic encode function
    RLPStream& prep(RLPStream& _s, unsigned _id, unsigned _args);

    /// Generate p2p message after encode
    std::shared_ptr<dev::p2p::P2PMessage> toMessage(PROTOCOL_ID _protocolId);

    RLP const& rlp() const { return m_rlp; }

public:
    SyncPacketType packetType;
    NodeID nodeId;

protected:
    RLP m_rlp;              /// The result of decode
    RLPStream m_rlpStream;  // The result of encode
    std::shared_ptr<dev::p2p::P2PMessageFactory> m_p2pFactory;

private:
    bool checkPacket(bytesConstRef _msg);
};

class SyncStatusPacket : public SyncMsgPacket
{
public:
    SyncStatusPacket() { packetType = StatusPacket; }
    void encode(int64_t _number, h256 const& _genesisHash, h256 const& _latestHash);
};

class SyncTransactionsPacket : public SyncMsgPacket
{
public:
    SyncTransactionsPacket() { packetType = TransactionsPacket; }
    void encode(std::vector<bytes> const& _txRLPs, bool const& _enableTreeRouter = false,
        uint64_t const& _consIndex = 0);
    void encodeRC2(std::vector<bytes> const& _txRLPs, unsigned const& _fieldSize);
    dev::p2p::P2PMessage::Ptr toMessage(PROTOCOL_ID _protocolId, bool const& _fromRPC = false);
};

class SyncBlocksPacket : public SyncMsgPacket
{
public:
    SyncBlocksPacket() { packetType = BlocksPacket; }
    void encode(std::vector<dev::bytes> const& _blockRLPs);
    void singleEncode(dev::bytes const& _blockRLP);
};
class SyncParamReqPacket : public SyncMsgPacket
{
public:
    SyncParamReqPacket() { packetType = ParamRequestPacket; }
    void encode(dev::bytes const& _blockRLP);
};
class SyncParamResPacket : public SyncMsgPacket
{
public:
    SyncParamResPacket() { packetType = ParamResponsePacket; }
    void encode(dev::bytes const& _blockRLP);
};
class SyncReqBlockPacket : public SyncMsgPacket
{
public:
    SyncReqBlockPacket() { packetType = ReqBlocskPacket; }
    void encode(int64_t _from, unsigned _size);
};



class SyncECRequestStatePacket : public SyncMsgPacket
{
public:
    SyncECRequestStatePacket() { packetType = ECRequestStatePacket; }
    void encode(int64_t const& block_num, std::string key);
};
class SyncECReponseStatePacket : public SyncMsgPacket
{
public:
    SyncECReponseStatePacket() { packetType = ECReponseStatePacket; }
    void encode(int64_t const& block_num, std::string const& key,std::string const& block_data);
};

class SyncECRequestChunkPacket : public SyncMsgPacket
{
public:
    SyncECRequestChunkPacket() { packetType = ECRequestChunkPacket; }
    void encode(unsigned int const& coding_epoch,unsigned int const& group_id, unsigned int const& chunk_pos);
};
class SyncECReponseChunkPacket : public SyncMsgPacket
{
public:
    SyncECReponseChunkPacket() { packetType = ECReponseChunkPacket; }
    void encode(unsigned int const& coding_epoch,unsigned int const& group_id, unsigned int const& chunk_pos,std::vector<std::string> merklelist);
};


class SyncECProofPacket : public SyncMsgPacket
{
public:
    SyncECProofPacket() { packetType = ECProofPacket; }
    void encode(unsigned int const& pos, std::string const& proof);
};

class SyncHeartPacket : public SyncMsgPacket
{
public:
    SyncHeartPacket() { packetType = HeartTest; }
    void encode(std::string const& msg);
};

// transaction status packet
class SyncTxsStatusPacket : public SyncMsgPacket
{
public:
    SyncTxsStatusPacket() { packetType = TxsStatusPacket; }
    void encode(int64_t const& _number, std::shared_ptr<std::set<dev::h256>> _txsHash);
};

// transaction request packet
class SyncTxsReqPacket : public SyncMsgPacket
{
public:
    SyncTxsReqPacket() { packetType = TxsRequestPacekt; }
    void encode(std::shared_ptr<std::vector<dev::h256>> _requestedTxs);
};
class SyncCheckpointPacket : public SyncMsgPacket
{
public:
    SyncCheckpointPacket() { packetType = CheckpointPacket; }
    void encode(dev::bytes const& _blockRLP);
};
}  // namespace sync
}  // namespace dev
