
include_directories(
  ../../core
  ../../gui/libkcolumnsview
  ../../gui
)


set( oktetapart_SRCS part.cpp browserextension.cpp partfactory.cpp )

kde4_add_plugin( oktetapart WITH_PREFIX ${oktetapart_SRCS})


target_link_libraries( oktetapart  oktetacore oktetagui ${KDE4_KPARTS_LIBS} )

install( TARGETS oktetapart  DESTINATION ${PLUGIN_INSTALL_DIR})
install( FILES oktetapart.desktop  DESTINATION ${SERVICES_INSTALL_DIR} )
install( FILES oktetapartui.rc  DESTINATION ${DATA_INSTALL_DIR}/oktetapart )

