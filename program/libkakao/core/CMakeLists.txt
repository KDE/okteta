project( kakaocore )

include_directories(
  entity
  document
  io
  io/filesystem
  system
  # TODO: hack, remove me!
  ../../libprocessdocuments
)

if(KDE4_BUILD_TESTS)
  add_subdirectory( tests )
endif(KDE4_BUILD_TESTS)

set( KAKAO_CORE_SRCS
#   entity/group.cpp
  entity/person.cpp
#   entity/abstractentity.cpp
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
  io/abstractmodelstreamencoder.cpp
  io/abstractexportjob.cpp
  io/abstractmodelexporter.cpp
  io/modelstreamencodethread.cpp
  io/abstractmodelsynchronizer.cpp
  io/abstractmodelsynchronizerfactory.cpp
  io/filesystem/abstractfilesystemloadjob.cpp
  io/filesystem/abstractfilesystemsynctoremotejob.cpp
  io/filesystem/abstractfilesystemsyncfromremotejob.cpp
  io/filesystem/abstractfilesystemsyncwithremotejob.cpp
  io/filesystem/abstractfilesystemconnectjob.cpp
  io/filesystem/abstractfilesystemexportjob.cpp
  io/filesystem/modelencoderfilesystemexportjob.cpp
  io/filesystem/modelencoderfilesystemexporter.cpp
  io/filesystem/abstractmodelfilesystemsynchronizer.cpp
  system/jobmanager.cpp
  system/kdocumentmanager.cpp
  system/kdocumentcreatemanager.cpp
  system/kdocumentsyncmanager.cpp
  system/modelcodecmanager.cpp
)

kde4_add_library( kakaocore STATIC ${KAKAO_CORE_SRCS} )
