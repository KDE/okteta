project( oktetadesignerplugin )

set( OKTETA_REL_DIR .. )

include_directories(
  ${OKTETA_REL_DIR}/core
  ${OKTETA_REL_DIR}/gui/libcolumnsview
  ${OKTETA_REL_DIR}/gui
)

set( oktetadesignerplugin_SRCS
  bytearraycolumnviewdesignerfactory.cpp
  bytearrayrowviewdesignerfactory.cpp
  oktetawidgetdesignerfactorycollection.cpp
  fillerbytearraymodel.cpp
  main.cpp
)


kde4_add_plugin( oktetadesignerplugin  ${oktetadesignerplugin_SRCS} )

target_link_libraries( oktetadesignerplugin
  ${oktetagui_LIB}
  ${oktetacore_LIB}
  ${QT_QTGUI_LIBRARY}
  ${QT_QTCORE_LIBRARY}
)

install( TARGETS oktetadesignerplugin  DESTINATION ${PLUGIN_INSTALL_DIR}/plugins/designer )


if( OKTETA_BUILD_EXAMPLES )
add_subdirectory( examples )
endif( OKTETA_BUILD_EXAMPLES )
