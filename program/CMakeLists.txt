project( oktetaprogram )

add_subdirectory( oktetakasten )

include_directories(
#TODO: remove this
  ../core
  ../gui
  ../libs/kasten/core
  ../libs/kasten/core/document
  ../libs/kasten/core/io
  ../libs/kasten/core/system
  ../libs/kasten/gui
  ../libs/kasten/gui/view
  ../libs/kasten/gui/io
  ../libs/kasten/gui/system
  ../libs/kasten/gui/controller
  ../libs/kasten/controllers
  oktetakasten/document
  oktetakasten/io
  oktetakasten/view
  ${CMAKE_CURRENT_BINARY_DIR}/oktetakasten/controllers
  oktetakasten/controllers/document
  oktetakasten/controllers/view
  oktetakasten/controllers/view/libfinddialog
)

set( okteta_SRCS
  main.cpp
  program.cpp
  about.cpp
  mainwindow.cpp
)

kde4_add_app_icon(okteta_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/hi*-app-okteta.png")

kde4_add_executable( okteta  ${okteta_SRCS} )

target_link_libraries( okteta
  okteta_kastencontrollers
  okteta_kastengui
  okteta_kastenio
  okteta_kastendocument
  kastencontrollers
  kastengui
  kastencore
  oktetagui
  oktetacore
  framesprint
#   ${KDE4_KIO_LIBS}
#   ${KDE4_KFILE_LIBS}
#   ${KDE4_KDNSSD_LIBS}
)

if( QCA2_FOUND )
  target_link_libraries( okteta ${QCA2_LIBRARIES} )
endif( QCA2_FOUND )


install( TARGETS okteta  ${INSTALL_TARGETS_DEFAULT_ARGS} )


install( FILES okteta.desktop  DESTINATION ${XDG_APPS_INSTALL_DIR} )
install( FILES oktetaui.rc  DESTINATION ${DATA_INSTALL_DIR}/okteta )

kde4_install_icons( ${ICON_INSTALL_DIR} )
