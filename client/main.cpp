#include "Client.h"
#include <QtCore/QCoreApplication>
#include <exception>
#include <iostream>

using namespace std;

int main(int argc, char** argv ) 
{

   try {
      QCoreApplication app( argc, argv );
      //return app.exec();

      by::Client c;
      

   }
   catch( std::exception const & e ) {
      std::cout << "exception: " << e.what();
      cout << endl;
   }

   return 0;
}


