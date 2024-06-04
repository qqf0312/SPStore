#include "Eurasure-P2P.h"

#include "Eurasure.h"
using namespace dev;
using namespace dev::sync;
using namespace dev::p2p;
using namespace ec;
void EurasureP2P::setEurasure(ec::Eurasure* eurasure)
{
    m_eurasure = eurasure;
}

void EurasureP2P::registerHandler()
{
    m_service->registerHandlerByProtoclID(
        m_protocolId, boost::bind(&EurasureP2P::messageHandler, this, _1, _2, _3));
}
void EurasureP2P::removeHandler()
{
    if (m_service)
    {
        m_service->removeHandlerByProtocolID(m_protocolId);
    }
}
void EurasureP2P::processPacket(SyncMsgPacket::Ptr packet, NodeAddr const& destnodeId)
{
    switch (packet->packetType)
    {
    case ECRequestChunkPacket: {
        // std::cout << "get RequestChunk Packet" << std::endl;

        RLP const& rlps = (*packet).rlp();
        unsigned int coding_epoch = rlps[0].toInt();
        unsigned int group_id = rlps[1].toInt();
        unsigned int chunk_pos = rlps[2].toInt();
        // std::cout << "ECRequestChunkPacket coding_epoch = " << coding_epoch
        //           << "group id = " << group_id << "   chunk_pos = " << chunk_pos
        //           << "   destnodeId = " << destnodeId << std::endl;
        while (m_eurasure->getCompleteEpoch() < coding_epoch)
            sleep(0.1);
        std::vector<std::string> request_chunk_and_merkle_hash =
            m_eurasure->readChunkAndComputeMerkleHashs(coding_epoch, chunk_pos, group_id);
        // if (chunk_pos == 4)
        // {
        //     // for (int i = 0; i < request_chunk_and_merkle_hash.size(); ++i)
        //     // {
        //     //     std::cout << "ECRequestChunkPacket hash = "
        //     //               << sha256(request_chunk_and_merkle_hash[i]).hex() << std::endl;
        //     // }
        // }
        // for (int i = 0; i < request_chunk_and_merkle_hash.size(); ++i)
        // {
        //     std::cout << "request chunk hash = " << request_chunk_and_merkle_hash[i] <<
        //     std::endl;
        // }
        // std::cout << "after ECRequestChunkPacket" << std::endl;

        responseChunk(coding_epoch, group_id, chunk_pos, request_chunk_and_merkle_hash, destnodeId);
    };
    break;

    case ECReponseChunkPacket: {
        // std::cout << "get ReponseChunk Packet" << destnodeId << std::endl;

        RLP const& rlps = (*packet).rlp();
        unsigned int coding_epoch = rlps[0].toInt();
        unsigned int group_id = rlps[1].toInt();
        unsigned int chunk_pos = rlps[2].toInt();
        // std::cout << "ECReponseChunkPacket coding_epoch =" << coding_epoch
        //           << "    group_id = " << group_id << "     chunk_pos = " << chunk_pos << std::endl;
        std::vector<std::string> merkle_list = rlps[3].toVector<std::string>();

        // if (group_id == 11 && chunk_pos == 0)
        // {
        if (!m_eurasure->verifyChunkMerkleRoot(merkle_list, coding_epoch, chunk_pos, group_id))
        {
            std::cout << "the chunks is incorrect!" << std::endl;
            return;
        }


        tbb::concurrent_unordered_map<std::string, unsigned int>::iterator pre_ac;
        std::string str = std::to_string(coding_epoch);
        str.append("-");
        str.append(std::to_string(group_id));
        pre_ac = chunk_count_map.find(str);
        if (pre_ac != chunk_count_map.end())
        {
            chunk_count_map[str] += 1;
        }
        else
        {
            chunk_count_map[str] = 1;
        }
        chunk_data_map.emplace(
            m_eurasure->GetChunkDataKey(coding_epoch, group_id, chunk_pos), merkle_list[0]);
        // }


        // tbb::concurrent_hash_map<unsigned int, std::string>::accessor next_ac;

        // next_ac->second = chunk_data;
        // auto map_it = chunk_map[coding_epoch].insert();
        // map_it[chunk_pos] = chunk_data;
        // chunk_map[coding_epoch] = map_it;
    };
    break;

    case ECProofPacket: {
        RLP const& rlps = (*packet).rlp();
        unsigned int pos = rlps[0].toInt();
        std::string proofs = asString(rlps[1].toBytes());
        // std::cout << "get proofs "
        //           << "pos = " << pos << "   proofs = " << proofs << std::endl;
    };
    break;
    case ECRequestStatePacket: {
        RLP const& rlps = (*packet).rlp();
        unsigned int block_num = rlps[0].toInt();
        std::string key = asString(rlps[1].toBytes());
        std::string data;
        // std::cout << "ECRequestStatePacket key = " << key << "  in block " << block_num << std::endl;
        if (state_map.count(std::to_string(block_num).append(key)) != 0)
        {
            data = state_map[std::to_string(block_num).append(key)];
        }
        else
        {
            data = m_eurasure->getState(block_num, key);
        }

        // std::cout << "ECRequestStatePacket destnodeId = " << destnodeId << std::endl;
        responseState(block_num, key, data, destnodeId);
    };
    break;
    case ECReponseStatePacket: {
        RLP const& rlps = (*packet).rlp();
        unsigned int block_num = rlps[0].toInt();
        std::string key = asString(rlps[1].toBytes());
        std::string data = asString(rlps[2].toBytes());
        // std::cout << "ECReponseStatePacket key = " << key << "  in block " << block_num << std::endl;
        state_map[std::to_string(block_num).append(key)] = data;
    };
    break;
    case HeartTest:{
        RLP const& rlps = (*packet).rlp();
        std::string heart = asString(rlps[0].toBytes());
        // std::cout << "heart test = " << heart  << std::endl;
    };break;
    default:
        std::cout << "no right packettype !" << std::endl;
        break;
    }
}
void  EurasureP2P::sendHeart(std::string const& heart, NodeAddr const& destnodeId)
{
     dev::sync::SyncHeartPacket retPacket;
        retPacket.encode(heart);
        auto msg = retPacket.toMessage(m_protocolId);
        m_service->asyncSendMessageByNodeID(
            destnodeId, msg, CallbackFuncWithSession(), dev::network::Options());

// std::cout << "sendHeart = " << heart  << std::endl;
}
void EurasureP2P::messageHandler(dev::p2p::NetworkException _e,
    std::shared_ptr<dev::p2p::P2PSession> _session, dev::p2p::P2PMessage::Ptr _msg)
{
    SyncMsgPacket::Ptr packet = std::make_shared<SyncMsgPacket>();
    // std::cout<< "receive packet from" << _session->nodeID().abridged() << "type = " <<packet->packetType <<   std::endl;
    if (!packet->decode(_session, _msg))
    {
        SYNC_ENGINE_LOG(WARNING) << LOG_BADGE("Rcv") << LOG_BADGE("Packet")
                                 << LOG_DESC("Reject packet") << LOG_KV("reason", "decode failed")
                                 << LOG_KV("nodeId", _session->nodeID().abridged())
                                 << LOG_KV("size", _msg->buffer()->size())
                                 << LOG_KV("message", toHex(*_msg->buffer()));
        return;
    }
    std::thread work_thread(
        boost::bind(&EurasureP2P::processPacket, this, packet, _session->nodeID()));
    work_thread.join();
}

