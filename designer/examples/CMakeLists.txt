set( oktetadesignerexamplewidget_SRCS
  main.cpp
)

qt6_wrap_ui( oktetadesignerexamplewidget_SRCS example_widget.ui )

add_executable( oktetadesignerexample  ${oktetadesignerexamplewidget_SRCS} )

target_link_libraries( oktetadesignerexample
  Okteta${OKTETALIBS_ABI_VERSION}Gui
  Okteta${OKTETALIBS_ABI_VERSION}Core
  Qt6::Widgets
)
