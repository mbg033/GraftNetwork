#ifndef POS_SALE_OBJECT_H_
#define POS_SALE_OBJECT_H_

#include "base_rta_object.h"

namespace supernode {

	class pos_sale_object : public base_rta_object {
		public:
		bool init(const rta_transaction_record_base& src) override {
			base_rta_object::init(src);

			transaction_record.block_num = m_fsn_servant->get_current_block_num();
			transaction_record.auth_nodes = m_fsn_servant->get_auth_sample( transaction_record.block_num );

			init_subnet();
			if( !broadcast_record("PosProxySale") ) return false;

			// TODO: add all other handlers for this sale request
			m_dapi_rpc_server->ADD_DAPI_GLOBAL_METHOD_HANDLER(transaction_record.payment_id, GetSaleStatus, rpc_command::POS_GET_SALE_STATUS, pos_sale_object);

			return true;
		}

		bool GetSaleStatus(const rpc_command::POS_GET_SALE_STATUS::request& in, rpc_command::POS_GET_SALE_STATUS::response& out) {
			// TODO: IMPL
			return true;
		}

	};

}

#endif /* POS_SALE_OBJECT_H_ */
