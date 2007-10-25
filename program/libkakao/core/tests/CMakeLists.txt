project( kakao_core_tests )


set( EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR} )

set( SUTDIR ${CMAKE_CURRENT_SOURCE_DIR}/.. )

include_directories(
  ${SUTDIR}
)


########### testdocumenttest ###############

set( testdocumenttest_SRCS
  testdocument.cpp
  testdocumenttest.cpp
)

kde4_add_executable( testdocumenttest  RUN_UNINSTALLED ${testdocumenttest_SRCS} )
target_link_libraries( testdocumenttest
  kakaocore
  ${KDE4_KIO_LIBS}
  ${QT_QTTEST_LIBRARY}
)

add_test( libkakao-testdocumenttest ${EXECUTABLE_OUTPUT_PATH}/testdocumenttest)

########### kdocumentmanagertest ###############

set( kdocumentmanagertest_SRCS
  testdocument.cpp
  kdocumentmanagertest.cpp
)

kde4_add_executable( kdocumentmanagertest  RUN_UNINSTALLED ${kdocumentmanagertest_SRCS} )
target_link_libraries( kdocumentmanagertest
  kakaocore
  ${KDE4_KIO_LIBS}
  ${QT_QTTEST_LIBRARY}
)

add_test( libkakao-kdocumentmanagertest ${EXECUTABLE_OUTPUT_PATH}/kdocumentmanagertest)

