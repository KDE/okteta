set(SUTDIR ${CMAKE_CURRENT_SOURCE_DIR}/..)

configure_file(oktetaparttest-config.hpp.in ${CMAKE_CURRENT_BINARY_DIR}/oktetaparttest-config.hpp @ONLY)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/../oktetapart.json.in oktetapart.json @ONLY)

# TODO: deduplicate object file generation of
ecm_add_test(
    ${SUTDIR}/part.cpp
    ${SUTDIR}/navigationextension.cpp
    ${SUTDIR}/partfactory.cpp
    oktetaparttest.cpp

    TEST_NAME oktetaparttest
    LINK_LIBRARIES
      OktetaKastenControllers
      OktetaKastenGui
      OktetaKastenCore
      KastenControllers
      KastenGui
      KastenCore
      KF6::I18n
      KF6::Parts
      Qt6::Test
)

target_include_directories(oktetaparttest PRIVATE
    ${SUTDIR}
    ${CMAKE_CURRENT_BINARY_DIR}/..
)

target_compile_definitions(oktetaparttest PRIVATE TRANSLATION_DOMAIN=\"oktetapart\")
