set(SUTDIR ${CMAKE_CURRENT_SOURCE_DIR}/..)

include_directories(
    ${SUTDIR}
    ${CMAKE_CURRENT_BINARY_DIR}/..
)

configure_file(oktetaparttest-config.hpp.in ${CMAKE_CURRENT_BINARY_DIR}/oktetaparttest-config.hpp @ONLY)

# TODO: deduplicate object file generation of
ecm_add_test(
    ${SUTDIR}/part.cpp
    ${SUTDIR}/browserextension.cpp
    ${SUTDIR}/partfactory.cpp
    oktetaparttest.cpp

    TEST_NAME oktetaparttest
    NAME_PREFIX kpart-
    LINK_LIBRARIES
      OktetaKastenControllers
      OktetaKastenGui
      OktetaKastenCore
      KastenControllers
      KastenGui
      KastenCore
      KF5::I18n
      KF5::Parts
      Qt5::Test
)

target_compile_definitions(oktetaparttest PRIVATE TRANSLATION_DOMAIN=\"oktetapart\")

kcoreaddons_desktop_to_json(oktetaparttest ${CMAKE_CURRENT_BINARY_DIR}/../oktetapart.desktop)

