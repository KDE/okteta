set(SUTDIR ${CMAKE_CURRENT_SOURCE_DIR}/..)

ecm_add_test(
    ${SUTDIR}/searchtool.cpp
    searchtooltest.cpp

    TEST_NAME searchtooltest
    NAME_PREFIX oktetakasten-controllers-search-
    LINK_LIBRARIES
        OktetaKastenControllersConfigEntries
        OktetaKastenControllersByteArraySearch
        OktetaKastenGui
        OktetaKastenCore
        KF5::I18n
        Qt5::Test
)
target_include_directories(searchtooltest
    PUBLIC
        ${SUTDIR}
)
target_compile_definitions(searchtooltest PRIVATE -DTRANSLATION_DOMAIN=\"liboktetakasten\")
