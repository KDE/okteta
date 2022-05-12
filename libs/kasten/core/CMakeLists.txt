set(KASTENCORE_VERSION ${KASTEN_LIB_VERSION})
set(KASTENCORE_SO_VERSION ${KASTEN_LIB_SOVERSION})

okteta_add_library(Core NAMESPACE Kasten
    PUBLIC
        KF5::KIOCore
        Qt::Core
        Qt::Gui
    PRIVATE
        KF5::I18n
        KF5::JobWidgets
        KF5::WidgetsAddons
        Qt::Network
    VERSION     ${KASTENCORE_VERSION}
    SOVERSION   ${KASTENCORE_SO_VERSION}
    ABIVERSION  ${KASTEN_ABI_VERSION}
    NO_TARGET_NAMESPACE
    NO_VERSIONED_INCLUDEDIR
    NO_VERSIONED_PACKAGE_NAME
)

target_compile_definitions(KastenCore PRIVATE -DTRANSLATION_DOMAIN=\"libkasten\")

macro(KASTENCORE_ADD_SUBLIBRARY _subdir)
    okteta_add_sublibrary(Core  NAMESPACE Kasten
        SUBDIR ${_subdir}
        ${ARGN}
    )
endmacro()

# kastencore_add_sublibrary(entity
#     BUILD_INCLUDEDIR
#     PUBLIC
#         AbstractEntity
#         Group
#         Person
# )

kastencore_add_sublibrary(document
    BUILD_INCLUDEDIR
    PUBLIC
        Versionable
#         UserListable

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
    BUILD_INCLUDEDIR
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
    BUILD_INCLUDEDIR
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
    BUILD_INCLUDEDIR
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

okteta_add_cmakeconfig(Core NAMESPACE Kasten
)

if(BUILD_TESTING)
    add_subdirectory(tests)
endif()
