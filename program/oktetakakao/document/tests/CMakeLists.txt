project( oktetakakao_document_tests )


set( EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR} )

set( SUTDIR ${CMAKE_CURRENT_SOURCE_DIR}/.. )

include_directories(
  ${SUTDIR}
  ${SUTDIR}/../../../core
  ${SUTDIR}/../../libkakao/core/document
  ${SUTDIR}/../../../core/tests
)

########### util ###############

set( oktetakakaodocumenttestutil_LIB_SRCS
  util/filesystem.cpp
)

kde4_add_library( oktetakakaodocumenttestutil  STATIC ${oktetakakaodocumenttestutil_LIB_SRCS} )

########### kbytearraydocumenttest ###############

set( kbytearraydocumenttest_SRCS
  kbytearraydocumenttest.cpp
  ${SUTDIR}/kbytearraydocument.cpp
)

kde4_add_executable( kbytearraydocumenttest  RUN_UNINSTALLED ${kbytearraydocumenttest_SRCS} )
target_link_libraries( kbytearraydocumenttest
  oktetakakaodocumenttestutil
  kakaocore oktetacoretestutil
  testoktetacore
  ${KDE4_KDECORE_LIBS} ${QT_QTTEST_LIBRARY}
)

add_test( okteta-kbytearraydocumenttest ${EXECUTABLE_OUTPUT_PATH}/kbytearraydocumenttest)

