#include "QServer.h"
#include "QSocket.h"

namespace by {

   QServer::QServer( QObject * parent )
   : QTcpServer( parent )
   {
   }

   void QServer::incomingConnection( int socket_id )
   {
      std::unique_ptr<QSocket> p = std::make_unique<QSocket>( this );
      p->setSocketDescriptor( socket_id );

      clients_.insert( p );
   }


}
