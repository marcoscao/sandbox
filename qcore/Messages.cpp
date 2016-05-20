#ifndef CORE_MESSAGES_H_
#define CORE_MESSAGES_H_

#include "Messages.h"
#include <QtCore/QDataStream>
#include <QtCore/QString>


namespace core {


   QDataStream & operator<<( QDataStream & out, Header const & h )
   {
      out << h.type;
      return out;
   }

   QDataStream & operator>>( QDataStream & out, Header & h )
   {
      out >> h.type;
      return out;
   }


   QDataStream & operator<<( QDataStream & out, assign_name_body const & st )
   {
      out << st.name;
      return out;
   }

   QDataStream & operator>>( QDataStream & out, assign_name_body & st )
   {
      out >> st.name;
      return out;
   }

   QDataStream & operator<<( QDataStream & out, userid_access_request_body const & st )
   {
      out << st.user_id;
      out << st.client_dni;
      return out;
   }

   QDataStream & operator>>( QDataStream & out, userid_access_request_body & st )
   {
      out >> st.user_id;
      out >> st.client_dni;
      return out;
   }

   QDataStream & operator<<( QDataStream & out, userid_authorization_body const & st )
   {
      out << st.user_id;
      out << st.authorized;
      out << st.client_dni;
      return out;
   }

   QDataStream & operator>>( QDataStream & out, userid_access_authorization_body & st )
   {
      out >> st.user_id;
      out >> st.authorized;
      out >> st.client_dni;
      return out;
   }


//   QDataStream & operator<<( QDataStream & out, AssignNameMsg const & a )
//   {
//      out << a.header();
//      out << a.body();
//      return out;
//   }
//
//   QDataStream & operator>>( QDataStream & out, AssignNameMsg & a )
//   {
//      out >> a.header();
//      out >> a.body();
//      return out;
//   }

//   QDataStream & operator<<( QDataStream & out, RequestUserIdAccessMsg const & r )
//   {
//      out << r.header();
//      return out;
//   }
//
//   QDataStream & operator>>( QDataStream & out, RequestUserIdAccessMsg & r )
//   {
//      out >> a.user_id;
//      return out;
//   }

}


#endif


