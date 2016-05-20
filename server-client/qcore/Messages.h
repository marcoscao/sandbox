#ifndef CORE_MESSAGES_H_
#define CORE_MESSAGES_H_

#include <QtCore/QString>


namespace core {

   enum MessageType {
      MSG_UNDEFINED = 0,
      MSG_ASSIGN_NAME = 1,
      MSG_USERID_REQUEST_ACCESS = 2
   };


   struct Header 
   {
      int type;

   };

   QDataStream & operator<<( QDataStream & out, Header const & h );
   QDataStream & operator>>( QDataStream & out, Header & h );


   struct Msg 
   {
      Header header;

      explicit Msg( int type )
      : header()
      {
         header.type = type;
      }
   };



   struct AssignNameMsg : public Msg 
   {   
      AssignNameMsg() 
      : Msg( MSG_ASSIGN_NAME ) {
      }
   
      QString name;
   };

   QDataStream & operator<<( QDataStream & out, AssignNameMsg const & a );
   QDataStream & operator>>( QDataStream & out, AssignNameMsg & a );



   struct RequestUserIdAccessMsg : public Msg 
   {   
      RequestUserIdAccessMsg() 
      : Msg( MSG_USERID_REQUEST_ACCESS ) {
      }
   
      int user_id;
   };

   QDataStream & operator<<( QDataStream & out, RequestUserIdAccessMsg const & a );
   QDataStream & operator>>( QDataStream & out, RequestUserIdAccessMsg & a );

}


#endif


