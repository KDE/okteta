include_directories(
  ../../../core
  ../../libkakao/core/document
  ../../libkakao/core/io
  ../document
)

if(KDE4_BUILD_TESTS)
  add_subdirectory(tests)
endif(KDE4_BUILD_TESTS)

set( OKTETA_KAKAO_IO_SRCS
  kabstractbytearraystreamencoder.cpp
  kbytearraysourcecodestreamencoder.cpp
  kbytearrayvaluestreamencoder.cpp
  kbytearraytextstreamencoder.cpp
  kbytearrayrawfilesynchronizer.cpp
  kbytearrayrawfilesynchronizerfactory.cpp
)

kde4_add_library( okteta_kakaoio STATIC ${OKTETA_KAKAO_IO_SRCS} )
