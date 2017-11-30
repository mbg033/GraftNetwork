#ifndef SUB_NET_BROADCAST_H_
#define SUB_NET_BROADCAST_H_

#include "supernode_common_struct.h"
#include <boost/function.hpp>
#include <boost/bind.hpp>


namespace supernode {

	class dapi_rpc_server;

	class sub_net_broadcast {
		public:
		// all messages will send with subnet_id
		// and handler will recieve only messages with subnet_id
		// as subnet_id we use PaymentID, generated as UUID in PoS
		// ALL data structs (IN and OUT) must be child form sub_net_data
		void set_members( dapi_rpc_server* pa, uuid_t subnet_id, const vector< boost::shared_ptr<fsn_data> >& members ) {}

		// TODO: we need access to some socket to got and filter message from incoming port (with one?)
		// TODO: in PoS and Wallet we need poll proxy to get status


		public:
		template<class T>
		bool send( const string& method, const T& data ) {}// return false if any of call's to other was fail

		template<class T>
		void add_handler( const string& method, boost::function<void (T)> handler ) {}

		protected:
		dapi_rpc_server* m_dapi_rpc_server	 = nullptr;

};


}

#endif /* SUB_NET_BROADCAST_H_ */
