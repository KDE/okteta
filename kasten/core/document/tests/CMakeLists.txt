set( SUTDIR ${CMAKE_CURRENT_SOURCE_DIR}/.. )

# make sure to not use KDE_EXPORT or KDE_IMPORT because we link statically
# against a small part of the libs
add_definitions(-DOKTETACORE_EXPORT=)
add_definitions(-DOKTETAKASTENCORE_EXPORT=)
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
  Qt5::Test
  KF5::I18n
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
  Qt5::Test
  KF5::I18n
)
