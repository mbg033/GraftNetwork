// Copyright (c) 2018, The Graft Project
// Copyright (c) 2014-2017, The Monero Project
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
// Parts of this file are originally copyright (c) 2012-2013 The Cryptonote developers

#include "include_base_utils.h"
using namespace epee;

#include "test_server.h"
#include "common/command_line.h"
#include "common/updates.h"
#include "common/download.h"
#include "common/util.h"
#include "misc_language.h"


#undef MONERO_DEFAULT_LOG_CATEGORY
#define MONERO_DEFAULT_LOG_CATEGORY "daemon.rpc"

#define MAX_RESTRICTED_FAKE_OUTS_COUNT 40
#define MAX_RESTRICTED_GLOBAL_FAKE_OUTS_COUNT 500

namespace {
static const char *response_text =
       "           GNU LESSER GENERAL PUBLIC LICENSE "
       "                       Version 3, 29 June 2007"
    " Copyright (C) 2007 Free Software Foundation, Inc. <http://fsf.org/>"
    " Everyone is permitted to copy and distribute verbatim copies"
    "f this license document, but changing it is not allowed."
    "This version of the GNU Lesser General Public License incorporates"
    "the terms and conditions of version 3 of the GNU General Public"
    "License, supplemented by the additional permissions listed below."

    "  0. Additional Definitions.";
}

namespace cryptonote
{

  //-----------------------------------------------------------------------------------
  void test_server::init_options(boost::program_options::options_description& desc)
  {


  }
  //------------------------------------------------------------------------------------------------------------------------------
  test_server::test_server()
  {
    LOG_PRINT_L0(__FUNCTION__);
  }
  //------------------------------------------------------------------------------------------------------------------------------
  bool test_server::init()
  {
    m_net_server.set_threads_prefix("RPC");


    return epee::http_server_impl_base<test_server, connection_context>::init(
      "8000", "0.0.0.0"
          );
  }

  bool test_server::on_test(const COMMAND_TEST::request &req, COMMAND_TEST::response &res)
  {
    LOG_PRINT_L0("req: " << req.request);
    res.response = response_text;
    return true;
  }

}  // namespace cryptonote



int main(int argc, char **argv)
{

  try {
    cryptonote::test_server server;
    server.init();
    LOG_PRINT_L0("Starting test server");
    server.run(100);
    LOG_PRINT_L0("Stopping test server");
  } catch (const std::exception& e)
  {
    LOG_ERROR("Server error: " << e.what());
    return 1;
  }

  return 0;


}
