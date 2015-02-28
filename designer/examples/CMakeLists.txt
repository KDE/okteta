project( oktetadesignerexamples )

set( oktetadesignerexamplewidget_SRCS
  main.cpp
)

qt5_wrap_ui( oktetadesignerexamplewidget_SRCS example_widget.ui )

add_executable( oktetadesignerexample  ${oktetadesignerexamplewidget_SRCS} )

target_link_libraries( oktetadesignerexample
  ${oktetagui_LIB}
  ${oktetacore_LIB}
  Qt5::Widgets
)
