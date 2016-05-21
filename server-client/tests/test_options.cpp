#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE ServerClient

#include <boost/test/unit_test.hpp>
#include "../ParseOptions.h"

struct PoFixture {
   
   PoFixture() 
   { 
      BOOST_TEST_MESSAGE( "setup fixture" ); 
   }
   
   ~PoFixture()         
   { 
      BOOST_TEST_MESSAGE( "teardown fixture" ); 
   }
};



BOOST_FIXTURE_TEST_SUITE( test_command_line, PoFixture )


BOOST_AUTO_TEST_CASE( no_command_line_options )
{
   ak::gen::ParseOptions po;

   //BOOST_CHECK( po.has_option( "source-path" ) == false );
   //BOOST_CHECK( po.has_option( "verbose" ) == false );
   //BOOST_CHECK( po.has_option( "dry-run" ) == false );
}


BOOST_AUTO_TEST_CASE( has_sources_option )
{
   //BOOST_CHECK( po.has_option( "source-path" )  );
}


BOOST_AUTO_TEST_SUITE_END()


BOOST_AUTO_TEST_SUITE( test_operations )

BOOST_AUTO_TEST_SUITE_END()

