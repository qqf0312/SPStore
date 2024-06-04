#include <libblockchain/BlockChainImp.h>
#include <libledger/DBInitializer.h>
#include <libmptstate/Eurasure.h>
#include <libmptstate/MPTState.h>
class Client
{
public:
    Client(std::shared_ptr<dev::ledger::DBInitializer> _dbInitializer,
        std::shared_ptr<dev::blockchain::BlockChainInterface> _chainManager)
      : m_dbInitializer(_dbInitializer), m_chainManager(_chainManager)
    {}
    bool initClient();
    void getState(std::string key, int block_height, std::string& data);
    void readKey(std::string path);
    void work();
    bool findKeyInBloomFilter(std::string key, int block_height);
    void testDecoding(int block_height, std::string const& key);

private:
    std::shared_ptr<dev::ledger::DBInitializer> m_dbInitializer;
    std::shared_ptr<dev::blockchain::BlockChainInterface> m_chainManager;
    std::shared_ptr<dev::mptstate::MPTState> m_mptstate;
    std::map<int, std::set<std::string>> key_set_map;
};