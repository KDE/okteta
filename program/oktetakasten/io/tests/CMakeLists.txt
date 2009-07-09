project( oktetakasten_io_tests )

set( SUTDIR ${CMAKE_CURRENT_SOURCE_DIR}/.. )

# make sure to not use KDE_EXPORT or KDE_IMPORT because we link statically
# against a small part of the libs
add_definitions(-DOKTETACORE_EXPORT=)
set( EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR} )


include_directories(
  ${SUTDIR}
  ${SUTDIR}/../document
  ${SUTDIR}/filesystem
  ${SUTDIR}/../../../core
  ${SUTDIR}/../../libkasten/core/document
  ${SUTDIR}/../../libkasten/core/io
  ${SUTDIR}/../../../core/tests
  ${SUTDIR}/../../libkasten/core/tests
)

########### io ###############

set( oktetakastenio_LIB_SRCS
  ${SUTDIR}/filesystem/bytearrayrawfileloadthread.cpp
  ${SUTDIR}/filesystem/bytearrayrawfilereloadthread.cpp
  ${SUTDIR}/filesystem/bytearrayrawfilewritethread.cpp
  ${SUTDIR}/filesystem/bytearrayrawfileloadjob.cpp
  ${SUTDIR}/filesystem/bytearrayrawfileconnectjob.cpp
  ${SUTDIR}/filesystem/bytearrayrawfilewritejob.cpp
  ${SUTDIR}/filesystem/bytearrayrawfilereloadjob.cpp
  ${SUTDIR}/filesystem/bytearrayrawfilewritetojob.cpp
)

kde4_add_library( oktetakastenio  STATIC ${oktetakastenio_LIB_SRCS} )


########### kbytearrayrawfilesynchronizertest ###############

set( kbytearrayrawfilesynchronizertest_SRCS
  kbytearrayrawfilesynchronizertest.cpp
  ${SUTDIR}/../document/kbytearraydocument.cpp
  ${SUTDIR}/filesystem/kbytearrayrawfilesynchronizer.cpp
)

kde4_add_unit_test( kbytearrayrawfilesynchronizertest
  TESTNAME oktetakasten-io-kbytearrayrawfilesynchronizertest
  ${kbytearrayrawfilesynchronizertest_SRCS}
)
target_link_libraries( kbytearrayrawfilesynchronizertest
  oktetakastenio
  kastencore
  kastencoretestutil
  oktetacoretestutil
  testoktetacore
  ${KDE4_KIO_LIBS}
  ${QT_QTTEST_LIBRARY}
)

########### kbytearrayrawfilesynchronizerfactorytest ###############

set( kbytearrayrawfilesynchronizerfactorytest_SRCS
  kbytearrayrawfilesynchronizerfactorytest.cpp
  ${SUTDIR}/../document/kbytearraydocument.cpp
  ${SUTDIR}/filesystem/kbytearrayrawfilesynchronizer.cpp
  ${SUTDIR}/filesystem/kbytearrayrawfilesynchronizerfactory.cpp
)

kde4_add_unit_test( kbytearrayrawfilesynchronizerfactorytest
  TESTNAME oktetakasten-io-kbytearrayrawfilesynchronizerfactorytest
  ${kbytearrayrawfilesynchronizerfactorytest_SRCS}
)
target_link_libraries( kbytearrayrawfilesynchronizerfactorytest
  oktetakastenio
  kastencore
  kastencoretestutil
  oktetacoretestutil
  testoktetacore
  ${KDE4_KIO_LIBS}
  ${QT_QTTEST_LIBRARY}
)
