#ifndef BY_QSERVER_H__
#define BY_QSERVER_H__

#include <QTcpServer>

namespace by {

   class QServer : public QTcpServer {

      Q_OBECT

   public:
      QServer( QObject * parent = nullptr );

   private:
      void incomingConnection( int socket_id );
   
   };

}

#endif

