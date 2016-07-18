set( okteta_SRCS
  main.cpp
  program.cpp
  about.cpp
  mainwindow.cpp
)

ecm_add_app_icon(okteta_SRCS
  ICONS
    ${CMAKE_CURRENT_SOURCE_DIR}/16-apps-okteta.png
    ${CMAKE_CURRENT_SOURCE_DIR}/32-apps-okteta.png
    ${CMAKE_CURRENT_SOURCE_DIR}/48-apps-okteta.png
)

add_executable( okteta  ${okteta_SRCS} )

target_link_libraries( okteta
  OktetaKastenControllers
  OktetaKastenGui
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
install( FILES org.kde.okteta.appdata.xml DESTINATION ${KDE_INSTALL_METAINFO} )

ecm_install_icons(
  ICONS 16-apps-okteta.png  32-apps-okteta.png  48-apps-okteta.png
  DESTINATION ${ICON_INSTALL_DIR}
)
