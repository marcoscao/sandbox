#ifndef QCLIENT_CLIENT_H__
#define QCLIENT_CLIENT_H__

#include <QtCore/QTimer>
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

      void bytes_written_slot( qint64 );

      void ready_read_slot();

      void timeout_slot_();

   signals:
      void quit_signal();

   private:
      QTcpSocket socket_;
      QTimer timer_;
   };

}

#endif


