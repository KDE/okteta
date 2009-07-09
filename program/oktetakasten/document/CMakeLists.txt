include_directories(
  ../../../core
  ../../../libs/kasten/core
  ../../../libs/kasten/core/entity
  ../../../libs/kasten/core/document
)

if(KDE4_BUILD_TESTS)
  add_subdirectory(tests)
endif(KDE4_BUILD_TESTS)

set( OKTETA_KASTEN_DOCUMENT_SRCS
  kbytearraydocumentfactory.cpp
  kbytearraydocument.cpp
  kbytearrayselection.cpp
)

kde4_add_library( okteta_kastendocument STATIC ${OKTETA_KASTEN_DOCUMENT_SRCS} )
