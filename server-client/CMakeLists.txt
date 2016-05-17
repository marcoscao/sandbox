cmake_minimum_required( VERSION 2.8 )

project(supuesto_4)

enable_testing()


# clang
SET( CMAKE_CXX_COMPILER "g++" )

SET( CMAKE_CXX_FLAGS "-std=c++14 -g" )


#
# main directories
#
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(TESTS_OUTPUT_DIR ${CMAKE_BINARY_DIR}/bin)


#
# Packages stuff
# set local dir with other find scripts
set( CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/third_parties/cmake/ )

#set( CMAKE_PREFIX_PATH ${PROJECT_SOURCE_DIR}/third_parties/cmake/qt5.6/ )
set( CMAKE_PREFIX_PATH /Users/msantamaria/work/qt5/5.6/clang_64/lib/cmake/ )

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

find_package( Qt5Widgets )
find_package( Qt5Network )

#set( QT5_INCLUDES /Users/msantamaria/work/qt5/5.6/clang_64/include/ )
#set( QT5_LIBS /Users/msantamaria/work/qt5/5.6/ios/lib/ )


#
# Boost stuff
#
find_package( boost REQUIRED COMPONENTS program_options date_time )

include_directories( ${Boost_INCLUDE_DIR} )
link_directories( ${Boost_LIBRARY_DIRS} ${QT5_LIBS} )


find_package( Qt5Core )


#
# set third parties includes and libraries paths
#

if( ${APPLE} )
   set( BOOST_PROGRAM_OPTIONS_LIB 
	    boost_program_options-mt
	    boost_date_time-mt 
	    boost_system-mt 
	    boost_filesystem-mt )
   
   set( BOOST_TEST_LIB 
            boost_unit_test_framework-mt )

endif()



message( "* boost include dirs: ${Boost_INCLUDE_DIR}" )
message( "* boost library dirs: ${Boost_LIBRARY_DIRS}" )
message( "* qt include dirs : ${QT_INCLUDE_DIRS}" )
message( "* TESTS_OUTPUT_DIR : ${TESTS_OUTPUT_DIR}" )


add_subdirectory( qserver )
add_subdirectory( qclient )

