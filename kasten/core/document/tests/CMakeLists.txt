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
  -DOKTETAKASTENCORE_STATIC_DEFINE
)

########### bytearraydocumenttest ###############

ecm_add_test(
  bytearraydocumenttest.cpp
  ${SUTDIR}/bytearraydocument.cpp
  TEST_NAME bytearraydocumenttest
  NAME_PREFIX oktetakasten-document-
  LINK_LIBRARIES
  OktetaKastenCoreDocumentSourcesTestShim
  KastenCore
  TestOktetaCore
  Qt5::Test
  KF5::I18n
)


########### bytearraydocumentfactorytest ###############

ecm_add_test(
  bytearraydocumentfactorytest.cpp
  ${SUTDIR}/bytearraydocument.cpp
  ${SUTDIR}/bytearraydocumentfactory.cpp
  ${LIBOKTETAWIDGETS_REL_DIR}/bytearraychar8stringdecoder.cpp
  ${LIBOKTETAWIDGETS_REL_DIR}/bytearrayvaluestringdecoder.cpp
  TEST_NAME bytearraydocumentfactorytest
  NAME_PREFIX oktetakasten-document-
  LINK_LIBRARIES
  OktetaKastenCoreDocumentSourcesTestShim
  KastenCore
  TestOktetaCoreTestUtil
  TestOktetaCore
  Qt5::Test
  KF5::I18n
)
target_include_directories(bytearraydocumentfactorytest
    PRIVATE
        ${LIBOKTETAWIDGETS_REL_DIR}
)
