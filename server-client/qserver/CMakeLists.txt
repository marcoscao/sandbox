
include_directories( "${PROJECT_SOURCE_DIR}")

set( SOURCES
   Server.cpp
   Connection.cpp
   main.cpp
)

add_executable( qserver ${SOURCES} )

target_link_libraries( qserver core Qt5::Widgets Qt5::Network )

install( TARGETS qserver DESTINATION bin )


