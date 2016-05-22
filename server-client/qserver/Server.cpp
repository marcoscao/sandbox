#include "Server.h"
#include "Connection.h"

#include <QtCore/QCoreApplication>
#include <QtCore/QFile>
#include <QtCore/QThreadPool>
#include <iostream>

using namespace std;

namespace core {

   QAtomicInt Server::client_dni_counter_ = 5000;

   Server::Server( QObject * parent )
   : QTcpServer( parent ),
      threadpool_(0),
      inactivity_timer_()
   {
      threadpool_ = new QThreadPool( this );
      threadpool_->setMaxThreadCount(5);

      connect( &inactivity_timer_, SIGNAL( timeout() ), this, SLOT( inactivity_timeout_slot_() ) );
   }

   Server::~Server() 
   {
      cout << "Server destroyed" << endl;
   }

   Server & Server::instance()
   {
      static Server s;
      return s;
   }

   int Server::new_client_dni()
   {
      // atomic operation. returns old value
      return client_dni_counter_++;
   }

   void Server::start()
   {
      listen();

      cout << "Starting server..." << endl;

      cout << "note: server will shutdown automatically after an inactivity period of " << INACTIVITY_TIMEOUT / 1000 << " seconds" << endl;
      //cout << " -> active threads: " << threadpool_->activeThreadCount() << endl;

      cout << endl;

      cout << "Server starting to listen on address " << serverAddress().toString().toStdString();
      cout << " and port " << serverPort() << endl;

      inactivity_timer_.start( INACTIVITY_TIMEOUT );
   }

   bool Server::register_business_client( QString const & client_name )
   {
      QMutexLocker lock( &mutex_ );
      auto it = business_clients_.insert( BusinessClientsCt::value_type( client_name.toStdString(), BusinessUsersCt() ) );

      reset_inactivity_timer(); 
      return it.second;
   }

   bool Server::register_business_userid( QString const & client_name, int user_id )
   {
      auto it = business_clients_.find( client_name.toStdString() );
      if( it == business_clients_.end() )
         return false;

      QMutexLocker lock( &mutex_ );
      auto it_u = it->second.insert( BusinessUsersCt::value_type( user_id ) );
      
      reset_inactivity_timer(); 
      return it_u.second;
   }

   void Server::reset_inactivity_timer()
   {
      inactivity_timer_.start( INACTIVITY_TIMEOUT );
   }

   void Server::incomingConnection( qintptr handle )
   {
      cout << "server: incomingConnection " << endl;

      Connection * c = new Connection();
      c->setAutoDelete(true);
      c->set_descriptor( handle );

      QMutexLocker lock( &mutex_ );
      threadpool_->start( c );
      reset_inactivity_timer(); 
   }

   void Server::inactivity_timeout_slot_()
   {
      cout << "Reach inactivity timeout. Stop server" << endl;
      
      QMutexLocker lock( &mutex_ );
      inactivity_timer_.stop();

      threadpool_->clear();

      cout << "stopping threads" << endl;
      cout << "dumping registered clients and users" << endl;

      dump_business_clients_();

      cout << endl;
      cout << "Shutdown Server." << endl;
      QCoreApplication::exit( 0 );
   }

   void Server::dump_business_clients_()
   {
      QFile f("server_log.log");
      if( f.open( QIODevice::WriteOnly | QIODevice::Text ) == false ) {
         cout << "server: error trying to create log file" << endl;
         return;
      }

      QTextStream ts( &f );

      std::for_each( begin(business_clients_), end(business_clients_), 
            [&ts]( BusinessClientsCt::value_type const & kv ) 
            {
               ts << "client: " << kv.first.c_str() << endl;

               std::for_each( begin(kv.second), end(kv.second),
                  [&ts]( BusinessUsersCt::value_type const & v ) 
                  {
                     ts << "  user id : " << v << endl;
                  }
               );
            }
      );

      f.close();
   }
}


