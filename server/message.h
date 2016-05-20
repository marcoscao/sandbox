#ifndef _CORE_MESSAGES_H__
#define _CORE_MESSAGES_H__

#include <string>

struct st_header {
   int type;
};

struct msg_client_connect {
   std::string name;
   int id;
};

struct msg_user_acces {
   std::string client_name;
   int user_id;
};


union message {
   st_header header;
   msg_client_connect body_1;
};


#endif

