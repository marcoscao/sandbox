#include "MsgManager.h"
#include "Messages.h"

#include <QtCore/QDataStream>
#include <QtNetwork/QTcpSocket>
#include <iostream>

using namespace std;

namespace core {

   MsgManager::MsgManager( )
   : socket_()
   {
      qRegisterMetaType< assign_client_name_body >();
      qRegisterMetaType< std::shared_ptr< assign_client_name_body > >();
      qRegisterMetaType< userid_access_request_body >();
      qRegisterMetaType< std::shared_ptr< core::userid_access_request_body > >();
      
      socket_ = std::make_unique< QTcpSocket >(); 

      connect( socket_.get(), SIGNAL( connected() ), this, SLOT( connected_slot() ) );
      connect( socket_.get(), SIGNAL( disconnected() ), this, SLOT( disconnected_slot() ) );
      connect( socket_.get(), SIGNAL( bytesWritten(qint64) ), this, SLOT( bytes_written_slot(qint64) ) );
      connect( socket_.get(), SIGNAL( readyRead() ), this, SLOT( ready_read_slot() ) );
   }

   void MsgManager::set_descriptor( int d )
   {
      socket_->setSocketDescriptor( d );
   }

   bool MsgManager::connect_to_host( QHostAddress const & host_addr, int server_port )
   {
      socket_->connectToHost( host_addr, server_port );

      if( socket_->waitForConnected( 3000 ) == false ) {
         cout << "timeout trying to connect to host and port: " << server_port << endl;
         //emit quit_signal();
         return false; 
      }

      return true;
   }

   void MsgManager::dispatch_message( )
   {
      //cout << "* Dispatching message..." << endl;

      // Read header. 
      // note: header uses 2 ints, ( type and body_size ) so real size is sizeof( Header ) which could be 8, 16, ...
      QByteArray ba = socket_->read( sizeof( Header) );
      QDataStream ds(ba);

      Header h;
      ds >> h;

      switch( h.type ) {
         
         case MSG_ASSIGN_CLIENT_NAME : 
            emit assign_client_name_sig( create_body<assign_client_name_body>( h.body_size ) ); 
            break;

         case MSG_USERID_ACCESS_REQUEST	: 
            emit userid_access_request_sig( create_body<core::userid_access_request_body>( h.body_size ) ); 
            break;

         case MSG_USERID_AUTHORIZATION : 
            emit userid_authorization_sig( create_body<userid_authorization_body>( h.body_size ) ); 
            break;
         
         case MSG_CLIENT_FINISHED : 
           // emit client_finished_sig(); 
            break;
         
         case MSG_SERVER_SHUTDOWN : 
           // emit server_shutdown_sig(); 
            break;
       
         default:
            emit undefined_message_type_sig( h ); 
            break;
      }
   }

   void MsgManager::connected_slot()
   {
      cout << "socket: client connected" << endl;
   }

   void MsgManager::disconnected_slot()
   {
      cout << "socket: client disconnected" << endl;
   }

   void MsgManager::ready_read_slot()
   {
      //cout << "socket: reading client data..." << endl;
      dispatch_message();
   }

   void MsgManager::bytes_written_slot( qint64 b )
   {
      //cout << "socket: bytes written: " << b << endl;
   }

} // end namespace

