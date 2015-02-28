project( oktetaprogram )

set( OKTETAKASTEN_REL_DIR ../kasten )

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

set( okteta_SRCS
  main.cpp
  program.cpp
  about.cpp
  mainwindow.cpp
)

#TODO KF5:
# kde4_add_app_icon(okteta_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/hi*-app-okteta.png")

add_executable( okteta  ${okteta_SRCS} )

target_link_libraries( okteta
  oktetakastencontrollers
  oktetakastengui
  OktetaKastenCore
  KastenControllers
  KastenGui
  KastenCore
  KF5::ConfigCore
  KF5::I18n
  KF5::XmlGui
  KF5::DBusAddons
)

install( TARGETS okteta  ${INSTALL_TARGETS_DEFAULT_ARGS} )


install( PROGRAMS org.kde.okteta.desktop  DESTINATION ${XDG_APPS_INSTALL_DIR} )
install( FILES oktetaui.rc  DESTINATION ${KXMLGUI_INSTALL_DIR}/okteta )
install( FILES okteta.appdata.xml DESTINATION ${SHARE_INSTALL_PREFIX}/appdata )

ecm_install_icons(
  ICONS 16-apps-okteta.png  32-apps-okteta.png  48-apps-okteta.png
  DESTINATION ${ICON_INSTALL_DIR}
)
