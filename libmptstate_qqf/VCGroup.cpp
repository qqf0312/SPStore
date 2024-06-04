#pragma once

#include "VCGroup.h"
#include <atomic>
VCTemplate* VCGroup::subVCTemplate = nullptr;
VCTemplate* VCGroup::mainVCTemplate = nullptr;
tbb::concurrent_queue<VCGroup*> VCGroup::proofTaskQueue;

static inline int computePostion(const std::string& posStr, int mod)
{
    uint64_t res = 0;
    for (int i = 0; i < 16; ++i)
    {
        // res += data[i] * 2;
        // std::cout<<posStr[i];
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
    // std::cout<<"computePos "<<res<<std::endl;
    return res % mod;
}

void VCGroup::setKV(const std::map<std::string, std::string>& _kvMap)
{
    for (auto it = _kvMap.begin(); it != _kvMap.end(); it++)
    {
        // std::cout << "setkv size = " <<
        // it->first.length()+it->second.length() << std::endl;
        int pos = computePostion(it->first, mainVCTemplate->m_size);
        posToAccounts[pos].insert(std::string(it->first + it->second));
        // std::cout<<it->first+it->second<<std::endl;
    }
}

void VCGroup::commitBlock(const std::map<std::string, std::string>& _kvMap)
{
    setKV(_kvMap);
    updateCommit(8);
}

void VCGroup::initTemplate(rocksdb::DB* vc_db, int main_size = 16, int sub_size = 5000)
{
    rocksdb::Options options;
    options.create_if_missing = true;
    // assert(vc_db);
    subVCTemplate = new VCTemplate(vc_db, "sub", sub_size, "   0");
    pointproofs_commitment_bytes commit_bytes;
    pointproofs_commit_serial(subVCTemplate->getCommitment(), &commit_bytes);
    mainVCTemplate = new VCTemplate(vc_db, "main", main_size,
        std::string(commit_bytes.data, commit_bytes.data + commit_bytes.len));
    // std::cout << "initTemplate success" << std::endl;
}

bool VCGroup::verify(std::string const& data, std::string const& maincommit,
    std::string const& maincommit_proof, int main_position, std::string const& subcommit,
    std::string const& subcommit_proof, int sub_position)
{
    // pointproofs_verify
    pointproofs_commitment commit;
    pointproofs_commitment_bytes commit_bytes;
    pointproofs_proof proof;
    pointproofs_proof_bytes proof_bytes;
    pointproofs_value values;

    values.data = (const uint8_t*)data.c_str();
    values.len = data.size();
    std::cout<<"len:"<<values.len<<std::endl;
    commit_bytes.data = (uint8_t*)const_cast<char*>(subcommit.c_str());
    commit_bytes.len = subcommit.length();
    proof_bytes.data = (uint8_t*)const_cast<char*>(subcommit_proof.c_str());
    proof_bytes.len = subcommit_proof.length();
    pointproofs_proof_deserial(proof_bytes, &proof);
    pointproofs_commit_deserial(commit_bytes, &commit);
    if (pointproofs_verify(
            subVCTemplate->getParams().verifier, commit, proof, values, sub_position) == true)
    {
        std::cout << "subcommit verify ok" << std::endl;
        pointproofs_free_proof(proof);
        pointproofs_free_commit(commit);
        values.data = (const uint8_t*)subcommit.c_str();
        values.len = subcommit.size();
        commit_bytes.data = (uint8_t*)const_cast<char*>(maincommit.c_str());
        commit_bytes.len = maincommit.length();
        proof_bytes.data = (uint8_t*)const_cast<char*>(maincommit_proof.c_str());
        proof_bytes.len = maincommit_proof.length();
        pointproofs_proof_deserial(proof_bytes, &proof);
        pointproofs_commit_deserial(commit_bytes, &commit);
        if (pointproofs_verify(
                mainVCTemplate->getParams().verifier, commit, proof, values, main_position) == true)
        {
            std::cout << "maincommit verify ok" << std::endl;
            pointproofs_free_proof(proof);
            pointproofs_free_commit(commit);
            return true;
        }
        else
        {
            std::cout << "maincommit verify failed" << std::endl;
            pointproofs_free_proof(proof);
            pointproofs_free_commit(commit);
        }
    }
    else
    {
        std::cout << "subCommit verify failed" << std::endl;
    }
    pointproofs_free_proof(proof);
    pointproofs_free_commit(commit);
    return false;
}

std::string VCGroup::getProof(
    const std::map<std::string, std::string>& _kvMap, const std::string& key)
{
    std::string vo = "";
    // std::cout << "vo init:" << vo.length() << std::endl;
    assert(recoverMain());
    // std::cout << "pos:" << computePostion(key, 16) << ":" << _kvMap.size() <<
    // std::endl;
    setKV(_kvMap);
    double start_time = GetMS();
    int pos = computePostion(key, mainVCTemplate->m_size);
    // std::cout << " VCGroup::getProof key in maincommit position =  " << pos
    // << std::endl; std::cout << " VCGroup::getProof key in subcommit position
    // =  "
    //           << computePostion(key, subVCTemplate->m_size) << std::endl;

    int count=0;
    auto& accounts = posToAccounts[pos];
    for (auto it = accounts.begin(); it != accounts.end();)
    {
        int sub_position = computePostion(*it, subVCTemplate->m_size);

        std::string newValue(*it);
        // std::cout << " newValue = " << newValue << " size = " <<
        // newValue.size() << std::endl;
        it++;
        while (it != accounts.end() && sub_position == computePostion(*it, subVCTemplate->m_size))
        {
            newValue.append(*it);
            count++;
            it++;
        }
        subUpdater[pos]->setValue(sub_position, newValue);
    }

    vo.append(subUpdater[pos]->getProof(computePostion(key, subVCTemplate->m_size)));
    // std::cout << GetMS() - start_time << std::endl;
    // std::cout << "subVO length = " << vo.length() << std::endl;
    // std::cout << "subVO = " << vo << std::endl;
    // recoverMain();
    vo.append(mainUpdater->getProof(pos));
    // std::cout << "proof time = " <<  GetMS() - start_time << std::endl;
    // std::cout << "subcommit = " << vo.substr(85,49) <<std::endl;
    // std::cout << "main commit proof= " << vo.substr(134,49) << std::endl;
    // std::cout << "mainVO lengh:" << vo.length() << std::endl;
    // std::cout << "data = " << vo.substr(0, 36) << std::endl;
    mainUpdater->updateCommit(8);
    int pacl = 49;
    int len = vo.size();
    std::string subcommit_proof = vo.substr(len - 3 * pacl, pacl);
    std::string subcommit = vo.substr(len - 2 * pacl, pacl);
    std::string maincommit_proof = vo.substr(len - pacl, pacl);
    std::cout << "len:"<<len<<"verify:"
              << VCGroup::verify(vo.substr(0, len - 3 * pacl), mainUpdater->getCommitStr(), maincommit_proof, pos,
                     subcommit, subcommit_proof,
                     computePostion(key, subVCTemplate->m_size))
              << std::endl;
    return vo;
}

void VCGroup::updateCommit(int _threadNumber)
{
    auto time1 = GetMS();
    tbb::parallel_for(
        tbb::blocked_range<int>(0, mainVCTemplate->m_size, mainVCTemplate->m_size / _threadNumber),
        [&](const tbb::blocked_range<int>& _r) {
            // std::cout<<"range begin:"<<_r.begin()<<std::endl;
            for (int pos = _r.begin(); pos != _r.end(); ++pos)
            {
                std::set<std::string> accounts = posToAccounts[pos];
                for (auto it = accounts.begin(); it != accounts.end();)
                {
                    int sub_position = computePostion(*it, subVCTemplate->m_size);

                    std::string newValue(*it);
                    // std::cout << " VCGroup::updateCommit value = " <<
                    // newValue<< std::endl;
                    it++;
                    while (it != accounts.end() &&
                           sub_position == computePostion(*it, subVCTemplate->m_size))
                    {
                        newValue.append(*it);
                        it++;
                    }
                    subUpdater[pos]->setValue(sub_position, newValue);
                }
                subUpdater[pos]->updateCommit();
                if (!subUpdater[pos]->writeCommit(blockNumber, "_subcommit", pos))
                    std::cout << "SUB COMMIT write error" << std::endl;
                mainUpdater->setValue(pos, subUpdater[pos]->getCommitStr());
                // std::cout << "set main:" << pos << std::endl;
            }
        });

    auto time2 = GetMS();
    mainUpdater->updateCommit(_threadNumber);

    mainUpdater->writeCommit(blockNumber, "_commit");
    std::cout << "commit time =" << time2 - time1 << ";" << GetMS() - time2
              << std::endl;
}

bool VCGroup::recoverMain()
{
    rocksdb::Status status;
    std::string value;
    for (int pos = 0; pos < mainVCTemplate->m_size; pos++)
    {
        status = mainVCTemplate->m_db->Get(
            rocksdb::ReadOptions(), DecToHex(blockNumber) + "_subcommit" + DecToHex(pos), &value);
        if (status.ok())
            mainUpdater->setValue(pos, value);
        else
        {
            std::cout << "RECOVER MAIN:read commit fail" << std::endl;
            return false;
        }
    }
    // std::cout<<"writeDB:"<<GetMS()-startTime<<std::endl;
    return true;
}

void VCGroup::testvc()
{
    size_t n = 65536;
  size_t k = n-8; // For more flexible `n` val. 
  size_t u_index = k + 1;
  size_t u_index_new = k ;
  double starttime = GetMS();
  // values to commit
  int counter = 0;
  pointproofs_value values[n];
  for (counter = 0; counter < n; counter++) {
    char * tmp = (char*) malloc(36*22 * sizeof(char));
    sprintf(tmp, "This is message %d for commit %d!", counter, 0);
    values[counter].data = (const unsigned char*) tmp;
    values[counter].len = strlen(tmp);
  }

#ifdef DEBUG
  printf("values:\n");
  for (counter = 0; counter < n; counter++) {
    printf("%zu: %s\n", values[counter].len, values[counter].data);
  }
#endif

  // generate parameters
  char seed[] = "this is a very long seed for pointproofs tests";
  uint8_t ciphersuite = 0;
  starttime = GetMS();
  pointproofs_params pointproofs_param;
  pointproofs_paramgen((const uint8_t*) seed, sizeof(seed), ciphersuite, n, &pointproofs_param);
  // assert();
  printf( "generate parameters time  = %f\n" ,GetMS() - starttime );

  // testing (de)serialization of parameters
  pointproofs_pp_bytes pointproofs_pp_string;
  pointproofs_vp_bytes pointproofs_vp_string;
  pointproofs_pp pp_recover;
  pointproofs_vp vp_recover;
  pointproofs_pp_bytes pointproofs_pp_string_recover;
  pointproofs_vp_bytes pointproofs_vp_string_recover;

  assert(pointproofs_pp_serial(pointproofs_param.prover, &pointproofs_pp_string) == 0);
  assert(pointproofs_vp_serial(pointproofs_param.verifier, &pointproofs_vp_string) == 0);
  assert(pointproofs_pp_deserial(pointproofs_pp_string, &pp_recover) == 0);
  assert(pointproofs_vp_deserial(pointproofs_vp_string, &vp_recover) == 0);
  assert(pointproofs_pp_serial(pp_recover, &pointproofs_pp_string_recover) == 0);
  assert(pointproofs_vp_serial(vp_recover, &pointproofs_vp_string_recover) == 0);

#ifdef DEBUG
  hexDump("prover param (in bytes)", pointproofs_pp_string.data, pointproofs_pp_string.len);
  hexDump("prover param recovered (in bytes)", pointproofs_pp_string_recover.data, pointproofs_pp_string_recover.len);

  hexDump("verifier param (in bytes)", pointproofs_vp_string.data, pointproofs_vp_string.len);
  hexDump("verifier param recovered (in bytes)", pointproofs_vp_string_recover.data, pointproofs_vp_string_recover.len);
#endif

  assert(memcmp(pointproofs_pp_string.data, pointproofs_pp_string_recover.data, pointproofs_pp_string.len) == 0);
  assert(memcmp(pointproofs_vp_string.data, pointproofs_vp_string_recover.data, pointproofs_vp_string.len) == 0);

  // generate a commit
  pointproofs_commitment commit;
  pointproofs_commitment_bytes commit_string;
  pointproofs_commitment commit_recover;
  pointproofs_commitment_bytes commit_string_recover;
  starttime = GetMS();
  pointproofs_commit(pp_recover, values, n, &commit) == 0;
  printf( "commit 1000 time  = %f\n",GetMS() - starttime );
  // assert(pointproofs_commit(pp_recover, values, n, &commit) == 0);
  assert(pointproofs_commit_serial(commit, &commit_string) == 0);
  assert(pointproofs_commit_deserial(commit_string, &commit_recover) == 0);
  assert(pointproofs_commit_serial(commit_recover, &commit_string_recover) == 0);

#ifdef DEBUG
  hexDump("commit (in bytes)", commit_string.data, commit_string.len);
  hexDump("commit recovered (in bytes)", commit_string_recover.data, commit_string_recover.len);
#endif

  assert(strcmp((const char*) commit_string.data, (const char*) commit_string_recover.data) == 0);

  pointproofs_proof proof;
  pointproofs_proof_bytes proof_string;
  pointproofs_proof proof_recover;
  pointproofs_proof_bytes proof_string_recover;

  for (counter = 0; counter < k; counter++) {
    // generate a proof
    starttime = GetMS();
    pointproofs_prove(pp_recover, values, n, counter, &proof);
    printf("generate a proof time  = %f\n" ,GetMS() - starttime );
    // assert(pointproofs_prove(pp_recover, values, n, counter, &proof) == 0);
    assert(pointproofs_proof_serial(proof, &proof_string) == 0);
   // printf("proof size = %d\n",proof_string.len);    
assert(pointproofs_proof_deserial(proof_string, &proof_recover) == 0);
    assert(pointproofs_proof_serial(proof_recover, &proof_string_recover) == 0);

#ifdef DEBUG
    hexDump("proof (in bytes)", proof_string.data, proof_string.len);
    hexDump("proof recovered (in bytes)", proof_string_recover.data, proof_string_recover.len);
#endif

    assert(strcmp((const char*) proof_string.data, (const char*) proof_string_recover.data) == 0);

    // verify the proof
    //assert(pointproofs_verify(vp_recover, commit, proof, values[counter], counter) == true);
    starttime = GetMS();
	pointproofs_verify(vp_recover, commit, proof, values[counter], counter); 
  printf("verify time  = %f\n" ,GetMS() - starttime );
}

  // update the commitment for index = 33
  pointproofs_commitment new_commit;
  pointproofs_proof new_proof;
starttime = GetMS();
  //assert(pointproofs_commit_update(pp_recover, commit, u_index, values[u_index], values[u_index_new], &new_commit) == 0);
pointproofs_commit_update(pp_recover, commit, u_index, values[u_index], values[u_index_new], &new_commit);
printf("commit update time  = %f\n" ,GetMS() - starttime );  
for (counter = 0; counter < k; counter++) {
    // update the proofs; the updated index will be 33
    assert(pointproofs_prove(pp_recover, values, n, counter, &proof) == 0);
    //assert(pointproofs_proof_update(pp_recover, proof, counter, u_index, values[u_index], values[u_index_new], &new_proof) == 0);
    // verify the new proof
    starttime = GetMS();
    pointproofs_proof_update(pp_recover, proof, counter, u_index, values[u_index], values[u_index_new], &new_proof);
printf("proof update time  = %f\n" ,GetMS() - starttime );
    assert(pointproofs_verify(vp_recover, new_commit, new_proof, values[counter], counter) == true);
  }

  pointproofs_free_commit(commit);
  pointproofs_free_commit(commit_recover);
  pointproofs_free_commit(new_commit);
  pointproofs_free_proof(proof);
  pointproofs_free_proof(proof_recover);
  pointproofs_free_proof(new_proof);
  pointproofs_free_prover_params(pointproofs_param.prover);
  pointproofs_free_prover_params(pp_recover);
  pointproofs_free_verifier_params(pointproofs_param.verifier);
  pointproofs_free_verifier_params(vp_recover);

  pointproofs_free_pp_string(pointproofs_pp_string); //need to free.
  pointproofs_free_pp_string(pointproofs_pp_string_recover); //need to free.
  pointproofs_free_vp_string(pointproofs_vp_string); //need to free.
  pointproofs_free_vp_string(pointproofs_vp_string_recover); //need to free.
  pointproofs_free_commit_string(commit_string); //need to free.
  pointproofs_free_commit_string(commit_string_recover); //need to free.
  pointproofs_free_proof_string(proof_string); //need to free.
  pointproofs_free_proof_string(proof_string_recover); //need to free.


  printf("basis tests: success\n");

}