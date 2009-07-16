include_directories(
  ../view
  ../document
  ../../../libs/kasten/gui
  ../../../libs/kasten/gui/view
  ../../../libs/kasten/gui/io
  ../../../libs/kasten/core
  ../../../libs/kasten/core/entity
  ../../../libs/kasten/core/document
  ../../../libs/kasten/core/io
  ../../../libs/kasten/core/io/filesystem
  ../../../gui
  ../../../gui/libcolumnsview
  ../../../core
)

if(KDE4_BUILD_TESTS)
  add_subdirectory(tests)
endif(KDE4_BUILD_TESTS)

set( OKTETA_TEXTRENDERER_SRCS
  bytearrayviewtextrenderer/abstractcolumntextrenderer.cpp
  bytearrayviewtextrenderer/offsetcolumntextrenderer.cpp
  bytearrayviewtextrenderer/bordercolumntextrenderer.cpp
  bytearrayviewtextrenderer/abstractbytearraycolumntextrenderer.cpp
  bytearrayviewtextrenderer/valuebytearraycolumntextrenderer.cpp
  bytearrayviewtextrenderer/charbytearraycolumntextrenderer.cpp
  bytearrayviewtextrenderer/bytearrayrowscolumntextrenderer.cpp
)

set( OKTETA_KASTEN_IO_FILESYSTEM_SRCS
  filesystem/bytearrayrawfileloadthread.cpp
  filesystem/bytearrayrawfilereloadthread.cpp
  filesystem/bytearrayrawfilewritethread.cpp
  filesystem/bytearrayrawfileloadjob.cpp
  filesystem/bytearrayrawfileconnectjob.cpp
  filesystem/bytearrayrawfilewritejob.cpp
  filesystem/bytearrayrawfilereloadjob.cpp
  filesystem/bytearrayrawfilewritetojob.cpp
  filesystem/kbytearrayrawfilesynchronizer.cpp
  filesystem/kbytearrayrawfilesynchronizerfactory.cpp
)

set( OKTETA_VALUES_STREAMENCODER_SRCS
  bytearrayvaluesstreamencoderconfigeditor.cpp
  bytearrayvaluesstreamencoderconfigeditorfactory.cpp
  kbytearrayvaluestreamencoder.cpp
)

set( OKTETA_SOURCECODE_STREAMENCODER_SRCS
  bytearraysourcecodestreamencoderconfigeditor.cpp
  bytearraysourcecodestreamencoderconfigeditorfactory.cpp
  kbytearraysourcecodestreamencoder.cpp
)

set( OKTETA_KASTEN_IO_SRCS
  kabstractbytearraystreamencoder.cpp
  bytearraytextstreamencoderpreview.cpp
  ${OKTETA_VALUES_STREAMENCODER_SRCS}
  kbytearraytextstreamencoder.cpp
  ${OKTETA_SOURCECODE_STREAMENCODER_SRCS}
  ${OKTETA_TEXTRENDERER_SRCS}
  bytearrayviewtextstreamencoder.cpp
  ${OKTETA_KASTEN_IO_FILESYSTEM_SRCS}
)

kde4_add_library( okteta_kastenio STATIC ${OKTETA_KASTEN_IO_SRCS} )
