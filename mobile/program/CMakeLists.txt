project( oktetamobileprogram )

set( OKTETAKASTEN_REL_DIR ../../kasten )
set( MOBILEKASTEN_REL_DIR ../kasten )
set( KASTEN_REL_DIR ../../libs/kasten )
set( OKTETA_REL_DIR ../.. )

include_directories(
  ${CMAKE_CURRENT_BINARY_DIR}/${OKTETAKASTEN_REL_DIR}/controllers
  ${OKTETAKASTEN_REL_DIR}/controllers
  ${OKTETAKASTEN_REL_DIR}/controllers/document
#   ${OKTETAKASTEN_REL_DIR}/controllers/view
#   ${OKTETAKASTEN_REL_DIR}/controllers/view/libfinddialog
  ${OKTETAKASTEN_REL_DIR}/gui
#   ${OKTETAKASTEN_REL_DIR}/gui/io
  ${OKTETAKASTEN_REL_DIR}/gui/view
  ${OKTETAKASTEN_REL_DIR}/core
  ${OKTETAKASTEN_REL_DIR}/core/io
  ${OKTETAKASTEN_REL_DIR}/core/document
  ${MOBILEKASTEN_REL_DIR}/controllers
  ${MOBILEKASTEN_REL_DIR}/gui
  ${MOBILEKASTEN_REL_DIR}/gui/shell
  ${MOBILEKASTEN_REL_DIR}/gui/controller
#   ${KASTEN_REL_DIR}/controllers
  ${KASTEN_REL_DIR}/gui
  ${KASTEN_REL_DIR}/gui/view
#   ${KASTEN_REL_DIR}/gui/io
  ${KASTEN_REL_DIR}/gui/system
#   ${KASTEN_REL_DIR}/gui/shell
  ${KASTEN_REL_DIR}/gui/controller
  ${KASTEN_REL_DIR}/core
  ${KASTEN_REL_DIR}/core/document
  ${KASTEN_REL_DIR}/core/entity
  ${KASTEN_REL_DIR}/core/io
  ${KASTEN_REL_DIR}/core/system
#TODO: remove this
#   ${OKTETA_REL_DIR}/gui
  ${OKTETA_REL_DIR}/core
)

set( okteta_SRCS
  main.cpp
  program.cpp
  about.cpp
  mainwindow.cpp
)

kde4_add_executable( okteta-mobile  ${okteta_SRCS} )

target_link_libraries( okteta-mobile
  kasten2okteta1controllers
#   kasten2okteta1gui
  kasten2okteta1core
  kasten2mobilecontrollers
  kasten2mobilegui
  kasten2controllers
#   kasten2gui
  kasten2core
  ${KDE4_KDEUI_LIBS}
)

install( TARGETS okteta-mobile  ${INSTALL_TARGETS_DEFAULT_ARGS} )

install( PROGRAMS okteta-mobile.desktop DESTINATION ${XDG_APPS_INSTALL_DIR} )
