project( kakaocore )

add_definitions( -DKDE_DEFAULT_DEBUG_AREA=1803 )

include_directories(
  entity
  document
  io
  io/filesystem
  system
)

if(KDE4_BUILD_TESTS)
  add_subdirectory( tests )
endif(KDE4_BUILD_TESTS)


set( KAKAO_CORE_ENTITY_SRCS
#   entity/group.cpp
  entity/person.cpp
#   entity/abstractentity.cpp
)

set( KAKAO_CORE_DOCUMENT_SRCS
  document/abstractmodel.cpp
  document/abstractmodelselection.cpp
  document/abstracttool.cpp
  document/kabstractdocument.cpp
  document/kabstractdocumentfactory.cpp
  document/kdocumentversiondata.cpp
)

set( KAKAO_CORE_IO_SRCS
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
)

set( KAKAO_CORE_IO_FILESYSTEM_SRCS
  io/filesystem/abstractfilesystemloadjob.cpp
  io/filesystem/abstractfilesystemsynctoremotejob.cpp
  io/filesystem/abstractfilesystemsyncfromremotejob.cpp
  io/filesystem/abstractfilesystemsyncwithremotejob.cpp
  io/filesystem/abstractfilesystemconnectjob.cpp
  io/filesystem/abstractfilesystemexportjob.cpp
  io/filesystem/modelencoderfilesystemexportjob.cpp
  io/filesystem/modelencoderfilesystemexporter.cpp
  io/filesystem/abstractmodelfilesystemsynchronizer.cpp
)

set( KAKAO_CORE_SYSTEM_SRCS
  system/jobmanager.cpp
  system/kdocumentmanager.cpp
  system/kdocumentcreatemanager.cpp
  system/kdocumentsyncmanager.cpp
  system/modelcodecmanager.cpp
)

set( KAKAO_CORE_SRCS
  ${KAKAO_CORE_ENTITY_SRCS}
  ${KAKAO_CORE_DOCUMENT_SRCS}
  ${KAKAO_CORE_IO_SRCS}
  ${KAKAO_CORE_IO_FILESYSTEM_SRCS}
  ${KAKAO_CORE_SYSTEM_SRCS}
)

kde4_add_library( kakaocore STATIC ${KAKAO_CORE_SRCS} )
