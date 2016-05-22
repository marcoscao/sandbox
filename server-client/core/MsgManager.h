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
#include <iostream>


namespace core {

   struct assign_client_name_body;
   struct userid_access_request_body;


   class MsgManager : public QObject { 
      Q_OBJECT

   public:
      MsgManager( );

      void set_descriptor( int );

      bool connect_to_host( QHostAddress const & host_addr, int server_port );

      /*
       * Emits signal with specific body, based on received message type ( header type )
       * Client classes connect to desired signals
       */
      void dispatch_message( );

      /*
       * @brief Calculates size of body to send, stores its value into its header and sends the message
       * @param msg Desired message to be send
       */
      template<typename T_MSG >
      void send_message( T_MSG & msg )
      {
         QByteArray ba;
         QDataStream ds( &ba, QIODevice::WriteOnly );

         ds << msg.body();
            
         QByteArray ba_all;
         QDataStream ds_all( &ba_all, QIODevice::WriteOnly );

         msg.header().body_size = ba.size();
         ds_all << msg.header() << msg.body();
      
         //cout << "ba_all size: " << ba_all.size() << endl;
         //cout << "ba_all contents: " << ba_all.toStdString() << endl;

         socket_->write( ba_all );
         socket_->waitForBytesWritten();
      }

   signals:
      void undefined_message_type_sig( Header );

      void assign_client_name_sig( std::shared_ptr< assign_client_name_body > );

      void userid_access_request_sig( std::shared_ptr< core::userid_access_request_body > );

      void userid_authorization_sig( std::shared_ptr< userid_authorization_body > );


   private slots:
      void connected_slot();

      void disconnected_slot();

      /*
       * data is available on the socket
       */
      void ready_read_slot();

      void bytes_written_slot( qint64 b );


   private:
      std::unique_ptr< QTcpSocket > socket_;

      /*
       * Reads body part from socket and builds its specific to be sent
       * Pending to read in chunks
       */
      template<typename T_BODY>
      std::shared_ptr< T_BODY > create_body( int body_size )
      {
         std::shared_ptr< T_BODY > p_st = std::make_shared< T_BODY >();
         QByteArray ba = socket_->read( body_size );

         QDataStream ds(ba);
         ds >> *p_st;

         return p_st;
      }
   };

} // end namespace


/*
 * Register types at global scope
 */

Q_DECLARE_METATYPE( core::assign_client_name_body );
Q_DECLARE_METATYPE( std::shared_ptr< core::assign_client_name_body> );
Q_DECLARE_METATYPE( core::userid_access_request_body );
Q_DECLARE_METATYPE( std::shared_ptr< core::userid_access_request_body> );

#endif

