#ifndef DAPI_RPC_SERVER_H_
#define DAPI_RPC_SERVER_H_

#include "supernode_rpc_command.h"

namespace supernode {

	class dapi_rpc_server {
		public:
		template<class IN, class OUT>
		void add_handler( const string& method, boost::function<bool (const IN&, OUT&)> handler ) {}
		#define ADD_DAPI_HANDLER(method, data, class_owner) add_handler<data::request, data::response>( #method, bind( &class_owner::method, this, _1, _2) );
		#define ADD_SUBNET_BROADCAST_HANDLER(method, data, class_owner) add_handler<data, rpc_command::SUB_NET_BROADCAST_RESPONCE>( #method, bind( &class_owner::method, this, _1, _2) );

		// IN must be child from sub_net_data
		// income message filtered by payment_id and method
		template<class IN, class OUT>
		void add_uuid_method_handler( uuid_t paymentid, const string& method, boost::function<bool (const IN&, OUT&)> handler ) {}
		#define ADD_DAPI_GLOBAL_METHOD_HANDLER(payid, method, data, class_owner) add_uuid_method_handler<data::request, data::response>( payid, #method, bind( &class_owner::method, this, _1, _2) );
	};

};




#endif /* DAPI_RPC_SERVER_H_ */
