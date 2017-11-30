#ifndef FSN_SERVANT_H_
#define FSN_SERVANT_H_

#include "supernode_common_struct.h"

#include <cryptonote_core/cryptonote_core.h>
#include <boost/thread/mutex.hpp>
#include <boost/thread/lock_guard.hpp>
#include <string>

namespace cryptonote {

class BlockchainDB;
class Blockchain;
class tx_memory_pool;


}

namespace supernode {

class fsn_servant {
public:

    fsn_servant(const std::string &db_path, bool testnet);
    // start from blockchain top and check, if block solved by one from  full_super_node_servant::all_fsn
    // push block number and full_super_node_data to output vector. stop, when output_vector.size==blockNums OR blockchain ends
    // output_vector.begin - newest block, solved by FSN
    // may be called from different threads.
    std::vector<std::pair<uint64_t, boost::shared_ptr<fsn_data>>>  last_blocks_resolved_by_fsn(uint64_t startFromBlock, uint64_t blockNums) const;
    // start scan blockchain from forBlockNum, scan from top to bottom
    vector< boost::shared_ptr<fsn_data> > get_auth_sample(uint64_t forBlockNum) const;
    uint64_t get_current_block_num() const;
    // TODO: all other stuff, such check wallet addr and stake we can put in this class too
    string sign_by_wallet_private_key(const string& str, const string& wallet_addr) const;
    bool is_sign_valid(const string& str, const fsn_wallet_data& wallet) const;
    // calc balance from chain begin to block_num
    uint64_t get_wallet_balance(uint64_t block_num, const fsn_wallet_data& wallet) const;

    void add_fsn(boost::shared_ptr<fsn_data> fsn);
public: // TODO: following should be private methods

    // next two fields may be references
    mutable boost::mutex all_fsn_guard;// DO NOT block for long time. if need - use copy
    vector< boost::shared_ptr<fsn_data>> all_fsn;// access to this data may be done from different threads

    static const unsigned fsn_per_auth_sample = 8;

private:
    static bool proofCoinbaseTx(const cryptonote::account_public_address &address, const cryptonote::block &block,
                         const crypto::secret_key &viewkey);
    bool initBlockchain(const std::string &dbpath, bool testnet);

private:

    bool                         m_testnet = false;
    cryptonote::BlockchainDB   * m_bdb;
    cryptonote::Blockchain     * m_bc;
    cryptonote::tx_memory_pool * m_mempool;

};

} // namespace supernode

#endif /* FSN_SERVANT_H_ */
