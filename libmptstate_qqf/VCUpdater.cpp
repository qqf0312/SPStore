#pragma once
#include "VCUpdater.h"
#include <tbb/concurrent_queue.h>
#include <tbb/concurrent_unordered_map.h>
#include <tbb/concurrent_vector.h>

#include <map>
#include <string>

tbb::concurrent_queue<std::string> VCUpdater::ProofStrQueue;


bool VCUpdater::verifyProof(const std::string& _proofStr, int _pos)
{
    // std::cout << "verify:" << _pos << "len:" << _proofStr.length() << std::endl;
    pointproofs_proof proof;
    pointproofs_proof_bytes proof_bytes;
    proof_bytes.data = (uint8_t*)(const_cast<char*>(_proofStr.c_str()));
    proof_bytes.len = _proofStr.length();
    pointproofs_proof_deserial(proof_bytes, &proof);
    auto time1 = GetMS();
    bool res =
        pointproofs_verify(m_template->vc_param.verifier, m_commit, proof, PosToValue[_pos], _pos);
    // std::cout << "verify:" << res << " cost:" << GetMS() - time1 << std::endl;
    // pointproofs_free_proof_string(proof_bytes);
    pointproofs_free_proof(proof);
    return res;
}


void VCUpdater::setValue(int _pos, const std::string& _value)
{
    // std::cout << "set " << _pos << ":" << _value << std::endl;
    // auto startTime=GetMS();
    uint8_t* tmp = new uint8_t[_value.length()];
    memcpy(tmp, (uint8_t*)(const_cast<char*>(_value.c_str())), _value.length());
    pointproofs_value newValue;
    newValue.data = tmp;
    newValue.len = _value.length();
    PosToValue[_pos] = newValue;
    isProved = false;
    // std::cout<<GetMS()-startTime<<std::endl;
}


std::string VCUpdater::getCommitStr()
{
    pointproofs_commitment_bytes commit_bytes;
    pointproofs_commit_serial(m_commit, &commit_bytes);
    std::string res = std::string(commit_bytes.data, commit_bytes.data + commit_bytes.len);
    pointproofs_free_commit_string(commit_bytes);
    return res;
}


void VCUpdater::updateCommit(int _threadNumber)
{
    size_t changedID[PosToValue.size()];
    pointproofs_value oldValues[PosToValue.size()];
    pointproofs_value newValues[PosToValue.size()];

    auto time1 = GetMS();
    if (PosToValue.size() == m_template->m_size)
    {
        for (int i = 0; i < m_template->m_size; i++)
            newValues[i] = PosToValue[i];
        pointproofs_commit(m_template->vc_param.prover, newValues, m_template->m_size, &m_commit);
        // std::cout<<"main Commit:"<<PosToValue.size()<<" time:"<<GetMS()-time1<<std::endl;
    }
    else
    {
        int index = 0;
        for (auto i = PosToValue.begin(); i != PosToValue.end(); i++)
        {
            int _pos = i->first;
            changedID[index] = _pos;
            oldValues[index] = m_template->vc_initValue;
            newValues[index] = i->second;
            index++;
        }
        pointproofs_batch_commit_update(m_template->vc_param.prover, m_commit, changedID, oldValues,
            newValues, PosToValue.size(), &m_commit);
        // std::cout<<"sub Commit:"<<PosToValue.size()<<" time:"<<GetMS()-time1<<std::endl;
    }
}

std::string VCUpdater::getProof(int pos)
{
    pointproofs_proof_bytes proof_bytes;
    pointproofs_proof newProof;
    auto proofTime = GetMS();

    pointproofs_proof_update(m_template->vc_param.prover, m_template->vc_proofs[pos], pos, pos,
        m_template->vc_initValue, m_template->vc_initValue, &newProof);

    std::string value = "";
    int count=0;
    if (PosToValue.find(pos) != PosToValue.end())
    {
        value = std::string(PosToValue[pos].data, PosToValue[pos].data + PosToValue[pos].len);
        int _pos = pos;

        for (auto i = PosToValue.begin(); i != PosToValue.end(); i++)
        {
            // std::cout<<i->first<<" "<<newProof.data<<std::endl;
            pointproofs_proof oldProof = newProof;
            // auto time1=GetMS();
            count++;
            pointproofs_proof_update(m_template->vc_param.prover, newProof, _pos, i->first,
                m_template->vc_initValue, i->second, &newProof);  // 0.4ms
            // std::cout<<"proof:"<<GetMS()-time1<<std::endl;
            pointproofs_free_proof(oldProof);
            // std::cout<<i->first<<" "<<newProof.data<<std::endl;
        }
    }
    // pointproofs_proof_serial(m_template->vc_proofs[pos], &proof_bytes);
    pointproofs_proof_serial(newProof, &proof_bytes);
    std::string proof = std::string(proof_bytes.data, proof_bytes.data + proof_bytes.len);
    // PosToProofStr[_pos] = std::string(proof_bytes.data, proof_bytes.data + proof_bytes.len);
    pointproofs_free_proof(newProof);
    pointproofs_free_proof_string(proof_bytes);
    // std::cout << "VCUpdater::getProof proof time  = " << GetMS() - proofTime << " count:"<<count<<std::endl;
    // std::cout << "VCUpdater::getProof  value =" << value << std::endl;
    return value + proof;
}

bool VCUpdater::writeCommit(int _blockNumber, const std::string& _tag, int _subcommitId)
{
    // rocksdb::Status status=m_db->Put()
    // auto startTime=GetMS();
    pointproofs_commitment_bytes commit_bytes;
    pointproofs_commit_serial(m_commit, &commit_bytes);
    rocksdb::Status status;
    if (_subcommitId >= 0)
    {
        status = m_template->m_db->Put(rocksdb::WriteOptions(),
            DecToHex(_blockNumber) + _tag + DecToHex(_subcommitId),
            std::string(commit_bytes.data, commit_bytes.data + commit_bytes.len));
        // if (_subcommitId == 6)
        // {
        //     std::cout << "subcommit " << DecToHex(_blockNumber) + _tag + DecToHex(_subcommitId)
        //               << "  = "
        //               << std::string(commit_bytes.data, commit_bytes.data + commit_bytes.len)
        //               << std::endl;
        // }
    }
    else
    {
        status = m_template->m_db->Put(rocksdb::WriteOptions(), DecToHex(_blockNumber) + _tag,
            std::string(commit_bytes.data, commit_bytes.data + commit_bytes.len));
        // std::cout << "maincommit " << DecToHex(_blockNumber) + _tag << "  = "
        //           << std::string(commit_bytes.data, commit_bytes.data + commit_bytes.len)
        //           << std::endl;
    }


    pointproofs_free_commit_string(commit_bytes);
    // std::cout<<"writeDB:"<<GetMS()-startTime<<std::endl;
    return status.ok();
}
