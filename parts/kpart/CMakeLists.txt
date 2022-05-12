add_definitions(-DTRANSLATION_DOMAIN=\"oktetapart\")

configure_file(oktetapart.desktop.in ${CMAKE_CURRENT_BINARY_DIR}/oktetapart.desktop @ONLY)

add_library(oktetapart MODULE)

target_link_libraries(oktetapart
    OktetaKastenControllers
    OktetaKastenGui
    OktetaKastenCore
    KastenControllers
    KastenGui
    KastenCore
    KF5::I18n
    KF5::Parts
)

target_sources(oktetapart
    PRIVATE
        part.cpp
        browserextension.cpp
        partfactory.cpp

        oktetapart.qrc
)

set_source_files_properties(partfactory.cpp PROPERTIES
    COMPILE_FLAGS -DOKTETA_VERSION=\\\"${Okteta_VERSION}\\\"
)

install( TARGETS oktetapart  DESTINATION ${KDE_INSTALL_PLUGINDIR}/kf${QT_MAJOR_VERSION}/parts)
kcoreaddons_desktop_to_json(oktetapart ${CMAKE_CURRENT_BINARY_DIR}/oktetapart.desktop)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/oktetapart.desktop  DESTINATION ${KDE_INSTALL_KSERVICESDIR})

if(BUILD_TESTING)
    add_subdirectory(tests)
endif()
