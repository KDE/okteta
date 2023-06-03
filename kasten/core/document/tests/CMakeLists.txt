set( SUTDIR ${CMAKE_CURRENT_SOURCE_DIR}/.. )

# Configure export macros to be setup as with a static lib
# as the source files are reused for the tests.
# Saves doing a separate export header.
add_definitions(-DOKTETAKASTENCORE_STATICLIB)
set( EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR} )


include_directories(
    ${CMAKE_CURRENT_BINARY_DIR}/../..
    ${SUTDIR}
    ${SUTDIR}/..
)

########### bytearraydocumenttest ###############

ecm_add_test(
    bytearraydocumenttest.cpp
    ${SUTDIR}/bytearraydocument.cpp
    TEST_NAME bytearraydocumenttest
    NAME_PREFIX oktetakasten-document-
    LINK_LIBRARIES
        KastenCore
        testoktetacore
        KF5::I18n
        Qt5::Test
)


########### bytearraydocumentfactorytest ###############

ecm_add_test(
    bytearraydocumentfactorytest.cpp
    ${SUTDIR}/bytearraydocument.cpp
    ${SUTDIR}/bytearraydocumentfactory.cpp
    TEST_NAME bytearraydocumentfactorytest
    NAME_PREFIX oktetakasten-document-
    LINK_LIBRARIES
        KastenCore
        oktetacoretestutil
        testoktetacore
        KF5::I18n
        Qt5::Test
)
