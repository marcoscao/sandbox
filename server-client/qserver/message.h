#ifndef CORE_MESSAGES_H__
#define CORE_MESSAGES_H__

#include <string>

namespace core {

   struct st_header {
      int type;
   };

   struct set_name_body {
      std::string name;
      int server_id;
   };

   struct msg_user_acces {
      std::string client_name;
      int user_id;
   };


   union message {
      st_header header;
      union {
         set_name_body body_1;
         user_acces body_2;
      }
   };

}

#endif

