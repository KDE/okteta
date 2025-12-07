configure_file(oktetapart.json.in ${CMAKE_CURRENT_BINARY_DIR}/oktetapart.json @ONLY)

add_library(oktetapart MODULE)

target_compile_definitions(oktetapart PRIVATE -DTRANSLATION_DOMAIN=\"oktetapart\")

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

configure_file(partfactory-config.hpp.in ${CMAKE_CURRENT_BINARY_DIR}/partfactory-config.hpp @ONLY)

install( TARGETS oktetapart  DESTINATION ${KDE_INSTALL_PLUGINDIR}/kf6/parts)

if(BUILD_TESTING)
    add_subdirectory(tests)
endif()
