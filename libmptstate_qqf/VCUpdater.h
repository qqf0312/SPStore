#pragma once
#include "VCTemplate.h"
#include <map>
#include <string>
#include <tbb/concurrent_queue.h>
#include <tbb/concurrent_unordered_map.h>
#include <tbb/concurrent_vector.h>

class VCUpdater {
  private:
    const VCTemplate *m_template;

  public:
    static tbb::concurrent_queue<std::string> ProofStrQueue;
    tbb::concurrent_unordered_map<int, pointproofs_value> PosToValue;
    bool isProved = false;
    pointproofs_commitment m_commit;
    std::string aggProofStr;
    tbb::concurrent_unordered_map<int, std::string> PosToProofStr;

    void setValue(int _pos, const std::string &_value);
    /**
     * 更新VC数组的Commit
     *
     */
    void updateCommit(int _threadNumber = 1);
    std::string getProof(int pos);
    std::string getCommitStr();
    /**
     *验证数据的证明是否正确
     *input:
     *  _proofStr: 验证信息
     *  _pos: 对应VC数组中的位置
     *
     *
     */
    bool verifyProof(const std::string &_proofStr, int _pos);
    bool writeCommit(int _blockNumber, const std::string &_tag,
                     int _subcommitId = -1);

    VCUpdater(VCTemplate *_template, int _Id = 16) : m_template(_template) {
        pointproofs_commit_update(
            m_template->vc_param.prover, m_template->vc_commit, 0,
            m_template->vc_initValue, m_template->vc_initValue, &m_commit);
    }

    ~VCUpdater() {
        // std::cout << "delete updater" << std::endl;
        if (m_commit.data)
            pointproofs_free_commit(m_commit);
        for (auto i = PosToValue.begin(); i != PosToValue.end(); i++)
            delete[] i->second.data;
    }
};
