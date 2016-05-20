#include "QSocket.h"

namespace by {

   QSocket::QSocket( QObject * parent )
   : QTcpSocket( parent )
   {
      connect( this, SIGNAL( readyRead ), this, SLOT( ready_read_slot ) );
      
      nextBlockSize = 0;
   }


}
