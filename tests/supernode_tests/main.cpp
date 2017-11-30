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

#include "gtest/gtest.h"

#include "wallet/wallet2_api.h"
#include "wallet/wallet2.h"
#include "supernode/supernode_handler.h"
#include "supernode/fsn_servant.h"
#include "include_base_utils.h"
#include "cryptonote_config.h"

#include <boost/chrono/chrono.hpp>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread/condition_variable.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/thread.hpp>

#include <iostream>
#include <vector>
#include <atomic>
#include <functional>
#include <string>

using namespace graft;
using namespace supernode;
using namespace std;

namespace consts {

}


struct SupernodeHandlerTest : public testing::Test
{

    SupernodeHandler * sh = nullptr;

    SupernodeHandlerTest()
    {
        string db_path = epee::string_tools::get_current_module_folder() + "/../../../../tests/data/supernode/test_blockchain";
        std::cout << db_path << std::endl;
        sh = new SupernodeHandler("", db_path, true);
    }

    ~SupernodeHandlerTest()
    {
        delete sh;
    }

};




TEST_F(SupernodeHandlerTest, CreateDestroyInstance)
{

    ASSERT_TRUE(sh->status() == SupernodeHandler::Status_Good);

}

TEST_F(SupernodeHandlerTest, ProofOfStakeTestMiner)
{
    vector<pair<string, string>> input;
    const string address1 = "T6SQG5uxxtZC17hQdnapt3WjHKZnKoJS5gzz9QMvyBthEa4ChsHujswji7vdzgUos371nBjVbweVyTrqi8mxwZHh2k1KZ14WJ";
    const string viewkey1 = "582305765f0b173567926068c66b6073632b705100772ac066472d75479f2b07";
    const string address2 = "T6TyzMRMpksMftG4twjXyaC1vdoJ4axHg3xxtbWiQ5Ps3soR779vdNF2R7iEhyZ1Uicacfc8X3drQFmtzLZtnPN81TwSDmyun";
    const string viewkey2 = "455224dc3f6363fa09590efa43f5b6bdc04194d2a9c6c91e7605f7083771d20a";
    input.push_back(std::make_pair(address1,
                    viewkey1));
    input.push_back(std::make_pair(address2,
                    viewkey2));
    vector<pair<string, uint64_t>> output;

    ASSERT_TRUE(sh->getSolvedBlockOwner(input, 1, output));
    ASSERT_TRUE(output.size() == 1);
    ASSERT_TRUE(address2 == output[0].first);


    ASSERT_TRUE(sh->getSolvedBlockOwner(input, 90, output));
    ASSERT_TRUE(output.size() == 90);
    ASSERT_TRUE(address1 == output[2].first);
    ASSERT_TRUE(2 == output[0].second);
    ASSERT_TRUE(address1 == output[3].first);
    ASSERT_TRUE(3 == output[1].second);
    ASSERT_TRUE(address1 == output[4].first);
    ASSERT_TRUE(4 == output[2].second);
    ASSERT_TRUE(address1 == output[5].first);
    ASSERT_TRUE(5 == output[3].second);


    ASSERT_TRUE(address2 == output[89].first);
    ASSERT_TRUE(91 == output[89].second);
    ASSERT_TRUE(address2 == output[87].first);
    ASSERT_TRUE(89 == output[87].second);

    output.clear();
    ASSERT_FALSE(sh->getSolvedBlockOwner(input, 1000*1000, output));
    ASSERT_TRUE(output.size() == 0);
}

TEST_F(SupernodeHandlerTest, ProofOfStakeTestNotMiner)
{
    vector<pair<string, string>> input;
    const string address = "T6Tzxn3e6E5RQdqr1XQobp2hnWhgcp5CXB1A8uG7LEDk9hHjsCkkkkWb7Y8zgGG915FR2racSumuPBJzcaJMg5cz1Gk2bDaUY";
    input.push_back(std::make_pair(address,
                    "fdbaef2b89aa679896103b0fb054edb5629c6f2309e1cf7052820576b8ccc30f"));
    vector<pair<string, uint64_t>> output;

    ASSERT_TRUE(sh->getSolvedBlockOwner(input, 10, output));
    ASSERT_TRUE(output.size() == 0);

}



struct FSNServantTest : public testing::Test
{

    fsn_servant * fsns = nullptr;
    string db_path = epee::string_tools::get_current_module_folder() + "/../../../../tests/data/supernode/test_blockchain";


    FSNServantTest()
    {
        fsns = new fsn_servant(db_path, true);

    }

    ~FSNServantTest()
    {
        delete fsns;
    }
};


TEST_F(FSNServantTest, CreateDestroyInstance)
{
    ASSERT_TRUE(fsns);
}

TEST_F(FSNServantTest, ProofOfStakeTestMiner)
{
    const string address1 = "T6SQG5uxxtZC17hQdnapt3WjHKZnKoJS5gzz9QMvyBthEa4ChsHujswji7vdzgUos371nBjVbweVyTrqi8mxwZHh2k1KZ14WJ";
    const string viewkey1 = "582305765f0b173567926068c66b6073632b705100772ac066472d75479f2b07";
    const string address2 = "T6TyzMRMpksMftG4twjXyaC1vdoJ4axHg3xxtbWiQ5Ps3soR779vdNF2R7iEhyZ1Uicacfc8X3drQFmtzLZtnPN81TwSDmyun";
    const string viewkey2 = "455224dc3f6363fa09590efa43f5b6bdc04194d2a9c6c91e7605f7083771d20a";

    fsns->add_fsn(boost::make_shared<fsn_data>(fsn_wallet_data{"", ""}, fsn_wallet_data{address1, viewkey1}));
    fsns->add_fsn(boost::make_shared<fsn_data>(fsn_wallet_data{"", ""}, fsn_wallet_data{address2, viewkey2}));

    vector<pair<uint64_t, boost::shared_ptr<fsn_data>>> output =
            fsns->last_blocks_resolved_by_fsn(1, 1);
    std::cout << "size: " << output.size() << std::endl;
    ASSERT_TRUE(output.size() == 1);
    ASSERT_TRUE(output[0].first == 1);
    ASSERT_TRUE(output[0].second->miner.addr == address1);


    output.clear();

    output = fsns->last_blocks_resolved_by_fsn(1, 10);
    std::cout << "size: " << output.size() << std::endl;
    ASSERT_TRUE(output.size() == 10);
    ASSERT_TRUE(output[0].first == 1);
    ASSERT_TRUE(output[0].second->miner.addr == address1);
    ASSERT_TRUE(output[9].second->miner.addr == address2);

    output = fsns->last_blocks_resolved_by_fsn(20000, 10);
    ASSERT_TRUE(output.empty());

    output = fsns->last_blocks_resolved_by_fsn(20, 10000);
    ASSERT_FALSE(output.empty());
    for (const auto &iter : output) {
        ASSERT_TRUE(iter.second->miner.addr == address1 || iter.second->miner.addr == address2);
    }


}

int main(int argc, char** argv)
{
    epee::string_tools::set_module_name_and_folder(argv[0]);
    mlog_configure("", true);
    mlog_set_log_level(1);
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
