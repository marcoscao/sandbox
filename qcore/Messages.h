#ifndef CORE_MESSAGES_H_
#define CORE_MESSAGES_H_

#include <QtCore/QString>


namespace core {

   enum MessageType {
      MSG_UNDEFINED = 0,
      MSG_ASSIGN_CLIENT_NAME = 1,
      MSG_USERID_ACCESS_REQUEST, 
		MSG_USERID_AUTHORIZATION,	
		MSG_CLIENT_FINISHED,	
		MSG_SERVER_SHUTDOWN	
   };


   struct Header 
   {
      int type;

   };

   QDataStream & operator<<( QDataStream & out, Header const & h );
   QDataStream & operator>>( QDataStream & out, Header & h );



   class MsgBase {
   public:

      explicit MsgBase( int type )
      : header_()
      {
         header_.type = type;
      }

	  const Header & header() const
	  {
		  return header_;
	  }

	  Header & header()
	  {
		  return header_;
	  }

   private:
      Header header;
   };




   template<typename T_BODY, int msg_type >
   class Msg : public MsgBase {
   public:
      Msg()
      : MsgBase( msg_type ),
	    body_()
      {
      }

	  const T_BODY & body() const
	  {
		  return body_;
	  }

	  T_BODY & body()
	  {
		  return body_;
	  }

   private:
      T_BODY body_;
   }

   template<typename T>
   QDataStream & operator<<( QDataStream & out, T const & msg )
   {
      out << msg.header();
      out << msg.body();
      return out;
   }

   template<typename T>
   QDataStream & operator>>( QDataStream & out, T & msg )
   {
      out >> msg.header();
      out >> msg.body();
      return out;
   }

//   class Msg 
//   {
//   public:
//      explicit Msg( int type )
//      : header()
//      {
//         header.type = type;
//      }
//
//	  const Header & header() const
//	  {
//		  return header_;
//	  }
//
//
//
//   private:
//      Header header;
//   };
//


//   struct AssignNameMsg : public Msg 
//   {   
//      AssignNameMsg() 
//      : Msg( MSG_ASSIGN_NAME ) {
//      }
//   
//      QString name;
//   };


   struct assign_name_body {
	   QString name;
   };

   using AssignNameMsg = Msg< assign_name_body, MSG_ASSIGN_CLIENT_NAME >;

   QDataStream & operator<<( QDataStream & out, assign_name_body const & );
   QDataStream & operator>>( QDataStream & out, assign_name_body & );

   QDataStream & operator<<( QDataStream & out, AssignNameMsg const & );
   QDataStream & operator>>( QDataStream & out, AssignNameMsg & );



//   struct RequestUserIdAccessMsg : public Msg 
//   {   
//      RequestUserIdAccessMsg() 
//      : Msg( MSG_USERID_REQUEST_ACCESS ) {
//      }
//   
//      int user_id;
//   };


   struct userid_access_request_body  
   {   
      int user_id;
	  QString client_dni;
   };

   using UserIdAccessRequestMsg = Msg< userid_access_request_body, MSG_USERID_ACCESS_REQUEST >;

   QDataStream & operator<<( QDataStream & out, userid_access_request_body const & );
   QDataStream & operator>>( QDataStream & out, userid_access_request_body & );

   QDataStream & operator<<( QDataStream & out, UserIdAccessRequestMsg const & );
   QDataStream & operator>>( QDataStream & out, UserIdAccessRequestMsg & );



   struct userid_authorization_body  
   {   
      int user_id;
	  bool authorized;
	  QString client_dni;
   };

   using UserIdAuthorizationdMsg = Msg< userid_authorization_body, MSG_USERID_AUTHORIZATION >;

   QDataStream & operator<<( QDataStream & out, userid_authorization_body const & );
   QDataStream & operator>>( QDataStream & out, userid_authorization_body & );

   QDataStream & operator<<( QDataStream & out, UserIdAuthorizationdMsg const & );
   QDataStream & operator>>( QDataStream & out, UserIdAuthorizationMsg & );




}


#endif


