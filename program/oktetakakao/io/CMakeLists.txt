include_directories(
  ../../../core
  ../../../gui
  ../../../gui/libcolumnsview
  ../../libkakao/core/entity
  ../../libkakao/core/document
  ../../libkakao/core/io
  ../../libkakao/core/io/filesystem
  ../../libkakao/core/io/network
  ../../libkakao/core/io/network/impulsivestreamio
  ../../libkakao/gui/view
  ../document
  ../view
  network/impulsivestreamio
)

if(KDE4_BUILD_TESTS)
  add_subdirectory(tests)
endif(KDE4_BUILD_TESTS)

set( OKTETA_TEXTRENDERER_SRCS
  bytearrayviewtextrenderer/offsetcolumntextrenderer.cpp
  bytearrayviewtextrenderer/bordercolumntextrenderer.cpp
  bytearrayviewtextrenderer/abstractbytearraycolumntextrenderer.cpp
  bytearrayviewtextrenderer/valuebytearraycolumntextrenderer.cpp
  bytearrayviewtextrenderer/charbytearraycolumntextrenderer.cpp
)

set( OKTETA_KAKAO_IO_FILESYSTEM_SRCS
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

set( OKTETA_KAKAO_IO_NETWORK_SRCS
  network/impulsivestreamio/arraychangemetricsimpulsivestreamreader.cpp
  network/impulsivestreamio/arraychangemetricsimpulsivestreamwriter.cpp
#   network/impulsivestreamio/basedataimpulsivestreamreader.cpp
  network/impulsivestreamio/bytearraychangeimpulsivestreamreader.cpp
  network/impulsivestreamio/bytearraychangeimpulsivestreamwriter.cpp
  network/impulsivestreamio/changelistimpulsivestreamreader.cpp
  network/impulsivestreamio/changelistimpulsivestreamwriter.cpp
  network/bytearraynetworkclientconnection.cpp
  network/bytearraynetworkserver.cpp
  network/bytearraynetworkserverconnection.cpp
  network/bytearraynetworkloadjob.cpp
  network/bytearraynetworksynchronizer.cpp
  network/bytearraynetworksynchronizerfactory.cpp
)

set( OKTETA_KAKAO_IO_SRCS
  kabstractbytearraystreamencoder.cpp
  kbytearraysourcecodestreamencoder.cpp
  kbytearrayvaluestreamencoder.cpp
  kbytearraytextstreamencoder.cpp
  ${OKTETA_TEXTRENDERER_SRCS}
  bytearrayviewtextstreamencoder.cpp
  ${OKTETA_KAKAO_IO_FILESYSTEM_SRCS}
  ${OKTETA_KAKAO_IO_NETWORK_SRCS}
)

kde4_add_library( okteta_kakaoio STATIC ${OKTETA_KAKAO_IO_SRCS} )
