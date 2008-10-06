project( kakaocore )

add_definitions( -DKDE_DEFAULT_DEBUG_AREA=1803 )

include_directories(
  entity
  document
  io
  io/filesystem
  io/network
  io/network/impulsivestreamio
  system
)

if(KDE4_BUILD_TESTS)
  add_subdirectory( tests )
endif(KDE4_BUILD_TESTS)

# set( LIB_IMPULSIVESTREAMIO_SRCS
#   libimpulsivestreamio/abstractimpulsivestreamreader.cpp
# )

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

set( KAKAO_CORE_IO_NETWORK_SRCS
  io/network/modelservermanager_p.cpp
  io/network/modelservermanager.cpp
#   io/network/abstractnetworkconnection_p.cpp
#   io/network/abstractnetworkconnection.cpp
  io/network/networkclientconnection_p.cpp
  io/network/networkclientconnection.cpp
  io/network/networkserverconnection_p.cpp
  io/network/networkserverconnection.cpp
  io/network/networkserverconnectionmanager_p.cpp
  io/network/networkserverconnectionmanager.cpp
  io/network/abstractimpulsivestreamreader.cpp
  io/network/abstractmodelnetworkclientconnection_p.cpp
  io/network/abstractmodelnetworkclientconnection.cpp
  io/network/abstractmodelnetworkserver.cpp
  io/network/abstractmodelnetworkserverconnection_p.cpp
  io/network/abstractmodelnetworkserverconnection.cpp
  io/network/abstractmodelnetworksynchronizer.cpp
  io/network/impulsivestreamio/int32impulsivestreamreader.cpp
  io/network/impulsivestreamio/int32impulsivestreamwriter.cpp
  io/network/impulsivestreamio/qbytearrayimpulsivestreamreader.cpp
  io/network/impulsivestreamio/qbytearrayimpulsivestreamwriter.cpp
  io/network/impulsivestreamio/qstringimpulsivestreamreader.cpp
  io/network/impulsivestreamio/qstringimpulsivestreamwriter.cpp
  io/network/impulsivestreamio/qimageimpulsivestreamreader.cpp
  io/network/impulsivestreamio/qimageimpulsivestreamwriter.cpp
  io/network/impulsivestreamio/personimpulsivestreamreader.cpp
  io/network/impulsivestreamio/personimpulsivestreamwriter.cpp
  io/network/impulsivestreamio/setuprequestimpulsivestreamreader.cpp
  io/network/impulsivestreamio/setuprequestimpulsivestreamwriter.cpp
  io/network/impulsivestreamio/setupreplyimpulsivestreamreader.cpp
  io/network/impulsivestreamio/setupreplyimpulsivestreamwriter.cpp
  io/network/impulsivestreamio/userlistimpulsivestreamreader.cpp
  io/network/impulsivestreamio/userlistimpulsivestreamwriter.cpp
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
  ${KAKAO_CORE_IO_NETWORK_SRCS}
  ${KAKAO_CORE_SYSTEM_SRCS}
)

kde4_add_library( kakaocore STATIC ${KAKAO_CORE_SRCS} )
