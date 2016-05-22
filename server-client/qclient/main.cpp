#include "Client.h"

#include <QtCore/QCoreApplication>
#include <QtCore/QTimer>

#include <exception>
#include <iostream>
#include <string>

using namespace std;

int main(int argc, char** argv ) 
{

   try {
      QCoreApplication app(argc,argv);

      if( argc < 2 ) {
         cout << "Please, enter server port" << endl;
         exit(0);
      }

      core::Client cl;
      QObject::connect( &cl, SIGNAL(quit_signal()), QCoreApplication::instance(), SLOT(quit() ), Qt::DirectConnection );
   
      cout << "client trying to connect" << endl;
      cl.start( stoi( argv[1] ) );

      cout << "entering event-loop" << endl;

      return app.exec();
   }
   catch( std::exception const & e ) {
      std::cout << "exception: " << e.what();
      cout << endl;
   }

   return 0;
}


