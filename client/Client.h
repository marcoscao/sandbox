#ifndef _CLIENT_H__
#define _CLIENT_H__

#include <string>
#include <vector>

namespace by {

   class Client {
   public:
      static int const MAX_USERS = 250;

      Client();

      ~Client() = default;

      void connect();

      void request_user_access_(); 

      void received_name( std::string const & name );

   private:
      using Users_Ct = std::vector< int >;

      std::string name_;
      Users_Ct users_;

      void populate_users_(); 
   };

}

#endif

