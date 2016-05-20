#include "Client.h"
#include <algorithm>
#include <iostream>

using namespace std;

namespace by {

   Client::Client()
   : name_(),
      users_()         
   {
      populate_users_();
   }

   void Client::populate_users_()
   {
      // Reserve space
      users_.reserve( MAX_USERS );
      
      // Inserts users ids
      for( int i=0; i < MAX_USERS; ++i )
         users_[i] = i;

      // Shuffle users ids
      random_shuffle( begin(users_), end( users_ ) );

   }

   void Client::connect()
   {
      cout << "Connecting to server..." << endl;
   }

   void Client::request_user_access_() 
   {
      Users_Ct::value_type user_id = users_.back();
      users_.pop_back();
      
      // stores elapsed time for the request
   }

   void Client::received_name( std::string const & name )
   {
      name_ = name;
   }

}

