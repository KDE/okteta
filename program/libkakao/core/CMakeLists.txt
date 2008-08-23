project( kakaocore )

include_directories(
  document
  io
  system
  # TODO: hack, remove me!
  ../../libprocessdocuments
)

if(KDE4_BUILD_TESTS)
  add_subdirectory( tests )
endif(KDE4_BUILD_TESTS)

set( KAKAO_CORE_SRCS
  document/abstractmodel.cpp
  document/abstractmodelselection.cpp
  document/abstracttool.cpp
  document/kabstractdocument.cpp
  document/kabstractdocumentfactory.cpp
  document/kdocumentversiondata.cpp
  io/abstractloadjob.cpp
  io/abstractconnectjob.cpp
  io/abstractsynctoremotejob.cpp
  io/abstractsyncfromremotejob.cpp
  io/abstractsyncwithremotejob.cpp
  io/abstractfilesystemloadjob.cpp
  io/abstractfilesystemsynctoremotejob.cpp
  io/abstractfilesystemsyncfromremotejob.cpp
  io/abstractfilesystemsyncwithremotejob.cpp
  io/abstractfilesystemconnectjob.cpp
  io/abstractmodelstreamencoder.cpp
  io/abstractexportjob.cpp
  io/abstractfilesystemexportjob.cpp
  io/abstractmodelexporter.cpp
  io/modelstreamencodethread.cpp
  io/modelencoderfilesystemexportjob.cpp
  io/modelencoderfilesystemexporter.cpp
  io/abstractmodelsynchronizer.cpp
  io/abstractmodelsynchronizerfactory.cpp
  io/abstractmodelfilesystemsynchronizer.cpp
  system/jobmanager.cpp
  system/kdocumentmanager.cpp
  system/kdocumentcreatemanager.cpp
  system/kdocumentsyncmanager.cpp
  system/modelcodecmanager.cpp
)

kde4_add_library( kakaocore STATIC ${KAKAO_CORE_SRCS} )
