#ifndef Q_SOCKET_H__
#define Q_SOCKET_H__

namespace by {

   class QSocket : public QTcpSocket {

      Q_OBJECT

   public:
         QSocket( QObject * parent = nullptr );

   private slots:
      void receive_client_data();

   private:
   
   };

}

#endif

