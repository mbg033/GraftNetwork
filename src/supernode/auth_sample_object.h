#ifndef AUTH_SAMPLE_OBJECT_H_
#define AUTH_SAMPLE_OBJECT_H_

#include "base_rta_object.h"
#include "supernode_common_struct.h"

namespace supernode {
	class auth_sample_object: public base_rta_object {
		public:
		bool init(const rta_transaction_record& src) {
			transaction_record = src;

/*
			init_subnet();
			if( !broadcast_record("PosProxySale") ) return false;

			// TODO: add all other handlers for this sale request
			m_dapi_rpc_server->ADD_DAPI_GLOBAL_METHOD_HANDLER(transaction_record.payment_id, GetSaleStatus, rpc_command::POS_GET_SALE_STATUS, pos_sale_object);
*/
			return true;
		}

		bool WalletProxyPay(const rta_transaction_record& src) {
			// TODO: IMPL compare - must be equal
			// TODO: build sub-net
			// TODO: send LOCK. WTF?? all our nodes got this packet by sub-net broadcast. so only top node must send broad cast
			// TODO: sign transaction record and send it to wallet.
			// TODO: add handler for:
			//                         got respose from wallet 'all signed and tx in pool'
			//                         get pos data

			return true;
		}
		public:
		string wallet_dapi_ip;
		string wallet_dapi_port;
		public:
		string pos_dapi_ip;
		string pos_dapi_port;
		public:
		string pay_to_wallet;


		protected:
		virtual bool init(const rta_transaction_record_base& src) override { return false; }


	};

}

#endif /* AUTH_SAMPLE_OBJECT_H_ */
