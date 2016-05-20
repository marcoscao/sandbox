#include "MsgManager.h"
#include "Messages.h"

namespace core {

	void MsgManager::dispatch_message( QTcpSocket & socket )
	{
		cout << "* Dispatching message..." << endl;

		// Read header
		QByteArray ba = socket.read( sizeof( Header) );
		QDataStream ds(ba);

		Header h;
		ds >> h;

		cout << "    - type: " << h.type << endl;

		switch( h.type ) {
		case MSG_ASSING_CLIENT_NAME		: emit assign_client_name_sig(); break;

		case MSG_USERID_ACCESS_REQUEST	: 
	  	{
			std::unique_ptr< userid_access_request_body > p_st = std::make_unique< userid_access_request_body >();
			ba = socket.read( sizeof( p_st ) );

			QDataStream ds(ba);
        	ds >> *p_st;
		  	
			emit userid_access_request_sig( p_st ); 							  
			break;
		}

		case MSG_USERID_AUTHORIZATION	: emit userid_authorized_sig(); break;
		case MSG_CLIENT_FINISHED		: emit client_finished_sig(); break;
		case MSG_SERVER_SHUTDOWN		: emit server_shutdown_sig(); break;
		default:
			emit undefined_message_type_sig(); 
			break;
		}
	}

	void MsgManager::

//
// public:
// 	void ready_read_slot( QTcpSocket * socket )
//    {
//       cout << "socket: reading data..." << endl;
//
//       cout << "  reading header..." << endl;
//
//       QByteArray ba = qt_socket_->read( sizeof( Header) );
//       QDataStream ds(ba);
//
//       Header h;
//       ds >> h;
//
//       cout << "    - type: " << h.type << endl;
//
//       switch( h.type ) {
//       case MSG_USERID_REQUEST_ACCESS: 
//       {
//          QByteArray ba2 = qt_socket_->read( sizeof( RequestUserIdAccessMsg ) );
//          QDataStream ds2(ba2);
//
//          RequestUserIdAccessMsg ru;
//          ds2 >> ru;
//
//          cout << "Request user_id : " << ru.user_id << endl;
//          break;
//       }
//       default:
//       {
//          QByteArray msg, msg2;
//          do {
//             msg2 = qt_socket_->read(1024);
//             msg = msg + msg2;
//          } while( msg2.size() );
//
//          cout << "socket: data->  " << msg.toStdString() << endl;
//          break;
//       }
//       }
//
//    }


} // end namespace

