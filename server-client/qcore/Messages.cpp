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


   QDataStream & operator<<( QDataStream & out, AssignNameMsg const & a )
   {
      out << a.name;
      return out;
   }

   QDataStream & operator>>( QDataStream & out, AssignNameMsg & a )
   {
      out >> a.name;
      return out;
   }

   QDataStream & operator<<( QDataStream & out, RequestUserIdAccessMsg const & a )
   {
      out << a.user_id;
      return out;
   }

   QDataStream & operator>>( QDataStream & out, RequestUserIdAccessMsg & a )
   {
      out >> a.user_id;
      return out;
   }

}


#endif


