#ifndef SUPERNODE_RPC_COMMAND_H_
#define SUPERNODE_RPC_COMMAND_H_

#include "supernode_common_struct.h"

namespace supernode {
	namespace rpc_command {
		// ---------------------------------------
		struct WALLET_PAY {
			struct request : public rta_transaction_record_base {
			};
			struct response {
				string data_for_client_wallet;
			};
		};


		// ---------------------------------------
		struct POS_SALE {
			struct request : public rta_transaction_record_base {
			};
			struct response {
			};
		};


		// ---------------------------------------
		struct POS_GET_SALE_STATUS {
			struct request : public sub_net_data {
			};
			struct response {
			};
		};


		// ---------------------------------------
		struct SUB_NET_BROADCAST_RESPONCE {
			bool Fail;
		};
		struct RTA_TRANSACTION_OBJECT : public rta_transaction_record_base {
			vector<fsn_data> nodes;
			//for DAPI call
			string sender_ip;
			string sender_port;
			string pay_to_wallet;
		};

	};
};



#endif /* SUPERNODE_RPC_COMMAND_H_ */
