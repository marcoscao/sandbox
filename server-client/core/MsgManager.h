/*
 * process received message ( header/body ) and send specific signal with the message body
 */

#ifndef CORE_MSGMANAGER_H__
#define CORE_MSGMANAGER_H__

#include "Messages.h"
#include <QtCore/QDataStream>
#include <QtCore/QEventLoop>
#include <QtCore/QObject>
#include <QtNetwork/QTcpSocket>
#include <memory>

class QTcpSocket;

namespace core {

   struct assign_client_name_body;
   struct userid_access_request_body;


   class MsgManager : public QObject { 
      Q_OBJECT
   public:
      MsgManager();

      /*
       * Emits signal with specific body, based on received message type ( header type )
       * Client classes connect to desired signals
       */
      void dispatch_message( QTcpSocket & );

      /*
       * Calculates size of body to send, stores its value into its header and sends the message
       */
      template<typename T_MSG >
      static void send_message( T_MSG & msg, QTcpSocket & socket )
      {
         QByteArray ba;
         QDataStream ds( &ba, QIODevice::WriteOnly );

         ds << msg.body();
            
         QByteArray ba_all;
         QDataStream ds_all( &ba_all, QIODevice::WriteOnly );

         msg.header().body_size = ba.size();
         ds_all << msg.header() << msg.body();
      
         //cout << "ba size : " << ba.size() << "  ba_all size: " << ba_all.size() << endl;
         //cout << "ba contents: " << ba.toStdString() << endl;
         //cout << endl;
         //cout << "ba_all contents: " << ba_all.toStdString() << endl;

         socket.write( ba_all );
         socket.waitForBytesWritten();
      }

   signals:
      void undefined_message_type_sig( Header );

      void assign_client_name_sig( std::shared_ptr< assign_client_name_body > );

      void userid_access_request_sig( std::shared_ptr< userid_access_request_body > );

      void userid_authorization_sig( std::shared_ptr< userid_authorization_body > );
   };

} // end namespace

// Q_DECLARE_METATYPE( core::assign_client_name_body );
// Q_DECLARE_METATYPE( std::shared_ptr< core::assign_client_name_body> );
// Q_DECLARE_METATYPE( core::userid_access_request_body );
// Q_DECLARE_METATYPE( std::shared_ptr< core::userid_access_request_body> );

#endif

