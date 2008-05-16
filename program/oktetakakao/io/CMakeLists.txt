include_directories(
  ../../../core
  ../../../gui
  ../../../gui/libcolumnsview
  ../../libkakao/core/document
  ../../libkakao/core/io
  ../../libkakao/gui/view
  ../document
  ../view
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

set( OKTETA_KAKAO_IO_SRCS
  kabstractbytearraystreamencoder.cpp
  kbytearraysourcecodestreamencoder.cpp
  kbytearrayvaluestreamencoder.cpp
  kbytearraytextstreamencoder.cpp
  ${OKTETA_TEXTRENDERER_SRCS}
  bytearrayviewtextstreamencoder.cpp
  kbytearrayrawfilesynchronizer.cpp
  kbytearrayrawfilesynchronizerfactory.cpp
)

kde4_add_library( okteta_kakaoio STATIC ${OKTETA_KAKAO_IO_SRCS} )
