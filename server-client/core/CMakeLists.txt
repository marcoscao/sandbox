
set( SOURCES
   Messages.cpp
   MsgManager.cpp
)

add_library( core SHARED ${SOURCES} )

target_link_libraries( core Qt5::Widgets Qt5::Network )

install( TARGETS core DESTINATION bin )


