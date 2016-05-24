/*
 * Connection class.   
 * Runnable ( thread from threadpool ), manages a live connection with a connected client  
 *
 * @author: marcos santamaria cao
 */

#ifndef QSERVER_SOCKET_H__
#define QSERVER_SOCKET_H__

#include "core/MsgManager.h"

#include <QtCore/QObject>
#include <QtCore/QRunnable>
#include <memory>

namespace core {

   struct userid_access_request_body;

   class Connection : public QObject, public QRunnable {
      Q_OBJECT
   public:
      Connection( QObject * parent = nullptr );
         
      ~Connection();

      void run();

      void set_descriptor( int id );

   public slots:
      void userid_access_request_slot( std::shared_ptr< core::userid_access_request_body > );

   private:
      int descriptor_;
      MsgManager * msg_mngr_;

      void send_assigned_client_name_();

      void send_userid_authorization_( bool result, std::shared_ptr< userid_access_request_body > );
   };
}

#endif

