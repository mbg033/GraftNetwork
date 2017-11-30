#ifndef FSN_SERVANT_H_
#define FSN_SERVANT_H_

#include "supernode_common_struct.h"
#include <boost/thread/mutex.hpp>
#include <boost/thread/lock_guard.hpp>

namespace supernode {

class fsn_servant {
public:
    // next two fields may be references
    mutable boost::mutex all_fsn_guard;// DO NOT block for long time. if need - use copy
    vector< boost::shared_ptr<fsn_data> > all_fsn;// access to this data may be done from different threads
    // start from blockchain top and check, if block solved by one from  full_super_node_servant::all_fsn
    // push block number and full_super_node_data to output vector. stop, when output_vector.size==blockNums OR blockchain ends
    // output_vector.begin - newest block, solved by FSN
    // may be called from different threads.
    vector<pair<uint64_t, boost::shared_ptr<fsn_data> > > last_blocks_resolved_by_fsn(uint64_t startFromBlock, uint64_t blockNums) const;
    // start scan blockchain from forBlockNum, scan from top to bottom
    vector< boost::shared_ptr<fsn_data> > get_auth_sample(uint64_t forBlockNum) const;
    uint64_t get_current_block_num() const;
    // TODO: all other stuff, such check wallet addr and stake we can put in this class too
    string sign_by_wallet_private_key(const string& str, const string& wallet_addr) const;
    bool is_sign_valid(const string& str, const fsn_wallet_data& wallet) const;
    // calc balance from chain begin to block_num
    uint64_t get_wallet_balance(uint64_t block_num, const fsn_wallet_data& wallet) const;
    static const unsigned fsn_per_auth_sample = 8;
};

} // namespace supernode

#endif /* FSN_SERVANT_H_ */
