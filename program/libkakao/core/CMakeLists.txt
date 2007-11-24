project( kakaocore )

include_directories(
  document
  io
  system
)

if(KDE4_BUILD_TESTS)
  add_subdirectory( tests )
endif(KDE4_BUILD_TESTS)

set( KAKAO_CORE_SRCS
  document/kabstractdocument.cpp
  document/kabstractdocumentselection.cpp
  document/kabstractdocumentfactory.cpp
  io/kabstractdocumentstreamencoder.cpp
  io/kabstractdocumentsynchronizer.cpp
  io/kabstractdocumentsynchronizerfactory.cpp
  io/kabstractdocumentfilesystemsynchronizer.cpp
  system/kdocumentmanager.cpp
  system/kdocumentcreatemanager.cpp
  system/kdocumentsyncmanager.cpp
  system/kdocumentcodecmanager.cpp
)

kde4_add_library( kakaocore STATIC ${KAKAO_CORE_SRCS} )
