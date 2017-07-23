add_definitions(-DTRANSLATION_DOMAIN=\"oktetapart\")

set( oktetapart_SRCS
  part.cpp
  browserextension.cpp
  partfactory.cpp
)

add_library( oktetapart  MODULE ${oktetapart_SRCS} )


target_link_libraries( oktetapart
  OktetaKastenControllers
  OktetaKastenGui
  OktetaKastenCore
  KastenControllers
  KastenGui
  KastenCore
  KF5::I18n
  KF5::Parts
)

install( TARGETS oktetapart  DESTINATION ${KDE_INSTALL_PLUGINDIR})
kcoreaddons_desktop_to_json(oktetapart oktetapart.desktop  SERVICE_TYPES kpart.desktop)

install(
  FILES oktetapartreadonlyui.rc oktetapartbrowserui.rc oktetapartreadwriteui.rc
  DESTINATION ${KDE_INSTALL_KXMLGUI5DIR}/oktetapart
)

if(BUILD_TESTING)
    add_subdirectory(tests)
endif()
