#include "Server.h"
#include "Socket.h"

#include <QtCore/QThreadPool>
#include <iostream>

using namespace std;

namespace core {

   int Server::client_dni_counter = 5000;

   Server::Server( QObject * parent )
   : QTcpServer( parent ),
      threadpool_(0)
   {
      threadpool_ = new QThreadPool( this );
      threadpool_->setMaxThreadCount(5);
   }

   Server::~Server() 
   {
      cout << "Server destroyed" << endl;
   }

   void Server::start()
   {
      //time_t t = chrono::system_clock::to_time_t( dt );
      //cout << "Server end at : " << ctime( &t ) << endl;

      //qt_server_ = std::make_unique< QTcpServer >( this );
      //connect( this, SIGNAL(newConnection()), this, SLOT( new_connection_slot() ) );

      listen();

      cout << "Server starting to listen on address " << serverAddress().toString().toStdString();
      cout << " and port " << serverPort() << endl;
   }

   // void Server::new_connection_slot()
   // {
   //    cout << "server: received new connection" << endl;
   //    // Starts new socket client in a new thread
   // }

   void Server::incomingConnection( qintptr handle )
   {
      cout << "server: incomingConnection " << endl;

      Socket * s = new Socket();
      s->setAutoDelete(true);
      s->set_descriptor( handle );

      threadpool_->start( s );
   }

}


