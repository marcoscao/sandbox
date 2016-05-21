#ifndef QCLIENT_CLIENT_H__
#define QCLIENT_CLIENT_H__

#include "core/MsgManager.h"

#include <QtCore/QTimer>
#include <QtNetwork/QTcpSocket>
#include <memory>

class QTcpSocket;

namespace core {

   struct assign_client_name_body;


   class Client : public QObject {
      Q_OBJECT
   public:
      Client( QObject * parent = nullptr );

      ~Client();

      void start( int server_port );
   
   public slots:
      void connected_slot();

      void disconnected_slot();

      void bytes_written_slot( qint64 );

      void ready_read_slot();

      void timeout_slot_();

      void assign_client_name_slot( std::shared_ptr< assign_client_name_body >  );

      void userid_authorization_slot( std::shared_ptr< userid_authorization_body > );

   signals:
      void quit_signal();

   private:
      using UsersCt = std::vector< int >;

      static const int MAX_AVAILABLE_USERS = 15;

      QTcpSocket socket_;
      QTimer timer_;
      MsgManager msg_mngr_;
      QString assigned_name_;
      UsersCt available_users_;
   };

}

#endif


