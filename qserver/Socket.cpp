#include "Socket.h"
#include "Server.h"
#include "qcore/Messages.h"

#include <QtCore/QEventLoop>
#include <QtNetwork/QTcpSocket>
#include <iostream>

using namespace std;

namespace core {

   Socket::Socket( QObject * parent )
   : QObject( parent ),
      descriptor_(0),
      qt_socket_(0)
   {

      cout << "socket: starting socket..." << endl;
   }

   Socket::~Socket()
   {
      cout << "Socket destroyed" << endl;
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

      // Asign name to new client
      // Server::client_id = 5000;

      AssignNameMsg am;
      am.name = QString("client_dni_") + QString::number( Server::client_dni_counter++ );
      QByteArray ba;
      QDataStream ds( &ba, QIODevice::WriteOnly );

      ds << am.header;
      ds << am;

      qt_socket_->write( ba );

      qt_socket_->waitForBytesWritten();

      // QDataStream ds;
      // ds << am.header;
      // ds << 
      //
      //
      // QByteArray msg = "Hi guy! I'm the Walrus";
      // qt_socket_->write( msg );

      // enter event loop for this thread
      QEventLoop el;
      el.exec();

      //QString s("client talking");
      //qt_socket_->write( s.toUtf8().data() );
      //qt_socket_->flush();
      //qt_socket_->waitForBytesWritten();
   }

   void Socket::connected_slot()
   {
      cout << "socket: connected" << endl;
   }

   void Socket::disconnected_slot()
   {
      cout << "socket: disconnected" << endl;
   }

   void Socket::ready_read_slot()
   {
      cout << "socket: reading data..." << endl;

      cout << "  reading header..." << endl;

      QByteArray ba = qt_socket_->read( sizeof( Header) );
      QDataStream ds(ba);

      Header h;
      ds >> h;

      cout << "    - type: " << h.type << endl;

      switch( h.type ) {
      case MSG_USERID_REQUEST_ACCESS: 
      {
         QByteArray ba2 = qt_socket_->read( sizeof( RequestUserIdAccessMsg ) );
         QDataStream ds2(ba2);

         RequestUserIdAccessMsg ru;
         ds2 >> ru;

         cout << "Request user_id : " << ru.user_id << endl;
         break;
      }
      default:
      {
         QByteArray msg, msg2;
         do {
            msg2 = qt_socket_->read(1024);
            msg = msg + msg2;
         } while( msg2.size() );

         cout << "socket: data->  " << msg.toStdString() << endl;
         break;
      }
      }

   }

   void Socket::bytes_written_slot( qint64 b )
   {
      cout << "socket: bytes written: " << b << endl;
   }
}
