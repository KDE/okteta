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
  Qt5::UiPlugin
)

set_target_properties( oktetadesignerplugin PROPERTIES
    OUTPUT_NAME oktetawidgets
)

install( TARGETS oktetadesignerplugin  DESTINATION ${KDE_INSTALL_QTPLUGINDIR}/designer )


if( OKTETA_BUILD_EXAMPLES )
add_subdirectory( examples )
endif()
