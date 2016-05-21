#include "Socket.h"
#include "Server.h"
#include "core/Messages.h"

#include <QtCore/QDataStream>
#include <QtCore/QEventLoop>
#include <QtNetwork/QTcpSocket>
#include <iostream>

using namespace std;

namespace core {

   Socket::Socket( QObject * parent )
   : QObject( parent ),
      descriptor_(0),
      qt_socket_(0),
      msg_mngr_()
   {

      cout << "socket: starting socket..." << endl;
	  
      connect( &msg_mngr_, SIGNAL( userid_access_request_sig( std::shared_ptr< userid_access_request_body > ) ), 
		  this, SLOT( userid_access_request_slot( std::shared_ptr< userid_access_request_body > ) ) );
   }

   Socket::~Socket()
   {
      cout << "Socket destructor" << endl;
   }

   void Socket::set_descriptor( int socket_id )
   {
      descriptor_ = socket_id;
   }

   void Socket::run()
   {
      cout << "socket: running socket thread..." << endl;

      if( descriptor_ == false ) {
         cout << "socket: no valid descriptor" << endl;
         return;
      }

      qt_socket_ = new QTcpSocket();

      connect( qt_socket_, SIGNAL( connected() ), this, SLOT( connected_slot() ) );
      connect( qt_socket_, SIGNAL( disconnected() ), this, SLOT( disconnected_slot() ) );
      connect( qt_socket_, SIGNAL( bytesWritten(qint64) ), this, SLOT( bytes_written_slot(qint64) ) );
      connect( qt_socket_, SIGNAL( readyRead() ), this, SLOT( ready_read_slot() ) );
     
      qt_socket_->setSocketDescriptor( descriptor_ );

      // send new assigned name to the client
      send_assigned_client_name_();

      // enter event loop for this thread
      QEventLoop el;
      el.exec();

      cout << "after event loop" << endl;
   }

   void Socket::connected_slot()
   {
      cout << "server-socket: client connected" << endl;
   }

   void Socket::disconnected_slot()
   {
      cout << "server-socket: client disconnected" << endl;
   }

   void Socket::ready_read_slot()
   {
      cout << "server-socket: reading client data..." << endl;

      //cout << "  reading header..." << endl;

      msg_mngr_.dispatch_message( *qt_socket_ );

      return;





      // QByteArray ba = qt_socket_->read( sizeof( Header) );
      // QDataStream ds(ba);
      //
      // Header h;
      // ds >> h;
      //
      // cout << "    - type: " << h.type << endl;
      //
      // switch( h.type ) {
      // case MSG_USERID_ACCESS_REQUEST: 
      // {
      //    QByteArray ba2 = qt_socket_->read( sizeof( UserIdAccesRequestMsg ) );
      //    QDataStream ds2(ba2);
      //
      //    RequestUserIdAccessMsg ru;
      //    ds2 >> ru;
      //
      //    cout << "Request user_id : " << ru.user_id << endl;
      //    break;
      // }
      // default:
      // {
      //    QByteArray msg, msg2;
      //    do {
      //       msg2 = qt_socket_->read(1024);
      //       msg = msg + msg2;
      //    } while( msg2.size() );
      //
      //    cout << "socket: data->  " << msg.toStdString() << endl;
      //    break;
      // }
      // }

   }

   void Socket::bytes_written_slot( qint64 b )
   {
      //cout << "socket: bytes written: " << b << endl;
   }

   void Socket::userid_access_request_slot( std::shared_ptr< userid_access_request_body > st )
   {
      // Trying to register business user_id per client_name 
      bool result = Server::instance().register_business_userid( st->client_name, st->user_id );

      cout << "server-socket: registering business user_id result : " << ( result ? "TRUE" : "FALSE" );
      cout << "  for client_name -> " << st->client_name.toStdString();
      cout << " and user_id : " << st->user_id << endl;

      // answer true/false
      send_userid_authorization_( result, st ); 
   }
   
   void Socket::send_assigned_client_name_()
   {
      QString connected_client_name = QString("[Client_DNI_") + QString::number( Server::new_client_dni() ) + QString("]");

      AssignNameMsg am;
      am.body().name = connected_client_name;

      MsgManager::send_message( am, *qt_socket_ );

      // Register business client_name
      bool result = Server::instance().register_business_client( am.body().name );
      cout << "  server-socket: registering business client result : " << result << endl;
   }

   void Socket::send_userid_authorization_( bool result, std::shared_ptr< userid_access_request_body > st )
   {
      cout << "server-socket: sending user_id authorization" << endl;

      UserIdAuthorizationMsg ua;
      ua.body().authorized = result;
      ua.body().user_id = st->user_id;
      ua.body().client_name = st->client_name;

      MsgManager::send_message( ua, *qt_socket_ );
   }
}
