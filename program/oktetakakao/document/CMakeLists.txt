include_directories(
  ../../../core
  ../../libkakao/core/document
)

if(KDE4_BUILD_TESTS)
  add_subdirectory(tests)
endif(KDE4_BUILD_TESTS)

set( OKTETA_KAKAO_DOCUMENT_SRCS
  kbytearraydocumentfactory.cpp
  kbytearraydocument.cpp
  kbytearrayselection.cpp
)

kde4_add_library( okteta_kakaodocument STATIC ${OKTETA_KAKAO_DOCUMENT_SRCS} )
