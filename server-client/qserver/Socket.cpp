#include "Socket.h"

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
   }

   void Socket::bytes_written_slot( qint64 b )
   {
      cout << "socket: bytes written: " << b << endl;
   }
}