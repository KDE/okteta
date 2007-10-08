add_subdirectory( tests )

include_directories(
  ../../core
  ../../gui/libkcolumnsview
  ../../gui
)

set( kbytearrayedit_PART_SRCS
  kbytearrayedit.cpp
  factory.cpp
)


kde4_add_plugin( kbytearrayedit WITH_PREFIX ${kbytearrayedit_PART_SRCS} )

target_link_libraries(kbytearrayedit  oktetacore oktetagui ${KDE4_KPARTS_LIBS} )

install( TARGETS kbytearrayedit          DESTINATION ${PLUGIN_INSTALL_DIR} )
install( FILES   kbytearrayedit.desktop  DESTINATION ${SERVICES_INSTALL_DIR} )


