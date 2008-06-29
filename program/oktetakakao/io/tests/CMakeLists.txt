project( oktetakakao_io_tests )

set( SUTDIR ${CMAKE_CURRENT_SOURCE_DIR}/.. )

# make sure to not use KDE_EXPORT or KDE_IMPORT because we link statically
# against a small part of the libs
add_definitions(-DOKTETACORE_EXPORT=)
set( EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR} )


include_directories(
  ${SUTDIR}
  ${SUTDIR}/../document
  ${SUTDIR}/../../../core
  ${SUTDIR}/../../libkakao/core/document
  ${SUTDIR}/../../libkakao/core/io
  ${SUTDIR}/../../../core/tests
  ${SUTDIR}/../../libkakao/core/tests
)

########### io ###############

set( oktetakakaoio_LIB_SRCS
  ${SUTDIR}/bytearrayrawfileloadthread.cpp
  ${SUTDIR}/bytearrayrawfilereloadthread.cpp
  ${SUTDIR}/bytearrayrawfilewritethread.cpp
  ${SUTDIR}/bytearrayrawfileloadjob.cpp
  ${SUTDIR}/bytearrayrawfileconnectjob.cpp
  ${SUTDIR}/bytearrayrawfilewritejob.cpp
  ${SUTDIR}/bytearrayrawfilereloadjob.cpp
  ${SUTDIR}/bytearrayrawfilewritetojob.cpp
)

kde4_add_library( oktetakakaoio  STATIC ${oktetakakaoio_LIB_SRCS} )


########### kbytearrayrawfilesynchronizertest ###############

set( kbytearrayrawfilesynchronizertest_SRCS
  kbytearrayrawfilesynchronizertest.cpp
  ${SUTDIR}/../document/kbytearraydocument.cpp
  ${SUTDIR}/kbytearrayrawfilesynchronizer.cpp
)

kde4_add_unit_test( kbytearrayrawfilesynchronizertest
  TESTNAME oktetakakao-io-kbytearrayrawfilesynchronizertest
  ${kbytearrayrawfilesynchronizertest_SRCS}
)
target_link_libraries( kbytearrayrawfilesynchronizertest
  oktetakakaoio
  kakaocore
  kakaocoretestutil
  oktetacoretestutil
  testoktetacore
  ${KDE4_KIO_LIBS}
  ${QT_QTTEST_LIBRARY}
)

########### kbytearrayrawfilesynchronizerfactorytest ###############

set( kbytearrayrawfilesynchronizerfactorytest_SRCS
  kbytearrayrawfilesynchronizerfactorytest.cpp
  ${SUTDIR}/../document/kbytearraydocument.cpp
  ${SUTDIR}/kbytearrayrawfilesynchronizer.cpp
  ${SUTDIR}/kbytearrayrawfilesynchronizerfactory.cpp
)

kde4_add_unit_test( kbytearrayrawfilesynchronizerfactorytest
  TESTNAME oktetakakao-io-kbytearrayrawfilesynchronizerfactorytest
  ${kbytearrayrawfilesynchronizerfactorytest_SRCS}
)
target_link_libraries( kbytearrayrawfilesynchronizerfactorytest
  oktetakakaoio
  kakaocore
  kakaocoretestutil
  oktetacoretestutil
  testoktetacore
  ${KDE4_KIO_LIBS}
  ${QT_QTTEST_LIBRARY}
)
