#ifndef AUTH_SAMPLE_H_
#define AUTH_SAMPLE_H_

#include "base_rta_processor.h"
#include "auth_sample_object.h"
#include "supernode_rpc_command.h"
#include <boost/pointer_cast.hpp>

namespace supernode {
	class auth_sample : public base_rta_processor {
		public:



		protected:
		void init() override {
			m_dapi_rpc_server->ADD_SUBNET_BROADCAST_HANDLER(PosProxySale, rpc_command::RTA_TRANSACTION_OBJECT, auth_sample);
			m_dapi_rpc_server->ADD_SUBNET_BROADCAST_HANDLER(WalletProxyPay, rpc_command::RTA_TRANSACTION_OBJECT, auth_sample);
			// TODO: add all other handlers
		}


		bool PosProxySale(const rpc_command::RTA_TRANSACTION_OBJECT& in, rpc_command::SUB_NET_BROADCAST_RESPONCE& out) {
			out.Fail = true;

			rta_transaction_record tr;
			if( !convert(tr, in) ) return false;
			if( !check(tr) ) return false;

			boost::shared_ptr<auth_sample_object> data = boost::shared_ptr<auth_sample_object>( new auth_sample_object() );
			setup(data);
			if( !data->init(tr) ) return false;

			data->pos_dapi_ip = in.sender_ip;
			data->pos_dapi_port = in.sender_port;
			data->pay_to_wallet = in.pay_to_wallet;

			add(data);


			out.Fail = false;
			return true;
		}

		bool WalletProxyPay(const rpc_command::RTA_TRANSACTION_OBJECT& in, rpc_command::SUB_NET_BROADCAST_RESPONCE& out) {
			out.Fail = true;

			boost::shared_ptr<base_rta_object> ff = object_by_payment(in.payment_id);
			boost::shared_ptr<auth_sample_object> data = boost::dynamic_pointer_cast<auth_sample_object>(ff);
			if(!data) return false;

			if(data->pay_to_wallet!=in.pay_to_wallet) { remove_record(data); return false; }

			data->wallet_dapi_ip = in.sender_ip;
			data->wallet_dapi_port = in.sender_port;

			rta_transaction_record tr;
			if( !convert(tr, in) ) { remove_record(data); return false; }

			if( !data->WalletProxyPay(tr) ) { remove_record(data); return false; }


			out.Fail = false;
			return true;
		}

		bool convert(rta_transaction_record& tr, const rpc_command::RTA_TRANSACTION_OBJECT& in) {
			// TODO: IMPL
			return true;
		}
		bool check(rta_transaction_record& tr) {
			// TODO: IMPL check sample selection if ok for givved block number
			return true;
		}

		void remove_record(boost::shared_ptr<auth_sample_object> record) {}


	};


};




#endif /* AUTH_SAMPLE_H_ */
