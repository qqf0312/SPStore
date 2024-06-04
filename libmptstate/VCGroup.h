#pragma once
#include "VCUpdater.h"
#include <map>
#include <set>
#include <tbb/concurrent_unordered_map.h>
#include <tbb/concurrent_vector.h>

class VCGroup {
  private:
    /**
     * 上层VC模板和下层的VC模板
     *
     *
     */
    static VCTemplate *mainVCTemplate;
    static VCTemplate *subVCTemplate;
    static tbb::concurrent_queue<VCGroup *> proofTaskQueue;

    /**
     * 上层VC更新句柄和下层VC更新句柄
     *
     *
     */
    VCUpdater *mainUpdater;
    std::vector<VCUpdater *> subUpdater;
    std::atomic_int provedCount;
    /**
     * VC数组位置和对应位置上的映射数据集合
     *
     *
     */
    std::map<int, std::set<std::string>> posToAccounts;

  public:
    double localProveTime;
    double setProofsTime = 0;
    /**
     * 初始化VC模板
     * input:
     *  vc_db: VC对应的数据库句柄
     *  main_size, sub_size: 上层VC的数组大小和下层VC的数据大小
     *
     */
    static void initTemplate(rocksdb::DB *vc_db, int main_size, int sub_size);

    /**
     * 数据的验证操作，正确返回true
     * input:
     *  data: VC对应位置的数据
     *  maincommit: 上层VC的Commit
     *  maincommit_proof: 上层VC指定位置的数据证明
     *  main_position: 数据在上层VC的位置索引
     *  subcommit: 下层VC的Commit
     *  subcommit_proof: 下层VC指定位置的数据证明
     *  sub_position: 数据在下层VC的位置索引
     */
    static bool verify(std::string const &data, std::string const &maincommit,
                       std::string const &maincommit_proof, int main_position,
                       std::string const &subcommit,
                       std::string const &subcommit_proof, int sub_position);
    int blockNumber;
    static void testvc();
    VCGroup(int _blockNumber) : provedCount(0), blockNumber(_blockNumber) {
        assert(subVCTemplate != nullptr || mainVCTemplate != nullptr);

        mainUpdater = new VCUpdater(mainVCTemplate);
        // subUpdater = reinterpret_cast<VCUpdater*>(new
        // char[subVCTemplate->m_size * sizeof(VCUpdater)]); for (auto i = 0; i
        // < subVCTemplate->m_size; i++)
        //     new (subUpdater + i) VCUpdater(subVCTemplate);

        for (int i = 0; i < mainVCTemplate->m_size; i++)
            subUpdater.push_back(new VCUpdater(subVCTemplate));

        for (int i = 0; i < mainVCTemplate->m_size; i++)
            posToAccounts[i] = std::set<std::string>();
    }

    ~VCGroup() {
        // std::cout << "delete vcgroup" << std::endl;
        delete mainUpdater;
        for (int i = 0; i < subUpdater.size(); i++)
            delete subUpdater[i];
    }
    void setKV(const std::map<std::string, std::string> &_kvMap);

    /**
     * 构造VC-Tree
     *
     * input:
     *  _kvMap: 位置以及对应位置的数据信息
     */
    void commitBlock(const std::map<std::string, std::string> &_kvMap);
    /**
     * 获取对应key所在VC-Tree的验证信息
     * input:
     *  _kvMap: key所在epoch的数据信息
     *  key: 需要验证信息的键
     */
    std::string getProof(const std::map<std::string, std::string> &_kvMap,
                         const std::string &key);
    /**
     * 初始化VC参数
     * 从之前写入的db读取对应的commit
     *
     */
    bool recoverMain();
    /**
     * 并发更新VC信息
     * input:
     *  线程数
     *
     *
     */
    void updateCommit(int _threadNumber);
};
