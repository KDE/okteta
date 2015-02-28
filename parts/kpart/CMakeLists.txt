project( oktetapart )

add_definitions(-DTRANSLATION_DOMAIN=\"oktetapart\")

set( OKTETAKASTEN_REL_DIR ../../kasten )

include_directories(
  ${CMAKE_CURRENT_BINARY_DIR}/${OKTETAKASTEN_REL_DIR}/controllers
  ${OKTETAKASTEN_REL_DIR}/controllers
  ${OKTETAKASTEN_REL_DIR}/controllers/document
  ${OKTETAKASTEN_REL_DIR}/controllers/view
  ${OKTETAKASTEN_REL_DIR}/controllers/view/libfinddialog
  ${OKTETAKASTEN_REL_DIR}/gui
  ${OKTETAKASTEN_REL_DIR}/gui/io
  ${OKTETAKASTEN_REL_DIR}/gui/view
  ${OKTETAKASTEN_REL_DIR}/gui/system
  ${OKTETAKASTEN_REL_DIR}/core
  ${OKTETAKASTEN_REL_DIR}/core/io
  ${OKTETAKASTEN_REL_DIR}/core/document
)


set( oktetapart_SRCS
  part.cpp
  browserextension.cpp
  partfactory.cpp
)

add_library( oktetapart  MODULE ${oktetapart_SRCS} )


target_link_libraries( oktetapart
  oktetakastencontrollers
  oktetakastengui
  oktetakastencore
  kastencontrollers
  KastenGui
  KastenCore
  KF5::I18n
  KF5::Parts
)

install( TARGETS oktetapart  DESTINATION ${PLUGIN_INSTALL_DIR})
kcoreaddons_desktop_to_json(oktetapart oktetapart.desktop )
install(
  FILES oktetapartreadonlyui.rc oktetapartbrowserui.rc oktetapartreadwriteui.rc
  DESTINATION ${DATA_INSTALL_DIR}/oktetapart
)

