#ifndef WALLET_PROXY_H_
#define WALLET_PROXY_H_

#include "wallet_pay_object.h"
#include "base_rta_processor.h"

namespace supernode {
	class wallet_proxy : public base_rta_processor {
		public:


		protected:
		void init() override {
			m_dapi_rpc_server->ADD_DAPI_HANDLER(Pay, rpc_command::WALLET_PAY, wallet_proxy);
			// TODO: add all other handlers
		}


		bool Pay(const rpc_command::WALLET_PAY::request& in, rpc_command::WALLET_PAY::response& out) {
			boost::shared_ptr<wallet_pay_object> data = boost::shared_ptr<wallet_pay_object>( new wallet_pay_object() );
			setup(data);
			if( !data->init(in) ) return false;
			add(data);

			out.data_for_client_wallet = data->transaction_record.data_for_client_wallet;

			return true;


		}


	};
}

#endif /* WALLET_PROXY_H_ */
