
set( SOURCES
   Messages.cpp
   MessagesMngr.cpp
)

add_library( core SHARED ${SOURCES} )

target_link_libraries( core )

install( TARGETS core DESTINATION bin )


