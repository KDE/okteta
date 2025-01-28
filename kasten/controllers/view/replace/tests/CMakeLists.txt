set(SUTDIR ${CMAKE_CURRENT_SOURCE_DIR}/..)
set(CONTROLLERSDIR ${SUTDIR}/../..)

ecm_add_test(
    ${CONTROLLERSDIR}/view/libbytearraysearch/bytearraysearchjob.cpp
    ${SUTDIR}/replacejob.cpp
    replacejobtest.cpp

    TEST_NAME replacejobtest
    LINK_LIBRARIES
        OktetaKastenGui
        OktetaKastenCore
        Qt5::Test
)
target_include_directories(replacejobtest
    PUBLIC
        ${SUTDIR}
        ${CONTROLLERSDIR}/view/libfinddialog
        ${CONTROLLERSDIR}/view/libbytearraysearch
)
