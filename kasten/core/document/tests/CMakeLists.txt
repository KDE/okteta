project( oktetakasten_document_tests )

set( SUTDIR ${CMAKE_CURRENT_SOURCE_DIR}/.. )

# make sure to not use KDE_EXPORT or KDE_IMPORT because we link statically
# against a small part of the libs
add_definitions(-DOKTETACORE_EXPORT=)
add_definitions(-DOKTETAKASTENCORE_EXPORT=)
set( EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR} )


include_directories(
  ${SUTDIR}
  ${SUTDIR}/..
  ${SUTDIR}/../../../core
  ${SUTDIR}/../../../libs/kasten/core/document
  ${SUTDIR}/../../../core/tests
  ${SUTDIR}/../../../libs/kasten/core/tests
)

########### bytearraydocumenttest ###############

set( bytearraydocumenttest_SRCS
  bytearraydocumenttest.cpp
  ${SUTDIR}/bytearraydocument.cpp
)

kde4_add_unit_test( bytearraydocumenttest
  TESTNAME oktetakasten-document-bytearraydocumenttest
  ${bytearraydocumenttest_SRCS}
)
target_link_libraries( bytearraydocumenttest
  ${KASTENCORE_LIBS}
  testoktetacore
  ${KDE4_KIO_LIBS}
  ${QT_QTTEST_LIBRARY}
)


########### bytearraydocumentfactorytest ###############

set( bytearraydocumentfactorytest_SRCS
  bytearraydocumentfactorytest.cpp
  ${SUTDIR}/bytearraydocument.cpp
  ${SUTDIR}/bytearraydocumentfactory.cpp
)

kde4_add_unit_test( bytearraydocumentfactorytest
  TESTNAME oktetakasten-document-bytearraydocumentfactorytest
  ${bytearraydocumentfactorytest_SRCS}
)
target_link_libraries( bytearraydocumentfactorytest
  ${KASTENCORE_LIBS}
  oktetacoretestutil
  testoktetacore
  ${KDE4_KIO_LIBS}
  ${QT_QTTEST_LIBRARY}
)
