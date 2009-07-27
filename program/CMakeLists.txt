project( oktetaprogram )

set( OKTETAKASTEN_REL_DIR ../kasten )
set( KASTEN_REL_DIR ../libs/kasten )
set( OKTETA_REL_DIR .. )

include_directories(
  ${CMAKE_CURRENT_BINARY_DIR}/${OKTETAKASTEN_REL_DIR}/controllers
  ${OKTETAKASTEN_REL_DIR}/controllers/document
  ${OKTETAKASTEN_REL_DIR}/controllers/view
  ${OKTETAKASTEN_REL_DIR}/controllers/view/libfinddialog
  ${OKTETAKASTEN_REL_DIR}/gui/view
  ${OKTETAKASTEN_REL_DIR}/core/io
  ${OKTETAKASTEN_REL_DIR}/core/document
  ${KASTEN_REL_DIR}/controllers
  ${KASTEN_REL_DIR}/gui
  ${KASTEN_REL_DIR}/gui/view
  ${KASTEN_REL_DIR}/gui/io
  ${KASTEN_REL_DIR}/gui/system
  ${KASTEN_REL_DIR}/gui/controller
  ${KASTEN_REL_DIR}/core
  ${KASTEN_REL_DIR}/core/document
  ${KASTEN_REL_DIR}/core/entity
  ${KASTEN_REL_DIR}/core/io
  ${KASTEN_REL_DIR}/core/system
#TODO: remove this
  ${OKTETA_REL_DIR}/gui
  ${OKTETA_REL_DIR}/core
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
  okteta_kastencore
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
