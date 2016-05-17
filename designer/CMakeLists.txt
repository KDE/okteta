set( oktetadesignerplugin_SRCS
  bytearraycolumnviewdesignerfactory.cpp
  bytearrayrowviewdesignerfactory.cpp
  oktetawidgetdesignerfactorycollection.cpp
  fillerbytearraymodel.cpp
)


add_library( oktetadesignerplugin  MODULE ${oktetadesignerplugin_SRCS} )

target_link_libraries( oktetadesignerplugin
  OktetaGui
  OktetaCore
  Qt5::Widgets
  Qt5::Designer
)

install( TARGETS oktetadesignerplugin  DESTINATION ${QT_PLUGIN_INSTALL_DIR}/designer )


if( OKTETA_BUILD_EXAMPLES )
add_subdirectory( examples )
endif( OKTETA_BUILD_EXAMPLES )
