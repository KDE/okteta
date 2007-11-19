project( kakao_core_tests )


set( EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR} )

set( SUTDIR ${CMAKE_CURRENT_SOURCE_DIR}/.. )

include_directories(
  ${SUTDIR}
)


########### util ###############

set( kakaocoretestutil_LIB_SRCS
  util/filesystem.cpp
)

kde4_add_library( kakaocoretestutil  STATIC ${kakaocoretestutil_LIB_SRCS} )

########### testdocumenttest ###############

set( testdocumenttest_SRCS
  testdocument.cpp
  testdocumenttest.cpp
)

kde4_add_unit_test( testdocumenttest
  TESTNAME libkakao-core-testdocumenttest
  ${testdocumenttest_SRCS}
)
target_link_libraries( testdocumenttest
  kakaocore
  ${KDE4_KIO_LIBS}
  ${QT_QTTEST_LIBRARY}
)

########### testdocumentfilesynchronizertest ###############

set( testdocumentfilesynchronizertest_SRCS
  testdocument.cpp
  testdocumentfilesynchronizer.cpp
  testdocumentfilesynchronizertest.cpp
)

kde4_add_unit_test( testdocumentfilesynchronizertest
  TESTNAME libkakao-core-testdocumentfilesynchronizertest
  ${testdocumentfilesynchronizertest_SRCS}
)
target_link_libraries( testdocumentfilesynchronizertest
  kakaocore
  kakaocoretestutil
  ${KDE4_KIO_LIBS}
  ${QT_QTTEST_LIBRARY}
)

########### kdocumentmanagertest ###############

set( kdocumentmanagertest_SRCS
  testdocument.cpp
  kdocumentmanagertest.cpp
)

kde4_add_unit_test( kdocumentmanagertest
  TESTNAME libkakao-core-kdocumentmanagertest
  ${kdocumentmanagertest_SRCS}
)
target_link_libraries( kdocumentmanagertest
  kakaocore
  ${KDE4_KIO_LIBS}
  ${QT_QTTEST_LIBRARY}
)
