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
      socket_(),
      timer_(),
      msg_mngr_(),
      assigned_name_("NOT-ASSIGNED")
   {
      connect( &socket_, SIGNAL( connected() ), this, SLOT( connected_slot() ) );
      connect( &socket_, SIGNAL( disconnected() ), this, SLOT( disconnected_slot() ) );
      connect( &socket_, SIGNAL( bytesWritten(qint64) ), this, SLOT( bytes_written_slot(qint64) ) );
      connect( &socket_, SIGNAL( readyRead() ), this, SLOT( ready_read_slot() ) );
   
      connect( &timer_, SIGNAL( timeout() ), this, SLOT( timeout_slot_() ) );


      // Connect signals for specific app messages received from server ( assigned_name, authorization, ... )
   
      connect( &msg_mngr_, SIGNAL( assign_client_name_sig( std::shared_ptr< assign_client_name_body > ) ), 
		  this, SLOT( assign_client_name_slot( std::shared_ptr< assign_client_name_body > ) ) );

      connect( &msg_mngr_, SIGNAL( userid_authorization_sig( std::shared_ptr< userid_authorization_body > ) ), 
		  this, SLOT( userid_authorization_slot( std::shared_ptr< userid_authorization_body > ) ) );


      // Populate and shuffle available users
      
      available_users_.reserve( MAX_AVAILABLE_USERS );
      for( int i=0; i < MAX_AVAILABLE_USERS; ++i )
         available_users_.push_back( i );

      random_shuffle( begin(available_users_), end(available_users_) );
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

      msg_mngr_.dispatch_message( socket_ );
   }

   void Client::bytes_written_slot( qint64 bytes )
   {
      //cout << "client: bytesWritten bytes: " << bytes << endl;
   }

   void Client::timeout_slot_()
   {
      if( available_users_.empty() ) {
         timer_.stop();
         cout << "client: no more user_ids. Stop sending messages" << endl;
         QCoreApplication::exit(0);
         return;
      }
      
      cout << "client: sending new user_id request for " << available_users_.back() << endl;

      UserIdAccessRequestMsg ua;

      ua.body().user_id = available_users_.back(); 
      ua.body().client_name = assigned_name_;

      MsgManager::send_message( ua, socket_ );
   }

   void Client::assign_client_name_slot( std::shared_ptr< assign_client_name_body > st )
   {
      assigned_name_ = st->name;
      cout << "client: assigned by server is -> " << assigned_name_.toStdString() << endl;
   }

   void Client::userid_authorization_slot( std::shared_ptr< userid_authorization_body > st )
   {
      cout << "client: user_id " << st->user_id << " has been ";
      
      if( st->authorized ) {
         cout << "AUTHORIZED" << endl;

         if( available_users_.back() != st->user_id ) {
            cout << "Oops! mismatch between current back() user_id and authorized user_id" << endl;
            return;
         }

         available_users_.pop_back();
      }
      else {
         cout << "REJECTED" << endl;
      }

   }

}

