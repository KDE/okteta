set( oktetadesignerexamplewidget_SRCS
  main.cpp
)

qt_wrap_ui( oktetadesignerexamplewidget_SRCS example_widget.ui )

add_executable( oktetadesignerexample  ${oktetadesignerexamplewidget_SRCS} )

target_link_libraries( oktetadesignerexample
  OktetaGui
  OktetaCore
  Qt::Widgets
)
