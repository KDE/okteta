set( SUTDIR ${CMAKE_CURRENT_SOURCE_DIR}/.. )
set(LIBOKTETAWIDGETS_REL_DIR ../../../gui/liboktetawidgets)

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
    ${LIBOKTETAWIDGETS_REL_DIR}/bytearraychar8stringdecoder.cpp
    ${LIBOKTETAWIDGETS_REL_DIR}/bytearrayvaluestringdecoder.cpp
    TEST_NAME bytearraydocumentfactorytest
    LINK_LIBRARIES
        OktetaKastenCoreDocumentSourcesTestShim
        KastenCore
        OktetaCoreSourcesShimTestUtil
        OktetaCoreByteArrayModels
        OktetaCoreCharCodecs
        OktetaCoreValueCodecs
        KF5::I18n
        Qt5::Test
)
target_compile_definitions(bytearraydocumentfactorytest PRIVATE -DTRANSLATION_DOMAIN=\"liboktetakasten\")
target_include_directories(bytearraydocumentfactorytest
    PRIVATE
        ${LIBOKTETAWIDGETS_REL_DIR}
)
