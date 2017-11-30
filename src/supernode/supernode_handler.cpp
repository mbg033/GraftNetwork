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

#include "supernode_handler.h"
#include <blockchain_db/blockchain_db.h>
#include <cryptonote_core/tx_pool.h>
#include <cryptonote_core/blockchain.h>

using namespace cryptonote;

namespace helpers {
public_key get_tx_gen_pub_key(const transaction &tx)
{

    if (!is_coinbase(tx)) {
        return null_pkey;
    }
    const tx_out &out = tx.vout.at(0);
    return boost::get<txout_to_key>(out.target).key;
}

public_key get_tx_pub_key_from_received_outs(const transaction &tx)
{
    std::vector<tx_extra_field> tx_extra_fields;

    if(!parse_tx_extra(tx.extra, tx_extra_fields))
    {
        // Extra may only be partially parsed, it's OK if tx_extra_fields contains public key
    }

    // Due to a previous bug, there might be more than one tx pubkey in extra, one being
    // the result of a previously discarded signature.
    // For speed, since scanning for outputs is a slow process, we check whether extra
    // contains more than one pubkey. If not, the first one is returned. If yes, they're
    // checked for whether they yield at least one output
    tx_extra_pub_key pub_key_field;

    if (!find_tx_extra_field_by_type(tx_extra_fields, pub_key_field, 0))
    {
        return null_pkey;
    }

    public_key tx_pub_key = pub_key_field.pub_key;

    bool two_found = find_tx_extra_field_by_type(tx_extra_fields, pub_key_field, 1);

    if (!two_found)
    {
        // easy case, just one found
        return tx_pub_key;
    }
    else
    {
        // just return second one if there are two.
        // this does not require private view key, as
        // its not needed for my use case.
        return pub_key_field.pub_key;
    }

    return null_pkey;
}


}

namespace graft {

SupernodeHandler::SupernodeHandler(const std::string &daemon_addr, const string &dbpath, bool testnet)
    : m_daemonAddress{daemon_addr}
    , m_testnet{testnet}
{
    if (!initBlockchain(dbpath, testnet))
        m_status = Status_Error;
}

bool SupernodeHandler::getSolvedBlockOwner(const std::vector<std::pair<std::string, std::string> > &miner_wallets,
                                           uint64_t last_blocks_count, std::vector<std::pair<std::string, uint64_t> > &found_wallets)
{
    // sanity check
    if (m_status != Status_Good)
        return false;

    uint64_t block_height = m_bc->get_current_blockchain_height();
    if (last_blocks_count > block_height) {
        return false;
    }

    found_wallets.clear();
    // option #2: as we have direct access to blockchain, we can use Blockchain class directly
    //1. retrieve the blocks
    uint64_t start_block = block_height - last_blocks_count;
    LOG_PRINT_L3("Start checking from block " << start_block);
    LOG_PRINT_L3("block height " << block_height);
    for (uint64_t block_index = start_block; block_index < block_height; ++block_index) {
        const cryptonote::block block = m_bdb->get_block_from_height(block_index);
        //2. for each blocks, apply function from xmrblocks (page.h:show_my_outputs)
        // TODO: can be faster algorithm?
        for (const auto &wallet_iter :  miner_wallets) {
            secret_key viewkey;
            epee::string_tools::hex_to_pod(wallet_iter.second, viewkey);
            cryptonote::account_public_address address;
            LOG_PRINT_L3("parsing address : " << wallet_iter.first << "; testnet: " << m_testnet);

            if (!cryptonote::get_account_address_from_str(address, m_testnet, wallet_iter.first)) {
                LOG_ERROR("Error parsing address: " << wallet_iter.first);
                continue;
            }

            LOG_PRINT_L3("pub spend key: " << epee::string_tools::pod_to_hex(address.m_spend_public_key));
            LOG_PRINT_L3("pub view key: " << epee::string_tools::pod_to_hex(address.m_view_public_key));

            if (proofCoinbaseTx(address, block, viewkey)) {
                found_wallets.push_back(std::make_pair(wallet_iter.first, block_index));
                // stop wallets loop as we already found the wallet who solved block
                break;
            }
        }
    }

    return true;

}

int SupernodeHandler::status() const
{
    return m_status;
}

bool SupernodeHandler::proofCoinbaseTx(const cryptonote::account_public_address &address, const cryptonote::block &block, const secret_key &viewkey)
{
    // public transaction key is combined with our viewkey
    // to create, so called, derived key.

    crypto::public_key tx_pubkey = helpers::get_tx_pub_key_from_received_outs(block.miner_tx);

    crypto::key_derivation derivation;
    if (!generate_key_derivation(tx_pubkey, viewkey, derivation)) {
        LOG_ERROR("Cant get derived key for: "  << "\n"
             << "pub_view_key: " << tx_pubkey << " and "
             << "prv_view_key" << viewkey);
        return false;
    }

    LOG_PRINT_L3("view_pub_key: " << epee::string_tools::pod_to_hex(address.m_view_public_key));
    LOG_PRINT_L3("priv_viewkey: " << epee::string_tools::pod_to_hex(viewkey));
    public_key output_pubkey = helpers::get_tx_gen_pub_key(block.miner_tx);
    public_key tx_pubkey_derived;
    derive_public_key(derivation,
                      0,
                      address.m_spend_public_key,
                      tx_pubkey_derived);

    LOG_PRINT_L3("out pubkey: " << epee::string_tools::pod_to_hex(output_pubkey));
    LOG_PRINT_L3("pubkey derived: " << epee::string_tools::pod_to_hex(tx_pubkey_derived));

    return tx_pubkey_derived == output_pubkey;
}

bool SupernodeHandler::initBlockchain(const string &dbpath, bool testnet)
{

    m_bc = nullptr;
    m_mempool = new cryptonote::tx_memory_pool(*m_bc);
    m_bc = new cryptonote::Blockchain(*m_mempool);

    m_bdb = cryptonote::new_db("lmdb");
    if (!m_bdb) {
        LOG_ERROR("Error initializing blockchain db");
        // TODO: set status
        return false;
    }

    boost::filesystem::path folder(dbpath);
    folder = boost::filesystem::canonical(folder);

    folder /= m_bdb->get_db_name();
    const std::string filename = folder.string();
    LOG_PRINT_L0("Loading blockchain from folder " << filename << " ...");
    try
    {
        m_bdb->open(filename, DBF_RDONLY);
    }
    catch (const std::exception& e)
    {
        LOG_PRINT_L0("Error opening database: " << e.what());
        return false;
    }
    bool result = m_bc->init(m_bdb, testnet);
    CHECK_AND_ASSERT_MES(result, false, "Failed to initialize source blockchain storage");
    LOG_PRINT_L0("Source blockchain storage initialized OK");
    return result;
}

}
