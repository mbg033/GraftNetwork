#ifndef SUPERNODE_COMMON_STRUCT_H_
#define SUPERNODE_COMMON_STRUCT_H_

#include <uuid/uuid.h>
#include <boost/shared_ptr.hpp>
#include <string>
#include <vector>
#include <inttypes.h>
using namespace std;

namespace supernode {

	// ------------------------------------------
	struct fsn_wallet_data {
		string addr;
		string view_key;
	};


	// ------------------------------------------
	struct fsn_data {
		fsn_wallet_data wallet;
		fsn_wallet_data miner;
		string ip;
		string port;
	};


	// ------------------------------------------
	struct sub_net_data {
		uuid_t payment_id;
	};


	// ------------------------------------------
	struct rta_transaction_record_base : public sub_net_data {
		unsigned sum;
		string pos_wallet;
		string data_for_client_wallet;// empty in wallet call
		uint64_t block_num;// empty in pos call
	};


	// ------------------------------------------
	struct rta_transaction_record : public rta_transaction_record_base {
		vector< boost::shared_ptr<fsn_data> > auth_nodes;
	};



};




#endif /* SUPERNODE_COMMON_STRUCT_H_ */
