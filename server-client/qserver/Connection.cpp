#include "Connection.h"
#include "Server.h"
#include "core/Messages.h"

#include <QtCore/QDataStream>
#include <QtCore/QEventLoop>
#include <QtCore/QThread>
#include <QtNetwork/QTcpSocket>
#include <iostream>

using namespace std;

namespace core {

   Connection::Connection( QObject * parent )
   : QObject( parent ),
      descriptor_(0),
      msg_mngr_(0)
   {
      cout << "connection: starting connection..." << endl;
   }

   Connection::~Connection()
   {
      cout << "Connection destructor" << endl;
   }

   void Connection::set_descriptor( int connection_id )
   {
      descriptor_ = connection_id;
   }

   void Connection::run()
   {
      cout << "connection: running connection thread..." << endl;

      if( descriptor_ == false ) {
         cout << "connection: no valid descriptor" << endl;
         return;
      }

      msg_mngr_ = new MsgManager( );
      msg_mngr_->set_descriptor( descriptor_ );

      // connect message sent by client to request user-id access
      connect( msg_mngr_, SIGNAL( userid_access_request_sig( std::shared_ptr< core::userid_access_request_body > ) ), 
		  this, SLOT( userid_access_request_slot( std::shared_ptr< core::userid_access_request_body > ) ), Qt::QueuedConnection );

      // send new assigned name to the client
      send_assigned_client_name_();

      // enter event loop for this thread
      QEventLoop el;
      el.exec();

      cout << "after event loop" << endl;
   }

   void Connection::userid_access_request_slot( std::shared_ptr< core::userid_access_request_body > st )
   {
      // Trying to register business user_id per client_name 
      bool result = Server::instance().register_business_userid( st->client_name, st->user_id );

      cout << "server-connection: registering business user_id result : " << ( result ? "TRUE" : "FALSE" );
      cout << "  for client_name -> " << st->client_name.toStdString();
      cout << " and user_id : " << st->user_id << endl;

      // answer true/false
      send_userid_authorization_( result, st ); 
   }
   
   void Connection::send_assigned_client_name_()
   {
      QString connected_client_name = QString("[Client_DNI_") + QString::number( Server::instance().new_client_dni() ) + QString("]");

      AssignNameMsg am;
      am.body().name = connected_client_name;

      // sleep to simulate server heavy load
      QThread::msleep( 3200 );
      msg_mngr_->send_message( am );

      // Register business client_name
      bool result = Server::instance().register_business_client( am.body().name );
      cout << "  server-connection: registering business client result : " << result << endl;
   }

   void Connection::send_userid_authorization_( bool result, std::shared_ptr< userid_access_request_body > st )
   {
      cout << "server-connection: sending user_id authorization" << endl;

      UserIdAuthorizationMsg ua;
      ua.body().authorized = result;
      ua.body().user_id = st->user_id;
      ua.body().client_name = st->client_name;

      QThread::msleep( 2600 );
      msg_mngr_->send_message( ua );
   }
}
