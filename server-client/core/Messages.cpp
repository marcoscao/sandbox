#include "Messages.h"

#include <QtCore/QDataStream>
#include <QtCore/QString>

namespace core {

   QDataStream & operator<<( QDataStream & out, core::Header const & h )
   {
      out << h.type;
      out << h.body_size;
      return out;
   }

   QDataStream & operator>>( QDataStream & out, core::Header & h )
   {
      out >> h.type;
      out >> h.body_size;
      return out;
   }

   QDataStream & operator<<( QDataStream & out, core::assign_client_name_body const & st )
   {
      out << st.name;
      return out;
   }

   QDataStream & operator>>( QDataStream & out, core::assign_client_name_body & st )
   {
      out >> st.name;
      return out;
   }

   QDataStream & operator<<( QDataStream & out, core::userid_access_request_body const & st )
   {
      out << st.user_id;
      out << st.client_name;
      return out;
   }

   QDataStream & operator>>( QDataStream & out, core::userid_access_request_body & st )
   {
      out >> st.user_id;
      out >> st.client_name;
      return out;
   }

   QDataStream & operator<<( QDataStream & out, core::userid_authorization_body const & st )
   {
      out << st.user_id;
      out << st.authorized;
      out << st.client_name;
      return out;
   }

   QDataStream & operator>>( QDataStream & out, core::userid_authorization_body & st )
   {
      out >> st.user_id;
      out >> st.authorized;
      out >> st.client_name;
      return out;
   }

}




