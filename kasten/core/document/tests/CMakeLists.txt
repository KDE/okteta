project( oktetakasten_document_tests )

set( SUTDIR ${CMAKE_CURRENT_SOURCE_DIR}/.. )

# make sure to not use KDE_EXPORT or KDE_IMPORT because we link statically
# against a small part of the libs
add_definitions(-DOKTETACORE_EXPORT=)
set( EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR} )


include_directories(
  ${SUTDIR}
  ${SUTDIR}/..
  ${SUTDIR}/../../../core
  ${SUTDIR}/../../../libs/kasten/core/document
  ${SUTDIR}/../../../core/tests
  ${SUTDIR}/../../../libs/kasten/core/tests
)

########### kbytearraydocumenttest ###############

set( kbytearraydocumenttest_SRCS
  kbytearraydocumenttest.cpp
  ${SUTDIR}/kbytearraydocument.cpp
)

kde4_add_unit_test( kbytearraydocumenttest
  TESTNAME oktetakasten-document-kbytearraydocumenttest
  ${kbytearraydocumenttest_SRCS}
)
target_link_libraries( kbytearraydocumenttest
  kastencore
  testoktetacore
  ${KDE4_KIO_LIBS}
  ${QT_QTTEST_LIBRARY}
)


########### kbytearraydocumentfactorytest ###############

set( kbytearraydocumentfactorytest_SRCS
  kbytearraydocumentfactorytest.cpp
  ${SUTDIR}/kbytearraydocument.cpp
  ${SUTDIR}/kbytearraydocumentfactory.cpp
)

kde4_add_unit_test( kbytearraydocumentfactorytest
  TESTNAME oktetakasten-document-kbytearraydocumentfactorytest
  ${kbytearraydocumentfactorytest_SRCS}
)
target_link_libraries( kbytearraydocumentfactorytest
  kastencore
  oktetacoretestutil
  testoktetacore
  ${KDE4_KIO_LIBS}
  ${QT_QTTEST_LIBRARY}
)
