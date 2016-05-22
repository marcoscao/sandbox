#include "Client.h"
#include "core/Messages.h"

#include <QtCore/QCoreApplication>
#include <QtCore/QDataStream>
#include <QtNetwork/QHostAddress>
#include <iostream>
#include <random>

using namespace std;

namespace core {

   Client::Client( QObject * parent )
   : QObject( parent ),
      timer_(),
      msg_mngr_(),
      assigned_name_("NOT-ASSIGNED"),
      available_users_(),
      send_user_counter_it_(),
      received_user_counter_it_()
   {   
      connect( &timer_, SIGNAL( timeout() ), this, SLOT( timeout_slot_() ) );

      // connects server message "assign-name" 
      connect( &msg_mngr_, SIGNAL( assign_client_name_sig( std::shared_ptr< assign_client_name_body > ) ), 
		  this, SLOT( assign_client_name_slot( std::shared_ptr< assign_client_name_body > ) ) );

      // connects server message "user-id-autho-response"
      connect( &msg_mngr_, SIGNAL( userid_authorization_sig( std::shared_ptr< userid_authorization_body > ) ), 
		  this, SLOT( userid_authorization_slot( std::shared_ptr< userid_authorization_body > ) ) );

      // Populate and shuffle available users
      initialize_users_(); 
   }

   Client::~Client() 
   {
      cout << "Client: destructor" << endl;
   }

   void Client::start( int server_port )
   {
      if( msg_mngr_.connect_to_host( QHostAddress::LocalHost, server_port ) == false ) {
          emit quit_signal();
          QCoreApplication::instance()->quit();
      }
   }


   // void Client::ready_read_slot()
   // {
   //    //cout << "client: ready_read" << endl;
   //    msg_mngr_.dispatch_message( );
   // }
   //
   // void Client::bytes_written_slot( qint64 bytes )
   // {
   //    //cout << "client: bytesWritten bytes: " << bytes << endl;
   // }

   void Client::timeout_slot_()
   {
      if( send_user_counter_it_ == available_users_.end() ) { //available_users_.empty() ) {
         cout << "client: no more user_ids. Stop sending request messages" << endl;
         timer_.stop();
         return;
      }
     
      // increments counter of sent users
      UsersCt::iterator it = send_user_counter_it_++;

      // stores current time for this request
      it.value() = QTime::currentTime();

      cout << "client: sending new user_id request for " << it.key() << endl;

      UserIdAccessRequestMsg ua;
      ua.body().user_id = it.key();
      ua.body().client_name = assigned_name_;

      // send message to server
      msg_mngr_.send_message( ua);
   }

   void Client::assign_client_name_slot( std::shared_ptr< assign_client_name_body > st )
   {
      assigned_name_ = st->name;
      cout << "client: assigned by server is -> " << assigned_name_.toStdString() << endl;
      cout << endl;
      cout << "client: starting to send user_id request every second" << endl << endl;
      timer_.start( 1000 );
   }

   void Client::userid_authorization_slot( std::shared_ptr< userid_authorization_body > st )
   {
      UsersCt::iterator it = available_users_.find( st->user_id );
      if( it == available_users_.end() ) {
         cout << "Oops! Something wrong no user_id " << st->user_id << endl;
         return;
      }

      QTime now = QTime::currentTime();
      int elapsed_ms = it.value().msecsTo( now );

      cout << "client: user_id " << st->user_id << " has been " << ( st->authorized ? "AUTHORIZED" : "REJECTED" );
      cout << " -> message elapsed time: " << ( elapsed_ms / 1000.0 ) << " seconds" << endl;

      // increments received authorizations and check whether reachs the end()
      if( ++received_user_counter_it_ == available_users_.end() ) {
         cout << "client: all pending user_ids have been received. Client has finished its job" << endl;
         QCoreApplication::exit(0);
      }
   }

   void Client::initialize_users_()
   {
      // temporary vector to shuffle user_ids
      using Ct = std::vector< int >;
      Ct v;
      v.reserve( MAX_AVAILABLE_USERS );

      for( int i=0; i < MAX_AVAILABLE_USERS; ++i )
         v.push_back( i+1 );

      random_shuffle( begin( v ), end( v ) );

      // populate hash map
      available_users_.reserve( MAX_AVAILABLE_USERS );
      std::for_each( begin( v ), end( v ),
            [this]( Ct::value_type val ) { available_users_.insert( val, QTime() ); } );

      send_user_counter_it_ = available_users_.begin();
      received_user_counter_it_ = available_users_.begin();
   }

}

