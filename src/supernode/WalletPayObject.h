#ifndef WALLET_PAY_OBJECT_H_
#define WALLET_PAY_OBJECT_H_

#include "BaseRTAObject.h"


namespace supernode {

	class WalletPayObject : public BaseRTAObject {
		public:
		bool Init(const RTA_TransactionRecordBase& src) override;

		bool GetPayStatus(const rpc_command::WALLET_GET_TRANSACTION_STATUS::request& in, rpc_command::WALLET_GET_TRANSACTION_STATUS::response& out);
		bool WalletTRSigned(const rpc_command::WALLET_TR_SIGNED::request& in, rpc_command::WALLET_TR_SIGNED::response& out);

		protected:
		bool PutTXToPool();

		protected:
		vector<rpc_command::WALLET_TR_SIGNED::request> m_Signs;// TODO: set proper struct here

	};

}

#endif /* WALLET_PAY_OBJECT_H_ */
