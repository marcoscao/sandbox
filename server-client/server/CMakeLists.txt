
include_directories( "${PROJECT_SOURCE_DIR}")
link_directories( "/opt/local/libexec/qt4/lib/" )

include( ${QT_USE_FILE} )

QT4_WRAP_CPP( MOC_SRC
   QServer.h
   QClient.h
)

set( SOURCES
   Server.cpp
   main.cpp
)

add_executable( server ${SOURCES} ${MOC_SRC} )

target_link_libraries( server QtCore )

install( TARGETS server DESTINATION bin )
#install( FILES "${PROJECT_BINARY_DIR}/version.h" DESTINATION include )


#add_subdirectory( tests )

