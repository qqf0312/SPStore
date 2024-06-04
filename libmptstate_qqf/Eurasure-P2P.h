
#include <libdevcore/Base64.h>
#include <libdevcore/Common.h>
#include <libdevcore/CommonData.h>
#include <libdevcore/FixedHash.h>
#include <libethcore/Block.h>
#include <libethcore/Exceptions.h>
#include <libnetwork/Common.h>
#include <libnetwork/Session.h>
#include <libp2p/P2PInterface.h>
#include <libp2p/P2PMessage.h>
#include <libsync/Common.h>
#include <libsync/SyncMsgPacket.h>
#include <stdlib.h>
#include <tbb/concurrent_queue.h>
#include <tbb/concurrent_unordered_map.h>
#include <unistd.h>

#include <algorithm>
#include <chrono>
#include <cmath>
#include <ctime>
#include <iostream>
#include <memory>
#include <string>

#define REQUESTBLOCKMESSAGETYPE 0
#define REQUESTCHUNKMESSAGETYPE 0
#define RESPONSEBLOCKMESSAGETYPE 1
#define RESPONSECHUNKMESSAGETYPE 1
#define BlockPoint std::shared_ptr<dev::eth::Block>
#define NodeAddr dev::h512
#define P2PManager std::shared_ptr<dev::p2p::P2PInterface>
#define MessageProtocolID dev::PROTOCOL_ID

namespace ec {
class Eurasure;
/*纠删码网络模块*/
class EurasureP2P {
  public:
    /**
     * 初始化纠删码网络服务
     * input:
     *    nodeid:节点哈希
     *    _service: 底层网络服务模块
     *    protocolId: 纠删码网络传输服务标识
     *
     * */
    EurasureP2P(NodeAddr const &nodeid,
                std::shared_ptr<dev::p2p::Service> _service,
                MessageProtocolID const &_protocolId)
        : m_nodeId(nodeid), m_service(_service), m_protocolId(_protocolId) {
        m_groupId = dev::eth::getGroupAndProtocol(_protocolId).first;
        // std::cout << "m_ecId = " << m_protocolId << std::endl;
        registerHandler();
    }
    void setEurasure(ec::Eurasure *eurasure);
    void registerHandler();
    void removeHandler();
    ~EurasureP2P() { removeHandler(); }
    /**
     * 纠删码消息处理
     * input:
     *  _e: 异常处理句柄
     *  _session: P2P网络会话句柄
     *  _msg: 接收到的消息
     *
     * */
    void messageHandler(dev::p2p::NetworkException _e,
                        std::shared_ptr<dev::p2p::P2PSession> _session,
                        dev::p2p::P2PMessage::Ptr _msg);
    /**
     * 发送数据块相关消息
     * input:
     *  message_type: 消息类型
     *  coding_epoch: 数据块所在轮次
     *  group_id, chunk_pos: 数据块所在组号以及在组中的相对索引位置
     *  merklelist: 数据块的默克尔证明
     *  dest_node_id: 请求的目标节点哈希
     */
    void sendChunkMessage(uint8_t message_type, unsigned int coding_epoch,
                          unsigned int group_id, unsigned int chunk_pos,
                          std::vector<std::string> const &merklelist,
                          dev::network::NodeID const &dest_node_id);
    void sendStateMessage(uint8_t message_type, unsigned int block_num,
                          std::string const &key, std::string const &data,
                          dev::network::NodeID const &dest_node_id);
    void sendProofMessage(unsigned int pos, std::string proofs,
                          dev::network::NodeID const &destnodeId);
    void requestChunk(unsigned int coding_epoch, unsigned int group_id,
                      unsigned int chunk_pos, NodeAddr const &destnodeId);

    void responseChunk(unsigned int coding_epoch, unsigned int group_id,
                       unsigned int chunk_pos,
                       std::vector<std::string> const &merklelist,
                       NodeAddr const &destnodeId);
    void requestState(unsigned int block_num, std::string key,
                      NodeAddr const &destnodeId);
    void responseState(unsigned int block_num, std::string const &key,
                       std::string const &data, NodeAddr const &destnodeId);
    void sendHeart(std::string const &heart, NodeAddr const &destnodeId);
    void sendProof(unsigned int pos, std::string proofs,
                   dev::network::NodeID const &destnodeId);
    void processPacket(dev::sync::SyncMsgPacket::Ptr packet,
                       NodeAddr const &destnodeId);

    tbb::concurrent_unordered_map<std::string, unsigned int> chunk_count_map;
    tbb::concurrent_unordered_map<std::string, std::string> chunk_data_map;
    std::map<std::string, std::string> state_map;
    tbb::concurrent_queue<std::pair<int, std::string>> vc_proof_queue;

  private:
    NodeAddr m_nodeId;
    std::shared_ptr<dev::p2p::Service> m_service;
    MessageProtocolID m_protocolId;
    dev::GROUP_ID m_groupId;
    ec::Eurasure *m_eurasure;
};
} // namespace ec