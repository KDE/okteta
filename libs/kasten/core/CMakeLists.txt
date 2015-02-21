project( kastencore )

add_definitions(-DTRANSLATION_DOMAIN=\"libkasten\")

include_directories(
  entity
  document
  io
  io/filesystem
  system
)

set( KASTEN_CORE_ENTITY_SRCS
#   entity/group.cpp
  entity/person.cpp
#   entity/abstractentity.cpp
)
ecm_generate_headers(KASTEN_CORE_ENTITY_CCHDRS
  HEADER_NAMES
    Person
  RELATIVE entity
  PREFIX Kasten
  REQUIRED_HEADERS KASTEN_CORE_ENTITY_HDRS
)

set( KASTEN_CORE_DOCUMENT_SRCS
  document/abstractmodel.cpp
  document/abstractmodelselection.cpp
  document/abstracttool.cpp
  document/abstractdocument.cpp
  document/abstractdocumentfactory.cpp
  document/documentversiondata.cpp
)
set( KASTEN_CORE_DOCUMENT_HDRS
  document/documentversionid.h
)
ecm_generate_headers(KASTEN_CORE_DOCUMENT_CCHDRS
  HEADER_NAMES
    AbstractModel
    AbstractModelSelection
    AbstractDocument
    AbstractDocumentFactory
    AbstractTool
    AbstractToolFactory
    DocumentVersionData
    Versionable
    UserListable
  RELATIVE document
  PREFIX Kasten
  REQUIRED_HEADERS KASTEN_CORE_DOCUMENT_HDRS
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
  io/modeldatageneratethread.cpp
  io/abstractmodeldatagenerator.cpp
  io/modelstreamencodethread.cpp
  io/abstractmodelsynchronizer.cpp
  io/abstractmodelsynchronizerfactory.cpp
)
ecm_generate_headers(KASTEN_CORE_IO_CCHDRS
  HEADER_NAMES
    AbstractSyncToRemoteJob
    AbstractModelSynchronizer
    AbstractSyncWithRemoteJob
    AbstractSyncFromRemoteJob
    AbstractConnectJob
    AbstractModelStreamEncoder
    AbstractModelDataGenerator
    AbstractExportJob
    AbstractModelExporter
    ModelStreamEncodeThread
    ModelDataGenerateThread
    AbstractloadJob
    AbstractModelSynchronizerfactory
  RELATIVE io
  PREFIX Kasten
  REQUIRED_HEADERS KASTEN_CORE_IO_HDRS
)

set( KASTEN_CORE_IO_FILESYSTEM_SRCS
  io/filesystem/abstractfilesystemloadjob_p.cpp
  io/filesystem/abstractfilesystemloadjob.cpp
  io/filesystem/abstractfilesystemsynctoremotejob_p.cpp
  io/filesystem/abstractfilesystemsynctoremotejob.cpp
  io/filesystem/abstractfilesystemsyncfromremotejob_p.cpp
  io/filesystem/abstractfilesystemsyncfromremotejob.cpp
  io/filesystem/abstractfilesystemsyncwithremotejob_p.cpp
  io/filesystem/abstractfilesystemsyncwithremotejob.cpp
  io/filesystem/abstractfilesystemconnectjob_p.cpp
  io/filesystem/abstractfilesystemconnectjob.cpp
  io/filesystem/abstractfilesystemexportjob_p.cpp
  io/filesystem/abstractfilesystemexportjob.cpp
  io/filesystem/modelencoderfilesystemexportjob_p.cpp
  io/filesystem/modelencoderfilesystemexportjob.cpp
  io/filesystem/modelencoderfilesystemexporter.cpp
  io/filesystem/abstractmodelfilesystemsynchronizer_p.cpp
  io/filesystem/abstractmodelfilesystemsynchronizer.cpp
)
ecm_generate_headers(KASTEN_CORE_IO_FILESYSTEM_CCHDRS
  HEADER_NAMES
    AbstractFileSystemSyncWithRemoteJob
    AbstractFileSystemSyncFromRemoteJob
    AbstractFileSystemSyncToRemoteJob
    AbstractModelFileSystemSynchronizer
    AbstractFileSystemConnectJob
    AbstractFileSystemLoadJob
    AbstractFileSystemExportjob
    ModelEncoderFileSystemExporter
    ModelEncoderFileSystemExportJob
  RELATIVE io/filesystem
  PREFIX Kasten
  REQUIRED_HEADERS KASTEN_CORE_IO_FILESYSTEM_HDRS
)

set( KASTEN_CORE_SYSTEM_SRCS
  system/jobmanager.cpp
  system/documentmanager.cpp
  system/documentcreatemanager.cpp
  system/documentsyncmanager.cpp
  system/modelcodecmanager.cpp
)
ecm_generate_headers(KASTEN_CORE_SYSTEM_CCHDRS
  HEADER_NAMES
    AbstractOverwriteDialog
    AbstractSaveDiscardDialog
    JobManager
    DocumentManager
    DocumentCreateManager
    DocumentSyncManager
    ModelCodecManager
  RELATIVE system
  PREFIX Kasten
  REQUIRED_HEADERS KASTEN_CORE_SYSTEM_HDRS
)

set( kastencore_LIB_SRCS
  kastencore.cpp
  ${KASTEN_CORE_ENTITY_SRCS}
  ${KASTEN_CORE_DOCUMENT_SRCS}
  ${KASTEN_CORE_IO_SRCS}
  ${KASTEN_CORE_IO_FILESYSTEM_SRCS}
  ${KASTEN_CORE_SYSTEM_SRCS}
)

set( kastencore_LIB_HDRS
  ${CMAKE_CURRENT_BINARY_DIR}/kastencore_export.h
  kastencore.h
  ${KASTEN_CORE_ENTITY_HDRS}
  ${KASTEN_CORE_DOCUMENT_HDRS}
  ${KASTEN_CORE_IO_HDRS}
  ${KASTEN_CORE_IO_FILESYSTEM_HDRS}
  ${KASTEN_CORE_SYSTEM_HDRS}
)

set( kastencore_LIB_CCHDRS
  ${KASTEN_CORE_ENTITY_CCHDRS}
  ${KASTEN_CORE_DOCUMENT_CCHDRS}
  ${KASTEN_CORE_IO_CCHDRS}
  ${KASTEN_CORE_IO_FILESYSTEM_CCHDRS}
  ${KASTEN_CORE_SYSTEM_CCHDRS}
)


add_library( ${kastencore_LIB} SHARED ${kastencore_LIB_SRCS} )
generate_export_header( ${kastencore_LIB} )
target_link_libraries( ${kastencore_LIB}
PUBLIC
  KF5::KIOCore
  Qt5::Core
  Qt5::Gui
PRIVATE
  KF5::I18n
  KF5::JobWidgets
  KF5::WidgetsAddons
  Qt5::Network
)

target_include_directories(${kastencore_LIB} PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/kasten>")

set_target_properties( ${kastencore_LIB} PROPERTIES
  OUTPUT_NAME ${kastencore_LIB_NAME}
  VERSION     ${KASTEN_LIB_VERSION}
  SOVERSION   ${KASTEN_LIB_SOVERSION}
)

install( TARGETS ${kastencore_LIB}  ${INSTALL_TARGETS_DEFAULT_ARGS} )

install( FILES ${kastencore_LIB_HDRS}
  DESTINATION ${KASTEN_INCLUDE_INSTALL_DIR}
  COMPONENT   Devel
)
install( FILES ${kastencore_LIB_CCHDRS}
  DESTINATION ${KASTEN_CCINCLUDE_INSTALL_DIR}
  COMPONENT Devel
)

if(BUILD_TESTING)
  add_subdirectory( tests )
endif(BUILD_TESTING)
