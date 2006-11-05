add_subdirectory( tests )

include_directories(${CMAKE_SOURCE_DIR}/khexedit/lib/  )


########### next target ###############

set(kbyteseditwidget_PART_SRCS kbyteseditwidget.cpp )

kde4_automoc(${kbyteseditwidget_PART_SRCS})

kde4_add_plugin(kbyteseditwidget WITH_PREFIX ${kbyteseditwidget_PART_SRCS})


target_link_libraries(kbyteseditwidget  khexeditcore khexeditgui ${KDE4_KPARTS_LIBS} )

install(TARGETS kbyteseditwidget  DESTINATION ${PLUGIN_INSTALL_DIR})


########### install files ###############

install( FILES kbyteseditwidget.desktop  DESTINATION ${SERVICES_INSTALL_DIR})


