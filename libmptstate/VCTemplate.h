#pragma once
extern "C" {
#include "pointproofs.h"
}
#include "rocksdb/db.h"
#include <ctime>
#include <iostream>
#include <sstream>
#include <string>
#include <sys/time.h>
#include <tbb/parallel_for.h>
static inline double GetMS() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec * 1000 + tv.tv_usec / 1000.0;
}

static inline std::string DecToHex(unsigned int num) {
    std::ostringstream buffer;
    buffer << std::hex << std::uppercase << num;
    return buffer.str();
}

class VCTemplate {
  private:
    friend class VCUpdater;
    std::string m_name;

    pointproofs_params vc_param;
    pointproofs_value vc_initValue;
    pointproofs_commitment vc_commit;
    pointproofs_proof *vc_proofs;
    void initCommitment();

  public:
    /**
     * VCTemplate初始化
     * input:
     *  _db: VCTemplate所在数据库句柄
     *  _name: VCTemplate存储对应的名称
     *  _size: VC数组大小
     *  _initValue: VC数组的初始化内容
     *
     *
     **/
    VCTemplate(rocksdb::DB *_db, const std::string &_name, int _size,
               const std::string &_initValue)
        : m_db(_db), m_name(_name), m_size(_size) {
        // init value
        m_name += std::to_string(m_size);
        std::cout << m_name << " init value";
        uint8_t *tmp = new uint8_t[_initValue.length()];
        memcpy(tmp, (uint8_t *)(const_cast<char *>(_initValue.c_str())),
               _initValue.length());
        vc_initValue.data = tmp;
        vc_initValue.len = _initValue.length();

        // init param commit proofs
        char seed[] = "this is a very long seed for pointproofs tests";
        std::string flag("");

        rocksdb::Status status = m_db->Get(
            rocksdb::ReadOptions(), m_name + DecToHex(m_size) + "_flag", &flag);
        if (!status.ok() || (status.ok() && flag != "1")) {
            pointproofs_value initValues[m_size];
            for (int i = 0; i < m_size; i++) {
                initValues[i].data = vc_initValue.data;
                initValues[i].len = vc_initValue.len;
            }

            std::cout << " param";
            pointproofs_paramgen((const uint8_t *)seed, sizeof(seed), 0, m_size,
                                 &vc_param);

            std::cout << " commitment";
            if (pointproofs_commit(vc_param.prover, initValues, m_size,
                                   &vc_commit))
                std::cout << m_name << " ERROR:initCommit" << std::endl;

            std::cout << " proofs";
            vc_proofs = new pointproofs_proof[m_size];
            tbb::parallel_for(
                tbb::blocked_range<int>(0, m_size),
                [&](const tbb::blocked_range<int> &_r) {
                    for (int pos = _r.begin(); pos != _r.end(); ++pos) {
                        auto time1 = GetMS();
                        pointproofs_prove(vc_param.prover, initValues, m_size,
                                          pos, &vc_proofs[pos]);
                        // std::cout << GetMS() - time1 << std::endl;
                    }
                });

            std::cout << " writeDB" << std::endl;
            writeDB();
        } else {
            std::cout << " readDB" << std::endl;
            std::string temp_string;
            status = m_db->Get(rocksdb::ReadOptions(), m_name + "_param_prover",
                               &temp_string);
            assert(status.ok());
            pointproofs_pp_bytes pp_bytes;
            pp_bytes.data =
                (uint8_t *)(const_cast<char *>(temp_string.c_str()));
            pp_bytes.len = temp_string.length();
            assert(!pointproofs_pp_deserial(pp_bytes, &vc_param.prover));

            status = m_db->Get(rocksdb::ReadOptions(),
                               m_name + "_param_verifier", &temp_string);
            assert(status.ok());
            pointproofs_vp_bytes vp_bytes;
            vp_bytes.data =
                (uint8_t *)(const_cast<char *>(temp_string.c_str()));
            vp_bytes.len = temp_string.length();
            assert(!pointproofs_vp_deserial(vp_bytes, &vc_param.verifier));

            pointproofs_proof_bytes proof_bytes;
            vc_proofs = new pointproofs_proof[m_size];
            for (int i = 0; i < m_size; i++) {
                status = m_db->Get(rocksdb::ReadOptions(),
                                   m_name + "_proofs_" + std::to_string(i),
                                   &temp_string);
                proof_bytes.data =
                    (uint8_t *)(const_cast<char *>(temp_string.c_str()));
                proof_bytes.len = temp_string.length();
                // proof_bytes=StringToBytes<pointproofs_proof_bytes>(temp_string);
                assert(!pointproofs_proof_deserial(proof_bytes, &vc_proofs[i]));
            }

            pointproofs_commitment_bytes commit_bytes;
            status = m_db->Get(rocksdb::ReadOptions(), m_name + "_commit",
                               &temp_string);
            commit_bytes.data =
                (uint8_t *)(const_cast<char *>(temp_string.c_str()));
            commit_bytes.len = temp_string.length();
            assert(!pointproofs_commit_deserial(commit_bytes, &vc_commit));
        }
    }

    VCTemplate() {}

    ~VCTemplate() {
        delete[] vc_initValue.data;
        pointproofs_free_commit(vc_commit);
        pointproofs_free_prover_params(vc_param.prover);
        pointproofs_free_verifier_params(vc_param.verifier);
        for (int i = 0; i < m_size; i++)
            pointproofs_free_proof(vc_proofs[i]);

        delete[] vc_proofs;
    };
    int m_size;
    rocksdb::DB *m_db;
    bool writeDB();
    pointproofs_proof getProof(int _pos) { return vc_proofs[_pos]; }
    pointproofs_commitment getCommitment() { return vc_commit; }
    pointproofs_params getParams() { return vc_param; }
};
