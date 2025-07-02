set( SUTDIR ${CMAKE_CURRENT_SOURCE_DIR}/.. )

add_library(OktetaKastenCoreDocumentSourcesTestShim INTERFACE)
target_include_directories(OktetaKastenCoreDocumentSourcesTestShim INTERFACE
    ${CMAKE_CURRENT_BINARY_DIR}/../..
    ${SUTDIR}
    ${SUTDIR}/..
)
# Configure export macros to be setup as with a static lib
# as the source files are reused for the tests.
# Saves doing a separate export header.
target_compile_definitions(OktetaKastenCoreDocumentSourcesTestShim INTERFACE
    -DOKTETAKASTENCORE_STATICLIB
)

########### bytearraydocumenttest ###############

ecm_add_test(
    bytearraydocumenttest.cpp
    ${SUTDIR}/bytearraydocument.cpp
    TEST_NAME bytearraydocumenttest
    NAME_PREFIX oktetakasten-core-document-
    LINK_LIBRARIES
        OktetaKastenCoreDocumentSourcesTestShim
        KastenCore
        OktetaCore
        KF5::I18n
        Qt5::Test
)
target_compile_definitions(bytearraydocumenttest PRIVATE -DTRANSLATION_DOMAIN=\"liboktetakasten\")


########### bytearraydocumentfactorytest ###############

ecm_add_test(
    bytearraydocumentfactorytest.cpp
    ${SUTDIR}/bytearraydocument.cpp
    ${SUTDIR}/bytearraydocumentfactory.cpp
    TEST_NAME bytearraydocumentfactorytest
    NAME_PREFIX oktetakasten-core-document-
    LINK_LIBRARIES
        OktetaKastenCoreDocumentSourcesTestShim
        KastenCore
        OktetaCoreSourcesShimTestUtil
        OktetaCoreByteArrayModels
        KF5::I18n
        Qt5::Test
)
target_compile_definitions(bytearraydocumentfactorytest PRIVATE -DTRANSLATION_DOMAIN=\"liboktetakasten\")
