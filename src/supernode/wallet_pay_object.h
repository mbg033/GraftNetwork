#ifndef WALLET_PAY_OBJECT_H_
#define WALLET_PAY_OBJECT_H_

#include "base_rta_object.h"

namespace supernode {

	class wallet_pay_object : public base_rta_object {
		public:
		bool init(const rta_transaction_record_base& src) override {
			base_rta_object::init(src);

			// we allready have block num
			transaction_record.auth_nodes = m_fsn_servant->get_auth_sample( transaction_record.block_num );

			init_subnet();
			if( !broadcast_record("WalletProxyPay") ) return false;

			// TODO: call to one FSN to get POS data and fill transaction_record.data_for_client_wallet

			// TODO: add all other handlers for this sale request
			m_dapi_rpc_server->ADD_DAPI_GLOBAL_METHOD_HANDLER(transaction_record.payment_id, GetPayStatus, rpc_command::WALLET_PAY, wallet_pay_object);

			return true;
		}

		bool GetPayStatus(const rpc_command::WALLET_PAY::request& in, rpc_command::WALLET_PAY::response& out) {
			// TODO: IMPL
			return true;
		}



	};

}

#endif /* WALLET_PAY_OBJECT_H_ */
