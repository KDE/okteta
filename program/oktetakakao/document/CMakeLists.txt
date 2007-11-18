include_directories(
  ../../../core
  ../../libkakao/core/document
)

if(KDE4_BUILD_TESTS)
  add_subdirectory(tests)
endif(KDE4_BUILD_TESTS)

set( OKTETA_KAKAO_CORE_SRCS
  kbytearraydocumentfactory.cpp
  kbytearrayrawfilesynchronizer.cpp
  kbytearraydocument.cpp
)

kde4_add_library( okteta_kakaocore STATIC ${OKTETA_KAKAO_CORE_SRCS} )
