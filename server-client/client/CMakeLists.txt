
include_directories( "${PROJECT_SOURCE_DIR}")
link_directories( "/opt/local/libexec/qt4/lib/" )

#include( ${QT_USE_FILE} )

#QT4_WRAP_UI(MOC_SRCS a.h )
#QT4_WRAP_CPP(MOC_SRCS a.h )

set( SOURCES
   Client.cpp
   main.cpp
)

add_executable( client ${SOURCES} ${MOC_SRCS} )



#target_link_libraries( client ${QT_LIBRARIES} )
target_link_libraries( client QtCore ${QT_LIBRARIES})

install( TARGETS client DESTINATION bin )
#install( FILES "${PROJECT_BINARY_DIR}/version.h" DESTINATION include )

#add_subdirectory( tests )

