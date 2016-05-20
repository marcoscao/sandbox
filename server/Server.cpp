#include "Server.h"
#include "message.h"

#include <algorithm>
#include <chrono>
#include <ctime>
#include <functional>
#include <iostream>
#include <thread>

using namespace std;


namespace by {

   Server::Server()
   :  stop_( false ) ,
      activity_(false),
      clients_()
   {
   }

   Server::~Server()
   {
      dump_clients_();
      cout << "Server end" << endl;
   }

   void Server::wait_for_clients_( int id )
   {
      cout << "waiting_for_clients: START waiting for clients" << endl;

      // wait for clients until server stop
      std::unique_lock<mutex> l(mutex_);
      while( stop_ == false ) {
         cout << "inside wait..." << endl;
         cv_clients_.wait( l );
      
         cout << "waitin... after raise cv" << endl;

         if( stop_ )
            break;

         cout << "waiting_for_clients: received client" << endl;
         activity_ = true;
      }
   
      cout << "wait_for_clients: END waiting. Server has been stopped" << endl;
   }

   void Server::start()
   {
      cout << "Starting server..." << endl;   
      auto dt = chrono::system_clock::now();

//      function<void(int)> f ( std::bind( &Server::do_some, this, 42 ) );
      thread tr1 ( &Server::wait_for_clients_, this, 42 ); 
     
      unique_lock<mutex> lck( mutex_ );
      while( cv_.wait_for( lck, chrono::seconds(5)) == cv_status::timeout ) {
            cout << ". 5 seconds or connection reached" << endl;
            if( activity_ ) {
               activity_ = false;
            }
            else {
               // no activity in last 5 seconds, so exit
               cout << "no activity in 5 seconds. Stopping server" << endl;
               stop_ = true;
               lck.unlock();
               cv_clients_.notify_one();
               tr1.join();
               break;
            }
      }


      time_t t = chrono::system_clock::to_time_t( dt );
      cout << "Server end at : " << ctime( &t ) << endl;

      //tr1.join();
   }

   void Server::dump_clients_() const
   {
      for( auto const & it : clients_ ) {
         cout << "* Registered users for client: " << it.first << endl;
         std::copy( begin( it.second ), end( it.second ), ostream_iterator<int>( cout, "\n" ) );
      }
   }

}


