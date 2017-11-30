// Copyright (c) 2017, The Graft Project
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without modification, are
// permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice, this list of
//    conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright notice, this list
//    of conditions and the following disclaimer in the documentation and/or other
//    materials provided with the distribution.
//
// 3. Neither the name of the copyright holder nor the names of its contributors may be
//    used to endorse or promote products derived from this software without specific
//    prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY
// EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL
// THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
// STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
// THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// Parts of this file are originally copyright (c) 2014-2017 The Monero Project




#include <cryptonote_core/cryptonote_core.h>

#include <string>
#include <vector>
#include <map>
#include <stdint.h>

#pragma	once

namespace cryptonote {

class BlockchainDB;
class Blockchain;
class tx_memory_pool;

}

namespace graft {

/*!
 * \brief The supernode_handler class - implementation of real-time-authorization methods
 */
class SupernodeHandler final
{
public:
    enum Status {
        Status_Good,
        Status_Error
    };

    SupernodeHandler(const std::string &daemon_addr, const std::string &dbpath, bool testnet);
    /*!
     * \brief get_solved_block_owner - returns the list of wallet addresses who solved last `last_blocks_count` blocks
     * \param miner_wallets          - list of wallet addresses and corresponding viewkeys to check
     * \param last_blocks_count      - number of last blocks to check
     * \param found_wallets          - list of wallet addresses and corresponding blocks
     * \return                       - true on success, false if any error happened; TODO: error code and verbose description
     */
    bool  getSolvedBlockOwner(const std::vector<std::pair<std::string, std::string>> &miner_wallets,
                                 uint64_t last_blocks_count,
                                 std::vector<std::pair<std::string, uint64_t>> &found_wallets);
    int status() const;

private:
    /*!
     * \brief proofCoinbaseTx - check given block mined by given address;
     * \param address         - miner's wallet address
     * \param block           - block to check
     * \param viewkey         - private viewkey
     * \return                - true if blocl
     */
    bool proofCoinbaseTx(const cryptonote::account_public_address &address, const cryptonote::block &block, const crypto::secret_key &viewkey);

    // void generateOrOpenDummyWallet();
    bool initBlockchain(const std::string &dbpath, bool testnet);

private:

    bool                         m_testnet = false;
    std::string                  m_daemonAddress;
    cryptonote::BlockchainDB   * m_bdb;
    cryptonote::Blockchain     * m_bc;
    cryptonote::tx_memory_pool * m_mempool;
    Status                       m_status = Status_Good;

};


} // namespace graft

