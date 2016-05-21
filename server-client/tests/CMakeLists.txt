
include_directories( "${PROJECT_SOURCE_DIR}")

set( SOURCES
   ../HelpCmd.cpp
   test_options.cpp
)

add_executable( test_server ${SOURCES} )

target_link_libraries( test_server core ${BOOST_TEST_LIB} )

add_test(   
   NAME test_server
   WORKING_DIRECTORY ${TESTS_OUTPUT_DIR}
   COMMAND ${TESTS_OUTPUT_DIR}/test_server
)

install( TARGETS test_server DESTINATION bin/tests )
#install( FILES "${PROJECT_BINARY_DIR}/version.h" DESTINATION include )


add_custom_command( 
   TARGET test_server
   POST_BUILD
   COMMAND ctest -C $<CONFIGURATION> --output-on-failure
)

