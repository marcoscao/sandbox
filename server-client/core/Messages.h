/*
 * Declares and defines the Header and Bodies for the different messages
 *
 * @author: marcos santamaria cao
 */

#ifndef CORE_MESSAGES_H_
#define CORE_MESSAGES_H_

#include <QtCore/QString>


namespace core {

   /*
    * Messages types identifiers
    */
   enum MessageType {
      MSG_UNDEFINED = 0,
      MSG_ASSIGN_CLIENT_NAME = 1,
      MSG_USERID_ACCESS_REQUEST, 
      MSG_USERID_AUTHORIZATION,	
      MSG_CLIENT_FINISHED,	
      MSG_SERVER_SHUTDOWN	
   };

   /*
    * Header part of the message
    * type  : message type ( MessageType enum value )
    */
   struct Header 
   {
      int type;
      int body_size;
   };

   QDataStream & operator<<( QDataStream & out, Header const & );
   QDataStream & operator>>( QDataStream & out, Header & );


   /*
    * message base class, not to usually be used directly
    */
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
      Header header_;
   };



   /*
    * Template to define and declare a complete message
    *
    * Sets the body struct to be used as well as the message type
    */
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

      template<typename>
      friend QDataStream & operator<<( QDataStream & out, T_BODY const & msg );

   private:
      T_BODY body_;
   };


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


   /*
    * body-part to assign a name to the connected client
    */
   struct assign_client_name_body {
      QString name;
   };

   using AssignNameMsg = Msg< assign_client_name_body, MSG_ASSIGN_CLIENT_NAME >;

   QDataStream & operator<<( QDataStream & out, assign_client_name_body const & );
   QDataStream & operator>>( QDataStream & out, assign_client_name_body & );


   /*
    * body part to request for user id access
    */
   struct userid_access_request_body  
   {   
      int user_id;
      QString client_name;

   };

   using UserIdAccessRequestMsg = Msg< userid_access_request_body, MSG_USERID_ACCESS_REQUEST >;

   QDataStream & operator<<( QDataStream & out, core::userid_access_request_body const & );
   QDataStream & operator>>( QDataStream & out, core::userid_access_request_body & );


   /*
    * body part to response a user-id access request
    */
   struct userid_authorization_body  
   {   
      int user_id;
      bool authorized;
      QString client_name;
   };

   using UserIdAuthorizationMsg = Msg< userid_authorization_body, MSG_USERID_AUTHORIZATION >;

   QDataStream & operator<<( QDataStream & out, userid_authorization_body const & );
   QDataStream & operator>>( QDataStream & out, userid_authorization_body & );

}


#endif


