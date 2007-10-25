project( oktetaprogram )

add_subdirectory( libkakao )
add_subdirectory( oktetakakao )

# if(KDE4_BUILD_TESTS)
#   add_subdirectory(tests)
# endif(KDE4_BUILD_TESTS)

include_directories(
  libkakao/core/document
  libkakao/core/system
  libkakao/gui/view
  libkakao/gui/system
  libkakao/gui/controller
  libkakao/controllers
  oktetakakao/document
  oktetakakao/view
  oktetakakao/controllers/document
  oktetakakao/controllers/view
  oktetakakao/controllers/view/libfinddialog
)

set( okteta_SRCS
  main.cpp
  program.cpp
  about.cpp
  mainwindow.cpp
)

kde4_add_executable( okteta  ${okteta_SRCS} )

target_link_libraries( okteta
  oktetagui
  oktetacore
  okteta_kakaocontrollers
  okteta_kakaogui
  okteta_kakaocore
  kakaocontrollers
  kakaogui
  kakaocore
  ${KDE4_KIO_LIBS}
)

install( TARGETS okteta  DESTINATION ${BIN_INSTALL_DIR} )


install( FILES okteta.desktop  DESTINATION ${XDG_APPS_INSTALL_DIR} )
install( FILES oktetaui.rc  DESTINATION ${DATA_INSTALL_DIR}/okteta )

kde4_install_icons( ${ICON_INSTALL_DIR} )
