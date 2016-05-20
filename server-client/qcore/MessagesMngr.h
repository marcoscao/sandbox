/*
 * process received header/body and send signals per message type
 */
#ifndef CORE_MSGMANAGER_H__
#define CORE_MSGMANAGER_H__

#include <QtCore/QObject>
#include <memory>

namespace core {

	struct userid_access_request_body;

	class MsgManager : public QObject { 
		Q_OBJECT

	public:
		void dispatch_message( QTcpSocket * );

	signals:
		void undefined_message_type_sig();
		void assign_client_name_sig();
		void userid_access_request_sig( std::unique_ptr< userid_access_request_body > );
		void userid_authorization_sig();
		void client_finished_sig();
		void server_shutdown_sig();
	};

} // end namespace

#endif

