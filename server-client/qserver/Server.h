#ifndef QSERVER_SERVER_H__
#define QSERVER_SERVER_H__

#include <QtNetwork/QTcpServer>

class QThreadPool;

namespace core {

   class Server : public QTcpServer {
      Q_OBJECT
   public:
      static int client_dni_counter;

      Server( QObject * parent = nullptr );
      
      ~Server();

      void start();

   protected:
      virtual void incomingConnection( qintptr );

   private:
      QThreadPool * threadpool_;
   };

}

#endif
