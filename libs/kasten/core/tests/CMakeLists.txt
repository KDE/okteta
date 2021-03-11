set( EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR} )

set( SUTDIR ${CMAKE_CURRENT_SOURCE_DIR}/.. )

include_directories(
  ${CMAKE_CURRENT_BINARY_DIR}/..
  ${SUTDIR}/..
  ${SUTDIR}/document
  ${SUTDIR}/io
  ${SUTDIR}/io/filesystem
  ${SUTDIR}
)

########### util ###############

set( kastencoretestutil_LIB_SRCS
  util/filesystem.cpp
)

add_library( kastencoretestutil  STATIC ${kastencoretestutil_LIB_SRCS} )
target_link_libraries( kastencoretestutil Qt5::Test Qt5::Core)

########### io ###############

set( kastencoretestio_LIB_SRCS
  testdocumentfileloadthread.cpp
  testdocumentfilereloadthread.cpp
  testdocumentfilewritethread.cpp
  testdocumentfileloadjob.cpp
  testdocumentfileconnectjob.cpp
  testdocumentfilewritejob.cpp
  testdocumentfilereloadjob.cpp
  testdocumentfilewritetojob.cpp
)

add_library( kastencoretestio  STATIC ${kastencoretestio_LIB_SRCS} )
target_link_libraries( kastencoretestio
PUBLIC
  Qt5::Core
PRIVATE
  KF5::I18n
  KF5::CoreAddons
)

########### testdocumenttest ###############

ecm_add_test(
  testdocument.cpp
  testdocumenttest.cpp
  TEST_NAME testdocumenttest
  NAME_PREFIX libkasten-core-
  LINK_LIBRARIES
  KastenCore
  Qt5::Test
)

########### testdocumentfilesynchronizertest ###############

ecm_add_test(
  testdocument.cpp
  testdocumentfilesynchronizer.cpp
  testdocumentfilesynchronizertest.cpp
  TEST_NAME testdocumentfilesynchronizertest
  NAME_PREFIX libkasten-core-
  LINK_LIBRARIES
  kastencoretestio
  KastenCore
  kastencoretestutil
  Qt5::Test
)

############ testdocumentfilesynchronizerfactorytest ###############

ecm_add_test(
  testdocument.cpp
  testdocumentfilesynchronizer.cpp
  testdocumentfilesynchronizerfactory.cpp
  testdocumentfilesynchronizerfactorytest.cpp
  TEST_NAME testdocumentfilesynchronizerfactorytest
  NAME_PREFIX libkasten-core-
  LINK_LIBRARIES
  kastencoretestio
  KastenCore
  kastencoretestutil
  Qt5::Test
)

########### documentmanagertest ###############

ecm_add_test(
  testdocument.cpp
  documentmanagertest.cpp
  TEST_NAME documentmanagertest
  NAME_PREFIX libkasten-core-
  LINK_LIBRARIES
  KastenCore
  Qt5::Test
)
