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

install( TARGETS oktetapart  DESTINATION ${PLUGIN_INSTALL_DIR})
kcoreaddons_desktop_to_json(oktetapart oktetapart.desktop )
install(
  FILES oktetapartreadonlyui.rc oktetapartbrowserui.rc oktetapartreadwriteui.rc
  DESTINATION ${KXMLGUI_INSTALL_DIR}/oktetapart
)

