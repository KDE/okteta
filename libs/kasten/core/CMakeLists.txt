project( kastencore )

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


set( KASTEN_CORE_ENTITY_SRCS
#   entity/group.cpp
  entity/person.cpp
#   entity/abstractentity.cpp
)

set( KASTEN_CORE_DOCUMENT_SRCS
  document/abstractmodel.cpp
  document/abstractmodelselection.cpp
  document/abstracttool.cpp
  document/kabstractdocument.cpp
  document/kabstractdocumentfactory.cpp
  document/kdocumentversiondata.cpp
)

set( KASTEN_CORE_IO_SRCS
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

set( KASTEN_CORE_IO_FILESYSTEM_SRCS
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

set( KASTEN_CORE_SYSTEM_SRCS
  system/jobmanager.cpp
  system/kdocumentmanager.cpp
  system/kdocumentcreatemanager.cpp
  system/kdocumentsyncmanager.cpp
  system/modelcodecmanager.cpp
)

set( kastencore_LIB_SRCS
  ${KASTEN_CORE_ENTITY_SRCS}
  ${KASTEN_CORE_DOCUMENT_SRCS}
  ${KASTEN_CORE_IO_SRCS}
  ${KASTEN_CORE_IO_FILESYSTEM_SRCS}
  ${KASTEN_CORE_SYSTEM_SRCS}
)

set( kastencore_LIB_HDRS
  kastencore_export.h
  document/abstractmodel.h
  document/abstractmodelselection.h
  document/kabstractdocument.h
  document/kabstractdocumentfactory.h
  document/abstracttool.h
  document/kdocumentversiondata.h
  document/kdocumentversionid.h
  document/kiversionable.h
  document/userlistable.h
  entity/person.h
  io/abstractsynctoremotejob.h
  io/abstractmodelsynchronizer.h
  io/abstractsyncwithremotejob.h
  io/filesystem/abstractfilesystemsyncwithremotejob.h
  io/filesystem/abstractfilesystemsyncfromremotejob.h
  io/filesystem/abstractfilesystemsynctoremotejob.h
  io/filesystem/abstractmodelfilesystemsynchronizer.h
  io/filesystem/abstractfilesystemconnectjob.h
  io/filesystem/abstractfilesystemloadjob.h
  io/filesystem/abstractfilesystemexportjob.h
  io/filesystem/modelencoderfilesystemexporter.h
  io/filesystem/modelencoderfilesystemexportjob.h
  io/abstractsyncfromremotejob.h
  io/abstractconnectjob.h
  io/abstractmodelstreamencoder.h
  io/abstractexportjob.h
  io/abstractmodelexporter.h
  io/modelstreamencodethread.h
  io/abstractloadjob.h
  io/abstractmodelsynchronizerfactory.h
  system/jobmanager.h
  system/kdocumentmanager.h
  system/kdocumentcreatemanager.h
  system/kdocumentsyncmanager.h
  system/modelcodecmanager.h
)

kde4_add_library( kastencore SHARED ${kastencore_LIB_SRCS} )
target_link_libraries( kastencore
  ${KDE4_KIO_LIBS}
)
set_target_properties( kastencore  PROPERTIES VERSION ${GENERIC_LIB_VERSION} SOVERSION ${GENERIC_LIB_SOVERSION} )

install( TARGETS kastencore  ${INSTALL_TARGETS_DEFAULT_ARGS} )

install( FILES ${kastencore_LIB_HDRS}  DESTINATION  ${INCLUDE_INSTALL_DIR}/kasten  COMPONENT Devel )
