#include "Server.h"
#include <exception>
#include <iostream>

using namespace std;

int main(int argc, char** argv ) 
{

   try {
      by::Server srv;
      //srv.start();

   }
   catch( std::exception const & e ) {
      std::cout << "exception: " << e.what();
      cout << endl;
   }

   return 0;
}


