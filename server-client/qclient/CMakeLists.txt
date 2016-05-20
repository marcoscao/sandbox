
include_directories( "${PROJECT_SOURCE_DIR}")

set( SOURCES
   Client.cpp
   main.cpp
)

add_executable( qclient ${SOURCES} )

target_link_libraries( core qclient Qt5::Widgets Qt5::Network )

install( TARGETS qclient DESTINATION bin )

#add_subdirectory( tests )

