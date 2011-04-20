project( oktetakasten_io_tests )

set( SUTDIR ${CMAKE_CURRENT_SOURCE_DIR}/.. )

# make sure to not use KDE_EXPORT or KDE_IMPORT because we link statically
# against a small part of the libs
add_definitions(-DOKTETACORE_EXPORT=)
add_definitions(-DOKTETAKASTENCORE_EXPORT=)
set( EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR} )


include_directories(
  ${SUTDIR}
  ${SUTDIR}/..
  ${SUTDIR}/../document
  ${SUTDIR}/filesystem
  ${SUTDIR}/../../../core
  ${SUTDIR}/../../../libs/kasten/core/document
  ${SUTDIR}/../../../libs/kasten/core/io
  ${SUTDIR}/../../../core/tests
  ${SUTDIR}/../../../libs/kasten/core/tests
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


########### bytearrayrawfilesynchronizertest ###############

set( bytearrayrawfilesynchronizertest_SRCS
  bytearrayrawfilesynchronizertest.cpp
  ${SUTDIR}/../document/bytearraydocument.cpp
  ${SUTDIR}/filesystem/bytearrayrawfilesynchronizer.cpp
  ${SUTDIR}/filesystem/externalbookmarkstorage.cpp
)

kde4_add_unit_test( bytearrayrawfilesynchronizertest
  TESTNAME oktetakasten-io-bytearrayrawfilesynchronizertest
  ${bytearrayrawfilesynchronizertest_SRCS}
)
target_link_libraries( bytearrayrawfilesynchronizertest
  oktetakastenio
  ${KASTENCORE_LIBS}
  kastencoretestutil
  oktetacoretestutil
  testoktetacore
  ${KDE4_KIO_LIBS}
  ${QT_QTTEST_LIBRARY}
)

########### bytearrayrawfilesynchronizerfactorytest ###############

set( bytearrayrawfilesynchronizerfactorytest_SRCS
  bytearrayrawfilesynchronizerfactorytest.cpp
  ${SUTDIR}/../document/bytearraydocument.cpp
  ${SUTDIR}/filesystem/externalbookmarkstorage.cpp
  ${SUTDIR}/filesystem/bytearrayrawfilesynchronizer.cpp
  ${SUTDIR}/filesystem/bytearrayrawfilesynchronizerfactory.cpp
)

kde4_add_unit_test( bytearrayrawfilesynchronizerfactorytest
  TESTNAME oktetakasten-io-bytearrayrawfilesynchronizerfactorytest
  ${bytearrayrawfilesynchronizerfactorytest_SRCS}
)
target_link_libraries( bytearrayrawfilesynchronizerfactorytest
  oktetakastenio
  ${KASTENCORE_LIBS}
  kastencoretestutil
  oktetacoretestutil
  testoktetacore
  ${KDE4_KIO_LIBS}
  ${QT_QTTEST_LIBRARY}
)
