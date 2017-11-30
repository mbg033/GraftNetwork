#ifndef POS_PROXY_H_
#define POS_PROXY_H_

#include "pos_sale_object.h"
#include "base_rta_processor.h"

namespace supernode {
	class pos_proxy : public base_rta_processor {
		public:

		protected:
		void init() override {
			m_dapi_rpc_server->ADD_DAPI_HANDLER(Sale, rpc_command::POS_SALE, pos_proxy);
			// TODO: add all other handlers
		}


		bool Sale(const rpc_command::POS_SALE::request& in, rpc_command::POS_SALE::response& out) {
			boost::shared_ptr<pos_sale_object> data = boost::shared_ptr<pos_sale_object>( new pos_sale_object() );
			setup(data);
			if( !data->init(in) ) return false;
			add(data);

			return true;
		}



	};

}

#endif /* POS_PROXY_H_ */
