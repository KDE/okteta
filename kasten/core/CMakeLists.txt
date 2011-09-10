set( KASTEN_REL_DIR ../../libs/kasten )
set( OKTETA_REL_DIR ../.. )

include_directories(
  document
  ${KASTEN_REL_DIR}/core
  ${KASTEN_REL_DIR}/core/document
  ${KASTEN_REL_DIR}/core/entity
  ${KASTEN_REL_DIR}/core/io
  ${KASTEN_REL_DIR}/core/io/filesystem
  ${OKTETA_REL_DIR}/core
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
  oktetakastencore.cpp
  ${okteta_kastencore_document_SRCS}
  ${okteta_kastencore_io_SRCS}
)

set( okteta_kastencore_LIB_HDRS
  oktetakastencore_export.h
  document/bytearrayselection.h
  document/bytearraydocument.h
  document/bytearraydocumentfactory.h
  io/filesystem/bytearrayrawfilesynchronizer.h
  io/filesystem/bytearrayrawfilesynchronizerfactory.h
)

kde4_add_library( ${oktetakastencore_LIB}  SHARED ${okteta_kastencore_LIB_SRCS} )

target_link_libraries( ${oktetakastencore_LIB}
  ${KASTENCORE_LIBS}
  ${oktetacore_LIB}
  ${KDE4_KDECORE_LIBS}
)
set_target_properties( ${oktetakastencore_LIB} PROPERTIES
  VERSION   ${OKTETAKASTEN_LIB_VERSION}
  SOVERSION ${OKTETAKASTEN_LIB_SOVERSION}
)

install( TARGETS ${oktetakastencore_LIB}  ${INSTALL_TARGETS_DEFAULT_ARGS} )

install( FILES ${okteta_kastencore_LIB_HDRS}
  DESTINATION ${OKTETAKASTEN_INCLUDE_INSTALL_DIR}
  COMPONENT Devel
)


if(KDE4_BUILD_TESTS)
  add_subdirectory(document/tests)
  add_subdirectory(io/tests)
endif(KDE4_BUILD_TESTS)
