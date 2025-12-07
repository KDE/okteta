set(SUTDIR ${CMAKE_CURRENT_SOURCE_DIR}/..)

ecm_add_test(
    ${SUTDIR}/searchtool.cpp
    searchtooltest.cpp

    TEST_NAME searchtooltest
    LINK_LIBRARIES
        OktetaKastenControllersConfigEntries
        OktetaKastenControllersByteArraySearch
        OktetaKastenGui
        OktetaKastenCore
        KF6::I18n
        Qt6::Test
)
target_include_directories(searchtooltest
    PUBLIC
        ${SUTDIR}
)
target_compile_definitions(searchtooltest PRIVATE -DTRANSLATION_DOMAIN=\"liboktetakasten\")
