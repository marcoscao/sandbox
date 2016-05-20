#ifndef QSERVER_SOCKET_H__
#define QSERVER_SOCKET_H__

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

   		void userid_access_request_slot( std::unique_ptr< userid_access_request_body >  );

   private:
      int descriptor_;
      QTcpSocket * qt_socket_;


   };

}

#endif

