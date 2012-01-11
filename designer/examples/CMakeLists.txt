project( oktetadesignerexamples )

set( OKTETA_REL_DIR ../.. )

set( oktetadesignerexamplewidget_SRCS
  main.cpp
)

kde4_add_ui_files( oktetadesignerexamplewidget_SRCS example_widget.ui )

kde4_add_executable( oktetadesignerexample  ${oktetadesignerexamplewidget_SRCS} )

target_link_libraries( oktetadesignerexample
  ${oktetagui_LIB}
  ${oktetacore_LIB}
  ${KDE4_KDEUI_LIBS}
  ${QT_QTGUI_LIBRARY}
  ${QT_QTCORE_LIBRARY}
)