void EurasureP2P::sendStateMessage(uint8_t message_type, unsigned int block_num,
    std::string const& key, std::string const& data, dev::network::NodeID const& dest_node_id)
{
    // std::cout << dest_node_id << std::endl;
    dev::sync::SyncPacketType packettype;
    if (message_type == 0)
        packettype = ECRequestStatePacket;
    else
        packettype = ECReponseStatePacket;

    switch (packettype)
    {
    case ECRequestStatePacket: {
        dev::sync::SyncECRequestStatePacket retPacket;
        retPacket.encode(block_num, key);
        auto msg = retPacket.toMessage(m_protocolId);
        m_service->asyncSendMessageByNodeID(
            dest_node_id, msg, CallbackFuncWithSession(), dev::network::Options());
        
    };break;

    case ECReponseStatePacket: {
        // std::cout << "send ECReponseChunkPacket" << std::endl;

        dev::sync::SyncECReponseStatePacket retPacket;
        retPacket.encode(block_num, key, data);
        auto msg = retPacket.toMessage(m_protocolId);
        m_service->asyncSendMessageByNodeID(
            dest_node_id, msg, CallbackFuncWithSession(), dev::network::Options());
        
    };break;
    default: {
        // std::cout << "unknown message!" << std::endl;
        
    }break;
    }
}

