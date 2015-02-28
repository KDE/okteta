add_definitions(-DTRANSLATION_DOMAIN=\"liboktetakasten\")

include_directories(
  document
)

set( okteta_kastencore_document_SRCS
  document/bytearraydocumentfactory.cpp
  document/bytearraydocument.cpp
  document/bytearrayselection.cpp
)

set( okteta_kastencore_io_filesystem_SRCS
#   io/filesystem/externalbookmarkstorage.cpp
  io/filesystem/bytearrayrawfileloadthread.cpp
  io/filesystem/bytearrayrawfilereloadthread.cpp
  io/filesystem/bytearrayrawfilewritethread.cpp
  io/filesystem/bytearrayrawfileloadjob.cpp
  io/filesystem/bytearrayrawfileconnectjob.cpp
  io/filesystem/bytearrayrawfilewritejob.cpp
  io/filesystem/bytearrayrawfilereloadjob.cpp
  io/filesystem/bytearrayrawfilewritetojob.cpp
  io/filesystem/bytearrayrawfilesynchronizer.cpp
  io/filesystem/bytearrayrawfilesynchronizerfactory.cpp
)


set( okteta_kastencore_io_SRCS
  ${okteta_kastencore_io_filesystem_SRCS}
)

set( okteta_kastencore_LIB_SRCS
  ${okteta_kastencore_document_SRCS}
  ${okteta_kastencore_io_SRCS}
)

set( okteta_kastencore_LIB_HDRS
  ${CMAKE_CURRENT_BINARY_DIR}/oktetakastencore_export.h
  document/bytearrayselection.h
  document/bytearraydocument.h
  document/bytearraydocumentfactory.h
  io/filesystem/bytearrayrawfilesynchronizer.h
  io/filesystem/bytearrayrawfilesynchronizerfactory.h
)

add_library( OktetaKastenCore  SHARED ${okteta_kastencore_LIB_SRCS} )
generate_export_header( OktetaKastenCore )

target_link_libraries( OktetaKastenCore
PUBLIC
  KastenCore
  OktetaCore
PRIVATE
  KF5::I18n
  KF5::Bookmarks
)

set_target_properties( OktetaKastenCore PROPERTIES
  OUTPUT_NAME kasten${KASTEN_ABI_VERSION}okteta${OKTETAKASTEN_ABI_VERSION}core
  VERSION     ${OKTETAKASTEN_LIB_VERSION}
  SOVERSION   ${OKTETAKASTEN_LIB_SOVERSION}
)

install( TARGETS OktetaKastenCore  ${INSTALL_TARGETS_DEFAULT_ARGS} )

install( FILES ${okteta_kastencore_LIB_HDRS}
  DESTINATION ${OKTETAKASTEN_INCLUDE_INSTALL_DIR}
  COMPONENT Devel
)


if(BUILD_TESTING)
  add_subdirectory(document/tests)
  add_subdirectory(io/tests)
endif(BUILD_TESTING)
