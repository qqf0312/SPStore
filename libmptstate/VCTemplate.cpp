#pragma once

#include "VCTemplate.h"
#include "rocksdb/db.h"
#include <tbb/parallel_for.h>
#include <iostream>
#include <sstream>
#include <string>









bool VCTemplate::writeDB()
{
    // write param
    pointproofs_pp_bytes pp_bytes;
    pointproofs_vp_bytes vp_bytes;
    if (pointproofs_pp_serial(vc_param.prover, &pp_bytes))
    {
        std::cout << m_name << " ERROR:serialize prover" << std::endl;
        return false;
    }

    if (pointproofs_vp_serial(vc_param.verifier, &vp_bytes))
    {
        std::cout << m_name << " ERROR:serialize verifier" << std::endl;
        return false;
    }

    rocksdb::Status status = m_db->Put(rocksdb::WriteOptions(), m_name + "_param_prover",
        std::string(pp_bytes.data, pp_bytes.data + pp_bytes.len));
    if (!status.ok())
    {
        std::cout << m_name << " ERROR:pp writeDB" << std::endl;
        return false;
    }
    status = m_db->Put(rocksdb::WriteOptions(), m_name + "_param_verifier",
        std::string(vp_bytes.data, vp_bytes.data + vp_bytes.len));
    if (!status.ok())
    {
        std::cout << m_name << " ERROR:vp writeDB" << std::endl;
        return false;
    }
    pointproofs_free_pp_string(pp_bytes);
    pointproofs_free_vp_string(vp_bytes);


    // write commit
    pointproofs_commitment_bytes commit_bytes;
    if (pointproofs_commit_serial(vc_commit, &commit_bytes))
    {
        std::cout << m_name << " ERROR:commit serial" << std::endl;
        return false;
    }
    // std::cout << "m_name + _commit = " << m_name + "_commit" << std::endl;
    status = m_db->Put(rocksdb::WriteOptions(), m_name + "_commit",
        std::string(commit_bytes.data, commit_bytes.data + commit_bytes.len));
    if (!status.ok())
    {
        std::cout << m_name << " ERROR:commit writeDB" << std::endl;
        return false;
    }

    // write value?

    // write proofs
    pointproofs_proof_bytes proof_bytes;
    for (int i = 0; i < m_size; i++)
    {
        if (pointproofs_proof_serial(vc_proofs[i], &proof_bytes))
        {
            std::cout << m_name << " ERROR:proof serial" << std::endl;
            return false;
        }
        status = m_db->Put(rocksdb::WriteOptions(), m_name + "_proofs_" + std::to_string(i),
            std::string(proof_bytes.data, proof_bytes.data + proof_bytes.len));
        if (!status.ok())
        {
            std::cout << m_name << " ERROR:proof writeDB" << std::endl;
            return false;
        }
    }

    // write flag
    status = m_db->Put(rocksdb::WriteOptions(), m_name  +DecToHex(m_size)+"_flag", "1");
    if (!status.ok())
    {
        std::cout << m_name << " ERROR:flag writeDB" << std::endl;
        return false;
    }
    return true;
}
