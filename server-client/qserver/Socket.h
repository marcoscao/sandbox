#ifndef QSERVER_SOCKET_H__
#define QSERVER_SOCKET_H__

#include "core/MsgManager.h"

#include <QtCore/QObject>
#include <QtCore/QRunnable>
#include <memory>

class QTcpSocket;

namespace core {

   struct userid_access_request_body;

   class Socket : public QObject, public QRunnable {
      Q_OBJECT
   public:
      Socket( QObject * parent = nullptr );
         
      ~Socket();

      void run();

      void set_descriptor( int id );

   public slots:
      void connected_slot();

      void disconnected_slot();

      void bytes_written_slot( qint64 );

      void ready_read_slot();

      void userid_access_request_slot( std::shared_ptr< userid_access_request_body >  );

   private:
      int descriptor_;
      QTcpSocket * qt_socket_;
      MsgManager msg_mngr_;

      void send_assigned_client_name_();

      void send_userid_authorization_( bool result, std::shared_ptr< userid_access_request_body > );
   };

}

#endif

