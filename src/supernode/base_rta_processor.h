#ifndef BASE_RTA_PROCESSOR_H_
#define BASE_RTA_PROCESSOR_H_

#include "base_rta_object.h"

namespace supernode {

	class base_rta_processor {
		public:
		virtual ~base_rta_processor() {}

		void set(const fsn_servant* ser, dapi_rpc_server* dapi) {
			m_fsn_servant = ser;
			m_dapi_rpc_server = dapi;
			init();
		}

		protected:
		void add(boost::shared_ptr<base_rta_object> obj) {
			boost::lock_guard<boost::mutex> lock(m_objects_guard);
			m_objects.push_back(obj);
		}
		void setup(boost::shared_ptr<base_rta_object> obj) {
			obj->set(m_fsn_servant, m_dapi_rpc_server);
		}

		boost::shared_ptr<base_rta_object> object_by_payment(const uuid_t payment_id) {
			boost::shared_ptr<base_rta_object> ret;
			{
				boost::lock_guard<boost::mutex> lock(m_objects_guard);
				for(auto& a : m_objects) if(a->transaction_record.payment_id==payment_id) {
					ret = a;
					break;
				}
			}
			return ret;
		}

		virtual void init()=0;

		protected:
		const fsn_servant* m_fsn_servant = nullptr;
		dapi_rpc_server* m_dapi_rpc_server = nullptr;
		mutable boost::mutex m_objects_guard;
		vector< boost::shared_ptr<base_rta_object> > m_objects;

	};

}

#endif /* BASE_RTA_PROCESSOR_H_ */
