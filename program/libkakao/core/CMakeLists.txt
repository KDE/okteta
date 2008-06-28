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
  document/abstractmodel.cpp
  document/abstractmodelselection.cpp
  document/kabstractdocument.cpp
  document/kabstractdocumentfactory.cpp
  document/kdocumentversiondata.cpp
  io/abstractmodelstreamencoder.cpp
  io/abstractmodelexporter.cpp
  io/abstractmodelfilesystemexporter.cpp
  io/modelencoderfilesystemexporter.cpp
  io/kabstractdocumentsynchronizer.cpp
  io/kabstractdocumentsynchronizerfactory.cpp
  io/kabstractdocumentfilesystemsynchronizer.cpp
  system/jobmanager.cpp
  system/kdocumentmanager.cpp
  system/kdocumentcreatemanager.cpp
  system/kdocumentsyncmanager.cpp
  system/modelcodecmanager.cpp
)

kde4_add_library( kakaocore STATIC ${KAKAO_CORE_SRCS} )
