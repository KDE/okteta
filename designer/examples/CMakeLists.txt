project( oktetadesignerexamples )

include_directories(
  ${INCLUDE_INSTALL_DIR}
  ${INCLUDE_INSTALL_DIR}/KDE
)

set( oktetadesignerexamplewidget_SRCS
  main.cpp
)

kde4_add_ui_files( oktetadesignerexamplewidget_SRCS example_widget.ui )

add_executable( oktetadesignerexample  ${oktetadesignerexamplewidget_SRCS} )

target_link_libraries( oktetadesignerexample
  ${oktetagui_LIB}
  ${oktetacore_LIB}
  Qt5::Widgets
)
