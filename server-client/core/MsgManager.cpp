#include "MsgManager.h"
#include "Messages.h"

#include <QtCore/QDataStream>
#include <QtNetwork/QTcpSocket>
#include <iostream>

using namespace std;

namespace core {

   /*
    * Pending to read in chunks
    */
   template<typename T_BODY>
   std::shared_ptr< T_BODY > create_body( QTcpSocket & socket, int body_size )
   {
      std::shared_ptr< T_BODY > p_st = std::make_shared< T_BODY >();
      QByteArray ba = socket.read( body_size );

      QDataStream ds(ba);
      ds >> *p_st;

      return p_st;
   }


   MsgManager::MsgManager()
   {
      // qRegisterMetaType< core::assign_client_name_body >();
      // qRegisterMetaType< std::shared_ptr< assign_client_name_body > >();
      // qRegisterMetaType< core::userid_access_request_body >();
      // qRegisterMetaType< std::shared_ptr< userid_access_request_body > >();
   }

   void MsgManager::dispatch_message( QTcpSocket & socket )
   {
      //cout << "* Dispatching message..." << endl;

      // Read header. 
      // note: header uses 2 ints, ( type and body_size ) so real size is sizeof( Header ) which could be 8, 16, ...
      QByteArray ba = socket.read( sizeof( Header) );
      QDataStream ds(ba);

      Header h;
      ds >> h;

      switch( h.type ) {
         
         case MSG_ASSIGN_CLIENT_NAME : 
            emit assign_client_name_sig( create_body<assign_client_name_body>( socket, h.body_size ) ); 
            break;

         case MSG_USERID_ACCESS_REQUEST	: 
            emit userid_access_request_sig( create_body<userid_access_request_body>( socket, h.body_size ) ); 
            break;

         case MSG_USERID_AUTHORIZATION : 
            emit userid_authorization_sig( create_body<userid_authorization_body>( socket, h.body_size ) ); 
         //   emit userid_authorization_sig(); 
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

} // end namespace

