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
  document/kbytearraydocumentfactory.cpp
  document/kbytearraydocument.cpp
  document/kbytearrayselection.cpp
)

set( okteta_kasten_io_filesystem_SRCS
  io/filesystem/bytearrayrawfileloadthread.cpp
  io/filesystem/bytearrayrawfilereloadthread.cpp
  io/filesystem/bytearrayrawfilewritethread.cpp
  io/filesystem/bytearrayrawfileloadjob.cpp
  io/filesystem/bytearrayrawfileconnectjob.cpp
  io/filesystem/bytearrayrawfilewritejob.cpp
  io/filesystem/bytearrayrawfilereloadjob.cpp
  io/filesystem/bytearrayrawfilewritetojob.cpp
  io/filesystem/kbytearrayrawfilesynchronizer.cpp
  io/filesystem/kbytearrayrawfilesynchronizerfactory.cpp
)


set( okteta_kastencore_io_SRCS
  ${okteta_kasten_io_filesystem_SRCS}
)

set( okteta_kastencore_LIB_SRCS
  ${okteta_kastencore_document_SRCS}
  ${okteta_kastencore_io_SRCS}
)


kde4_add_library( okteta_kastencore  STATIC ${okteta_kastencore_LIB_SRCS} )


if(KDE4_BUILD_TESTS)
  add_subdirectory(document/tests)
  add_subdirectory(io/tests)
endif(KDE4_BUILD_TESTS)
