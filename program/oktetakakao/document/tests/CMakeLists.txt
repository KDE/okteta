project( oktetakakao_document_tests )

set( SUTDIR ${CMAKE_CURRENT_SOURCE_DIR}/.. )

include_directories(
  ${SUTDIR}
  ${SUTDIR}/../../../core
  ${SUTDIR}/../../libkakao/core/document
  ${SUTDIR}/../../../core/tests
  ${SUTDIR}/../../libkakao/core/tests
)

########### kbytearraydocumenttest ###############

set( kbytearraydocumenttest_SRCS
  kbytearraydocumenttest.cpp
  ${SUTDIR}/kbytearraydocument.cpp
)

kde4_add_unit_test( kbytearraydocumenttest
  TESTNAME oktetakakao-document-kbytearraydocumenttest
  ${kbytearraydocumenttest_SRCS}
)
target_link_libraries( kbytearraydocumenttest
  kakaocore
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
  TESTNAME oktetakakao-document-kbytearraydocumentfactorytest
  ${kbytearraydocumentfactorytest_SRCS}
)
target_link_libraries( kbytearraydocumentfactorytest
  kakaocore
  oktetacoretestutil
  testoktetacore
  ${KDE4_KIO_LIBS}
  ${QT_QTTEST_LIBRARY}
)
