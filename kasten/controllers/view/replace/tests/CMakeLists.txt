set(SUTDIR ${CMAKE_CURRENT_SOURCE_DIR}/..)

ecm_add_test(
    ${SUTDIR}/replacejob.cpp
    replacejobtest.cpp

    TEST_NAME replacejobtest
    LINK_LIBRARIES
        OktetaKastenControllersByteArraySearch
        OktetaKastenGui
        OktetaKastenCore
        Qt5::Test
)
target_include_directories(replacejobtest
    PUBLIC
        ${SUTDIR}
)