#include "Client.h"
#include <QtCore/QCoreApplication>
#include <QtNetwork/QHostAddress>
#include <iostream>

using namespace std;

namespace core {

   Client::Client( QObject * parent )
   : QObject( parent ),
      socket_()
   {
      connect( &socket_, SIGNAL( connected() ), this, SLOT( connected_slot() ) );
      connect( &socket_, SIGNAL( disconnected() ), this, SLOT( disconnected_slot() ) );
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
   }

   void Client::disconnected_slot()
   {
      cout << "client: disconnected" << endl;
   }

}

