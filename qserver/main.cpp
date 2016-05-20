#include "Server.h"

#include <QtCore/QCoreApplication>

#include <exception>
#include <iostream>

using namespace std;

int main(int argc, char** argv ) 
{

   try {
      QCoreApplication app(argc,argv);

      core::Server srv;
      srv.start();

      return app.exec();
   }
   catch( std::exception const & e ) {
      std::cout << "exception: " << e.what();
      cout << endl;
   }

   return 0;
}


