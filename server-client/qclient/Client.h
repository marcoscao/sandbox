#ifndef QCLIENT_CLIENT_H__
#define QCLIENT_CLIENT_H__

#include <QtCore/Qobject>
#include <QtNetwork/QTcpSocket>

class QTcpSocket;

namespace core {

   class Client : public QObject {
      Q_OBJECT
   public:
      Client( QObject * parent = nullptr );

      ~Client();

      void start( int server_port );
   
   public slots:
      void connected_slot();

      void disconnected_slot();

   signals:
      void quit_signal();

   private:
      QTcpSocket socket_;

   };

}

#endif


