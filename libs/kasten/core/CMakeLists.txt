ecm_setup_version( ${KASTEN_LIB_VERSION}
    VARIABLE_PREFIX KASTENCORE
    SOVERSION ${KASTEN_LIB_SOVERSION}
    PACKAGE_VERSION_FILE "${CMAKE_CURRENT_BINARY_DIR}/KastenCoreConfigVersion.cmake"
)

add_definitions(-DTRANSLATION_DOMAIN=\"libkasten\")

include_directories(
    entity
    document
    io
    io/filesystem
    system
)

macro(KASTENCORE_ADD_SUBLIBRARY _subdir)
    okteta_add_sublibrary(Core  NAMESPACE Kasten
        SUBDIR ${_subdir}
        ${ARGN}
    )
endmacro()

kastencore_add_sublibrary(entity
    PUBLIC
#         AbstractEntity
#         Group
        Person
)

kastencore_add_sublibrary(document
    PUBLIC
        Versionable
        UserListable

        AbstractModel
        AbstractModelSelection
        AbstractDocument
        AbstractDocumentFactory
        AbstractTool
        AbstractToolFactory

        DocumentVersionId
        DocumentVersionData
)

kastencore_add_sublibrary(io
    PUBLIC
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
        AbstractLoadJob
        AbstractModelSynchronizerFactory
)

kastencore_add_sublibrary(io/filesystem
    PUBLIC
        AbstractFileSystemSyncWithRemoteJob
        AbstractFileSystemSyncFromRemoteJob
        AbstractFileSystemSyncToRemoteJob
        AbstractModelFileSystemSynchronizer
        AbstractFileSystemConnectJob
        AbstractFileSystemLoadJob
        AbstractFileSystemExportJob
        ModelEncoderFileSystemExporter
        ModelEncoderFileSystemExportJob
)

kastencore_add_sublibrary(system
    PUBLIC
        AbstractOverwriteDialog
        AbstractSaveDiscardDialog

        JobManager
        DocumentManager
        DocumentCreateManager
        DocumentSyncManager
        ModelCodecManager
)

okteta_add_sublibrary(Core  NAMESPACE Kasten
    PUBLIC
        KastenCore
)


okteta_add_library(Core NAMESPACE Kasten
    PUBLIC
        KF5::KIOCore
        Qt5::Core
        Qt5::Gui
    PRIVATE
        KF5::I18n
        KF5::JobWidgets
        KF5::WidgetsAddons
        Qt5::Network
    VERSION     ${KASTENCORE_VERSION}
    SOVERSION   ${KASTENCORE_SOVERSION}
    ABIVERSION  ${KASTEN_ABI_VERSION}
    SOURCES   ${kastencore_LIB_SRCS}
    HEADERS   ${kastencore_LIB_HDRS}
    CCHEADERS ${kastencore_LIB_CCHDRS}
    INCLUDE_INSTALL_DIR ${KDE_INSTALL_INCLUDEDIR}
    NO_TARGET_NAMESPACE
    NO_VERSIONED_INCLUDEDIR
    LOWERCASE_LIB
    EXPORT KastenCoreTargets
)

okteta_add_cmakeconfig(Core NAMESPACE Kasten
    NO_TARGET_NAMESPACE
    CONFIG_TEMPLATE "${CMAKE_CURRENT_SOURCE_DIR}/KastenCoreConfig.cmake.in"
    EXPORT KastenCoreTargets
#     ABIVERSION ${KASTEN_ABI_VERSION}
    FILES
        "${CMAKE_CURRENT_BINARY_DIR}/KastenCoreConfigVersion.cmake"
)

if(BUILD_TESTING)
  add_subdirectory( tests )
endif()
