set(SUTDIR ${CMAKE_CURRENT_SOURCE_DIR}/..)
set(CONTROLLERSDIR ${SUTDIR}/../..)

ecm_add_test(
    ${CONTROLLERSDIR}/view/libbytearraysearch/bytearraysearchutils.cpp
    ${CONTROLLERSDIR}/view/libbytearraysearch/bytearraysearchjob.cpp
    ${SUTDIR}/searchtool.cpp
    searchtooltest.cpp

    TEST_NAME searchtooltest
    NAME_PREFIX oktetakasten-controllers-search-
    LINK_LIBRARIES
        OktetaKastenGui
        OktetaKastenCore
        KF5::I18n
        Qt5::Test
)
target_include_directories(searchtooltest
    PUBLIC
        ${SUTDIR}
        ${CONTROLLERSDIR}/libconfigentries
        ${CONTROLLERSDIR}/view/libfinddialog
        ${CONTROLLERSDIR}/view/libbytearraysearch
)
target_compile_definitions(searchtooltest PRIVATE -DTRANSLATION_DOMAIN=\"liboktetakasten\")
