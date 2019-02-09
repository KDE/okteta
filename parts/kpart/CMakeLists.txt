add_definitions(-DTRANSLATION_DOMAIN=\"oktetapart\")

set( oktetapart_SRCS
  part.cpp
  browserextension.cpp
  partfactory.cpp
)

qt5_add_resources(oktetapart_SRCS
    oktetapart.qrc
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

install( FILES oktetapart.desktop  DESTINATION ${KDE_INSTALL_KSERVICES5DIR} )

if(BUILD_TESTING)
    add_subdirectory(tests)
endif()
