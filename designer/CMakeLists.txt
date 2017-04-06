if (NOT DesignerPluginDeps_FOUND)
  return()
endif()

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
)
target_include_directories( oktetadesignerplugin
    PRIVATE
        ${Qt5UiPlugin_INCLUDE_DIRS}
        ${Qt5Designer_INCLUDE_DIRS}
)

install( TARGETS oktetadesignerplugin  DESTINATION ${KDE_INSTALL_QTPLUGINDIR}/designer )


if( OKTETA_BUILD_EXAMPLES )
add_subdirectory( examples )
endif( OKTETA_BUILD_EXAMPLES )
