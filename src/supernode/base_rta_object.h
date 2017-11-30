#ifndef BASE_AUTH_OBJECT_H_
#define BASE_AUTH_OBJECT_H_

#include "supernode_common_struct.h"
#include "sub_net_broadcast.h"
#include "dapi_rpc_server.h"
#include "fsn_servant.h"


namespace supernode {

	class base_rta_object {
		public:
		rta_transaction_record transaction_record;

		public:
		virtual bool init(const rta_transaction_record_base& src) {
			rta_transaction_record_base& dst = transaction_record;
			dst = src;
			return true;
		}

		virtual void set(const fsn_servant* ser, dapi_rpc_server* dapi) {
			m_fsn_servant = ser;
			m_dapi_rpc_server = dapi;
		}
		virtual ~base_rta_object() {}

		protected:
		void init_subnet() {
			m_sub_net_broadcast.set_members(m_dapi_rpc_server, transaction_record.payment_id, transaction_record.auth_nodes);
		}

		bool broadcast_record(const string& call) {
			return m_sub_net_broadcast.send(call, transaction_record);
		}


		protected:
		sub_net_broadcast m_sub_net_broadcast;
		const fsn_servant* m_fsn_servant = nullptr;
		dapi_rpc_server* m_dapi_rpc_server = nullptr;

	};

}

#endif /* BASE_AUTH_OBJECT_H_ */