void EurasureP2P::sendChunkMessage(uint8_t message_type, unsigned int coding_epoch,
    unsigned int group_id, unsigned int chunk_pos, std::vector<std::string> const& merklelist,
    dev::network::NodeID const& dest_node_id)
{
    // std::cout << dest_node_id << std::endl;
    dev::sync::SyncPacketType packettype;
    if (message_type == 0)
        packettype = ECRequestChunkPacket;
    else
        packettype = ECReponseChunkPacket;

    switch (packettype)
    {
    case ECRequestChunkPacket: {
        // std::cout << "send ECRequestChunkPacket" << std::endl;

        dev::sync::SyncECRequestChunkPacket retPacket;
        retPacket.encode(coding_epoch, group_id, chunk_pos);
        auto msg = retPacket.toMessage(m_protocolId);
        m_service->asyncSendMessageByNodeID(
            dest_node_id, msg, CallbackFuncWithSession(), dev::network::Options());
       
    }; break;

    case ECReponseChunkPacket: {
        // std::cout << "send ECReponseChunkPacket" << std::endl;

        dev::sync::SyncECReponseChunkPacket retPacket;
        retPacket.encode(coding_epoch, group_id, chunk_pos, merklelist);
        auto msg = retPacket.toMessage(m_protocolId);
        m_service->asyncSendMessageByNodeID(
            dest_node_id, msg, CallbackFuncWithSession(), dev::network::Options());
       
    }; break;
    default: {
        // std::cout << "unknown message!" << std::endl;
        
    };break;
    }
}
void EurasureP2P::sendProofMessage(
    unsigned int pos, std::string proofs, dev::network::NodeID const& destnodeId)
{
    dev::sync::SyncPacketType packettype;
    packettype = ECProofPacket;

    dev::sync::SyncECProofPacket retPacket;
    retPacket.encode(pos, proofs);
    auto msg = retPacket.toMessage(m_protocolId);
    m_service->asyncSendMessageByNodeID(
        destnodeId, msg, CallbackFuncWithSession(), dev::network::Options());
    // std::cout << "send proof !!!" << std::endl;
}
void EurasureP2P::requestChunk(unsigned int coding_epoch, unsigned int group_id,
    unsigned int chunk_pos, dev::network::NodeID const& destnodeId)
{
    sendChunkMessage(REQUESTCHUNKMESSAGETYPE, coding_epoch, group_id, chunk_pos,
        std::vector<std::string>(), destnodeId);
}
void EurasureP2P::responseChunk(unsigned int coding_epoch, unsigned int group_id,
    unsigned int chunk_pos, std::vector<std::string> const& merklelist,
    dev::network::NodeID const& destnodeId)
{
    sendChunkMessage(
        RESPONSECHUNKMESSAGETYPE, coding_epoch, group_id, chunk_pos, merklelist, destnodeId);
}
void EurasureP2P::sendProof(
    unsigned int pos, std::string proofs, dev::network::NodeID const& destnodeId)
{
    sendProofMessage(pos, proofs, destnodeId);
}

void EurasureP2P::requestState(unsigned int block_num, std::string key, NodeAddr const& destnodeId)
{
    sendStateMessage(0, block_num, key, "", destnodeId);
}
void EurasureP2P::responseState(unsigned int block_num, std::string const& key,
    std::string const& data, NodeAddr const& destnodeId)
{
    sendStateMessage(1, block_num, key, data, destnodeId);
}