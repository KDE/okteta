add_definitions(-DTRANSLATION_DOMAIN=\"oktetapart\")

configure_file(oktetapart.json.in ${CMAKE_CURRENT_BINARY_DIR}/oktetapart.json @ONLY)

add_library(oktetapart MODULE)

target_link_libraries(oktetapart
    OktetaKastenControllers
    OktetaKastenGui
    OktetaKastenCore
    KastenControllers
    KastenGui
    KastenCore
    KF6::I18n
    KF6::Parts
)

target_sources(oktetapart
    PRIVATE
        part.cpp
        navigationextension.cpp
        partfactory.cpp

        oktetapart.qrc
)

set_source_files_properties(partfactory.cpp PROPERTIES
    COMPILE_FLAGS -DOKTETA_VERSION=\\\"${Okteta_VERSION}\\\"
)

install( TARGETS oktetapart  DESTINATION ${KDE_INSTALL_PLUGINDIR}/kf6/parts)

if(BUILD_TESTING)
    add_subdirectory(tests)
endif()
