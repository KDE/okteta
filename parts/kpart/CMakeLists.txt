set( KASTEN_REL_DIR ../../libs/kasten )
set( OKTETAKASTEN_REL_DIR ../../kasten )
set( OKTETA_REL_DIR ../.. )

include_directories(
  ${CMAKE_CURRENT_BINARY_DIR}/${OKTETAKASTEN_REL_DIR}/controllers
  ${OKTETAKASTEN_REL_DIR}/controllers
  ${OKTETAKASTEN_REL_DIR}/controllers/document
  ${OKTETAKASTEN_REL_DIR}/controllers/view
  ${OKTETAKASTEN_REL_DIR}/controllers/view/libfinddialog
  ${OKTETAKASTEN_REL_DIR}/gui
  ${OKTETAKASTEN_REL_DIR}/gui/io
  ${OKTETAKASTEN_REL_DIR}/gui/view
  ${OKTETAKASTEN_REL_DIR}/core
  ${OKTETAKASTEN_REL_DIR}/core/io
  ${OKTETAKASTEN_REL_DIR}/core/document
  ${KASTEN_REL_DIR}/controllers
  ${KASTEN_REL_DIR}/gui
  ${KASTEN_REL_DIR}/gui/view
  ${KASTEN_REL_DIR}/gui/io
  ${KASTEN_REL_DIR}/gui/system
  ${KASTEN_REL_DIR}/gui/shell
  ${KASTEN_REL_DIR}/gui/controller
  ${KASTEN_REL_DIR}/core
  ${KASTEN_REL_DIR}/core/document
  ${KASTEN_REL_DIR}/core/entity
  ${KASTEN_REL_DIR}/core/io
  ${KASTEN_REL_DIR}/core/system
#TODO: remove this
  ${OKTETA_REL_DIR}/core
  ${OKTETA_REL_DIR}/gui/libcolumnsview
  ${OKTETA_REL_DIR}/gui
)


set( oktetapart_SRCS
  part.cpp
  browserextension.cpp
  partfactory.cpp
  main.cpp
)

kde4_add_plugin( oktetapart  ${oktetapart_SRCS})


target_link_libraries( oktetapart
  kasten2okteta1controllers
  kasten2okteta1gui
  kasten2okteta1core
  kasten2controllers
  kasten2gui
  kasten2core
  ${KDE4_KPARTS_LIBS}
)

install( TARGETS oktetapart  DESTINATION ${PLUGIN_INSTALL_DIR})
install( FILES oktetapart.desktop  DESTINATION ${SERVICES_INSTALL_DIR} )
install(
  FILES oktetapartreadonlyui.rc oktetapartbrowserui.rc oktetapartreadwriteui.rc
  DESTINATION ${DATA_INSTALL_DIR}/oktetapart
)

