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
  io/filesystem/externalbookmarkstorage.cpp
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

set( oktete_kastencore_LIB_HDRS
  oktetakastencore_export.h
  document/bytearrayselection.h
  document/bytearraydocument.h
  document/bytearraydocumentfactory.h
  io/filesystem/bytearrayrawfilesynchronizer.h
  io/filesystem/bytearrayrawfilesynchronizerfactory.h
)

kde4_add_library( oktetakastencore  SHARED ${okteta_kastencore_LIB_SRCS} )

target_link_libraries( oktetakastencore
  kastencore
  oktetacore
  ${KDE4_KIO_LIBS}
)
set_target_properties( oktetakastencore  PROPERTIES VERSION ${GENERIC_LIB_VERSION} SOVERSION ${GENERIC_LIB_SOVERSION} )

install( TARGETS oktetakastencore  ${INSTALL_TARGETS_DEFAULT_ARGS} )

# install( FILES ${oktete_kastencore_LIB_HDRS}  DESTINATION  ${INCLUDE_INSTALL_DIR}/kasten  COMPONENT Devel )


if(KDE4_BUILD_TESTS)
  add_subdirectory(document/tests)
  add_subdirectory(io/tests)
endif(KDE4_BUILD_TESTS)
