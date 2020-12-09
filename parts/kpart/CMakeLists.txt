add_definitions(-DTRANSLATION_DOMAIN=\"oktetapart\")

configure_file(oktetapart.desktop.in ${CMAKE_CURRENT_BINARY_DIR}/oktetapart.desktop @ONLY)

set( oktetapart_SRCS
  part.cpp
  browserextension.cpp
  partfactory.cpp
)

set_source_files_properties(partfactory.cpp PROPERTIES
    COMPILE_FLAGS -DOKTETA_VERSION=\\\"${Okteta_VERSION}\\\"
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

install( TARGETS oktetapart  DESTINATION ${KDE_INSTALL_PLUGINDIR}/kf5/parts)
kcoreaddons_desktop_to_json(oktetapart ${CMAKE_CURRENT_BINARY_DIR}/oktetapart.desktop)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/oktetapart.desktop  DESTINATION ${KDE_INSTALL_KSERVICES5DIR})

if(BUILD_TESTING)
    add_subdirectory(tests)
endif()
