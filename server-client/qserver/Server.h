#ifndef QSERVER_SERVER_H__
#define QSERVER_SERVER_H__

#include <QtNetwork/QTcpServer>
#include <QtCore/QAtomicInt>

class QThreadPool;

namespace core {

   class Server : public QTcpServer {
      Q_OBJECT
   public:

      Server( QObject * parent = nullptr );
      
      ~Server();

      void start();

	  int new_client_dni();

   protected:
      virtual void incomingConnection( qintptr );

   private:
      QThreadPool * threadpool_;
      static QAtomicInt client_dni_counter_;
   };

}

#endif
