project( kakaocore )

include_directories(
  document
  system
)

if(KDE4_BUILD_TESTS)
  add_subdirectory( tests )
endif(KDE4_BUILD_TESTS)

set( KAKAO_CORE_SRCS
  document/kabstractdocument.cpp
  document/kabstractdocumentfactory.cpp
#   document/kcodeprocess.cpp
#   document/kabstractdocumentencoder.cpp
#   document/kabstractdocumentdecoder.cpp
  document/kabstractdocumentsynchronizer.cpp
  document/kabstractdocumentfilesystemsynchronizer.cpp
  system/kdocumentmanager.cpp
  system/kdocumentcreator.cpp
  system/kdocumentloadermanager.cpp
#   system/kdocumentsynchronizer.cpp
)

kde4_add_library( kakaocore STATIC ${KAKAO_CORE_SRCS} )
