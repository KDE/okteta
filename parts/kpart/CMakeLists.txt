
include_directories( ${CMAKE_SOURCE_DIR}/khexedit/lib/  )


########### next target ###############

set(khexedit2part_PART_SRCS khepart.cpp khebrowserextension.cpp khepartfactory.cpp )

kde4_automoc(${khexedit2part_PART_SRCS})

kde4_add_plugin(khexedit2part WITH_PREFIX ${khexedit2part_PART_SRCS})


target_link_libraries(khexedit2part  khexeditcore khexeditgui ${KDE4_KPARTS_LIBS} )

install(TARGETS khexedit2part  DESTINATION ${PLUGIN_INSTALL_DIR})


########### install files ###############

install( FILES khexedit2part.desktop  DESTINATION ${SERVICES_INSTALL_DIR})
install( FILES khexedit2partui.rc  DESTINATION ${DATA_INSTALL_DIR}/khexedit2part)

