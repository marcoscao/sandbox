#include "Client.h"
#include "qcore/Messages.h"

#include <QtCore/QCoreApplication>
#include <QtNetwork/QHostAddress>
#include <iostream>

using namespace std;

namespace core {

   Client::Client( QObject * parent )
   : QObject( parent ),
      socket_(),
      timer_()
   {
      connect( &socket_, SIGNAL( connected() ), this, SLOT( connected_slot() ) );
      connect( &socket_, SIGNAL( disconnected() ), this, SLOT( disconnected_slot() ) );
      connect( &socket_, SIGNAL( bytesWritten(qint64) ), this, SLOT( bytes_written_slot(qint64) ) );
      connect( &socket_, SIGNAL( readyRead() ), this, SLOT( ready_read_slot() ) );
   
      connect( &timer_, SIGNAL( timeout() ), this, SLOT( timeout_slot_() ) );
   }

   Client::~Client() 
   {
      cout << "Client: destroyed" << endl;
   }

   void Client::start( int server_port )
   {
      socket_.connectToHost( QHostAddress::LocalHost, server_port );
      if( socket_.waitForConnected( 2000 ) == false ) {
         cout << "timeout trying to connect to server port: " << server_port << endl;
         emit quit_signal();
         QCoreApplication::instance()->quit();
      }
   }

   void Client::connected_slot()
   {
      cout << "client: connected" << endl;
      timer_.start( 1000 );
   }

   void Client::disconnected_slot()
   {
      cout << "client: disconnected" << endl;
   }

   void Client::ready_read_slot()
   {
      cout << "client: ready_read" << endl;

      cout << "  reading header..." << endl;

      QByteArray ba = socket_.read( sizeof( Header) );
      QDataStream ds(ba);

      Header h;
      ds >> h;

      cout << "    - type: " << h.type << endl;

      switch( h.type ) {
      case MSG_ASSIGN_NAME: 
      {
         QByteArray ba2 = socket_.read( sizeof( AssignNameMsg ) );
         QDataStream ds2(ba2);

         AssignNameMsg ru;
         ds2 >> ru;

         cout << "Assigned name by server is : " << ru.name.toStdString() << endl;
         break;
      }
      default:
      {
         QByteArray msg, msg2;
         do {
            msg2 = socket_.read(1024);
            msg = msg + msg2;
         } while( msg2.size() );

         cout << "socket: data->  " << msg.toStdString() << endl;
         break;
      }
      }

      
      QByteArray msg;
      QByteArray msg2;
      
      do {
         msg2 = socket_.read( 1024 );
         msg = msg + msg2;
      } while( msg2.size() );

      cout << "client: data received: " << msg.toStdString() << endl;
   }

   void Client::bytes_written_slot( qint64 bytes )
   {
      cout << "client: bytesWritten bytes: " << bytes << endl;
   }

   void Client::timeout_slot_()
   {
      cout << "client: sending message" << endl;

      //AssignNameMsg am;
      //am.name = "hey!!!!!!!!!! testing";

      RequestUserIdAccessMsg am;
      am.user_id = 42;

      QByteArray ba;
      QDataStream ds( &ba, QIODevice::WriteOnly );

      ds << am.header;
      ds << am;

      socket_.write( ba );

      socket_.waitForBytesWritten();
   }

}

