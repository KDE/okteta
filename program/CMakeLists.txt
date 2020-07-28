set( okteta_SRCS
  main.cpp
  program.cpp
  about.cpp
  mainwindow.cpp
)

set_source_files_properties(about.cpp PROPERTIES
    COMPILE_FLAGS -DOKTETA_VERSION=\\\"${Okteta_VERSION}\\\"
)

qt5_add_resources(okteta_SRCS
    okteta.qrc
)

ecm_add_app_icon(okteta_SRCS
  ICONS
    ${CMAKE_CURRENT_SOURCE_DIR}/16-apps-okteta.png
    ${CMAKE_CURRENT_SOURCE_DIR}/22-apps-okteta.png
    ${CMAKE_CURRENT_SOURCE_DIR}/32-apps-okteta.png
    ${CMAKE_CURRENT_SOURCE_DIR}/48-apps-okteta.png
    ${CMAKE_CURRENT_SOURCE_DIR}/64-apps-okteta.png
    ${CMAKE_CURRENT_SOURCE_DIR}/128-apps-okteta.png
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
  KF5::Crash
)

install( TARGETS okteta  ${KDE_INSTALL_TARGETS_DEFAULT_ARGS} )


install( PROGRAMS org.kde.okteta.desktop  DESTINATION ${KDE_INSTALL_APPDIR} )
install( FILES org.kde.okteta.appdata.xml DESTINATION ${KDE_INSTALL_METAINFODIR} )

ecm_install_icons(
  ICONS
    16-apps-okteta.png
    22-apps-okteta.png
    32-apps-okteta.png
    48-apps-okteta.png
    64-apps-okteta.png
    128-apps-okteta.png
  DESTINATION ${KDE_INSTALL_ICONDIR}
)
