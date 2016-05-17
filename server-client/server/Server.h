#ifndef SERVER_H___
#define SERVER_H___

#include <condition_variable>
#include <string>
#include <unordered_map>
#include <vector>
#include <mutex>

namespace by {

   // struct st_header {
   //    int type;
   //    int size;
   // };
   //
   // struct st_body_1 {
   //    std::string name;
   // };
   //
   // union msg {
   //    st_header header;
   //    st_body_1 body_1;
   // };

   class Server {
   public:
      //! Holds connected client name with its users 
      using Clients_Ct = std::unordered_map< std::string, std::vector<int> >;

      Server();

      ~Server();

      void start();

      void wait_for_clients_( int id );

   private:
      bool stop_;
      bool activity_;
      Clients_Ct clients_;
      std::mutex mutex_;
      std::condition_variable cv_;
      std::condition_variable cv_clients_;

      //! print client and users at end
      void dump_clients_() const;

   };

}

#endif


