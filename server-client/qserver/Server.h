/*
 * Server class
 * Qt 5.6 based
 *
 * A threadpool launchs a thread per client connection ( until reach a maximum of N )
 * Each Connection class ( Runnable ) manages the communication between specific client and server
 *
 * Singleton class
 */

#ifndef QSERVER_SERVER_H__
#define QSERVER_SERVER_H__

#include <QtCore/QAtomicInt>
#include <QtCore/QMutex>
#include <QtCore/QTimer>
#include <QtNetwork/QTcpServer>

#include <unordered_map>
#include <unordered_set>

class QThreadPool;

namespace core {

   class Server : public QTcpServer {
      Q_OBJECT
   public:
      using BusinessUsersCt = std::unordered_set< int >;
      using BusinessClientsCt = std::unordered_map< std::string, BusinessUsersCt >;
      
      ~Server();

      static Server & instance();

      int new_client_dni();

      void start();

      /*
       * Register a new bussiness client
       */
      bool register_business_client( QString const & );

      /*
       * Returns true if user_id is granted to be registered, false either no valid client, repeated user_id, ...
       */
      bool register_business_userid( QString const & client_name, int user_id );

      /*
       */
      void reset_inactivity_timer();

   protected:
      virtual void incomingConnection( qintptr );

   private:
      /*
       * inactivity time before shutdown gently
       */
      static const int INACTIVITY_TIMEOUT = 20000;

      QThreadPool * threadpool_;
      static QAtomicInt client_dni_counter_;
      BusinessClientsCt business_clients_;
      QMutex mutex_;
      QTimer inactivity_timer_;

      Server( QObject * parent = nullptr );

      void dump_business_clients_();

   private slots:
      void inactivity_timeout_slot_();
   };

}

#endif
