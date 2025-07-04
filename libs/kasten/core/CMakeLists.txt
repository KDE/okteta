set(KASTENCORE_VERSION ${KASTEN_LIB_VERSION})
set(KASTENCORE_SO_VERSION ${KASTEN_LIB_SOVERSION})

okteta_add_library(Core NAMESPACE Kasten
    PUBLIC
        KF5::CoreAddons
        Qt5::Core
    VERSION     ${KASTENCORE_VERSION}
    SOVERSION   ${KASTENCORE_SO_VERSION}
    ABIVERSION  ${KASTEN_ABI_VERSION}
    NO_TARGET_NAMESPACE
    NO_VERSIONED_EXPORTED_TARGET_NAME
    NO_VERSIONED_INCLUDEDIR
    NO_VERSIONED_PACKAGE_NAME
    KI18N_TRANSLATION_DOMAIN "libkasten"
)

ecm_qt_declare_logging_category(KastenCore
    HEADER "logging.hpp"
    IDENTIFIER "LOG_KASTEN_CORE"
    CATEGORY_NAME "kasten.core"
    DESCRIPTION "Kasten Core library"
    EXPORT OKTETA
)

macro(KASTENCORE_ADD_SUBLIBRARY _library_name)
    okteta_add_sublibrary(Core NAMESPACE Kasten
        SUBLIBRARY ${_library_name}
        ${ARGN}
    )
endmacro()

macro(KASTENCORE_SUBLIBRARY_SOURCES _library_name)
    okteta_sublibrary_sources(Core NAMESPACE Kasten
        SUBLIBRARY ${_library_name}
        ${ARGN}
    )
endmacro()

# kastencore_library_sources(entity
#     BUILD_INCLUDEDIR
#     PUBLIC
#         AbstractEntity
#         Group
#         Person
# )

kastencore_add_sublibrary(KastenCoreUserMessages
    UNTESTED
    PUBLIC
        Qt5::Core
    PRIVATE
        KF5::WidgetsAddons
)

kastencore_sublibrary_sources(KastenCoreUserMessages
    SUBDIR usermessages
    BUILD_INCLUDEDIR
    PUBLIC
        UserErrorReport
        UserNotification
        UserResponseOption
        UserQuery
)

kastencore_add_sublibrary(KastenCoreDocument
    UNTESTED
    PUBLIC
        KF5::CoreAddons
        Qt5::Core
)

kastencore_sublibrary_sources(KastenCoreDocument
    PUBLIC
        KastenCore
)

kastencore_sublibrary_sources(KastenCoreDocument
    SUBDIR document
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

kastencore_sublibrary_sources(KastenCoreDocument
    SUBDIR io
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

kastencore_add_sublibrary(KastenCoreFileSystemIo
    UNTESTED
    PUBLIC
        KastenCoreDocument
    PRIVATE
        KF5::JobWidgets
        KF5::KIOCore
        Qt5::Network
)

kastencore_sublibrary_sources(KastenCoreFileSystemIo
    SUBDIR io/filesystem
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

kastencore_add_sublibrary(KastenCoreSystem
    UNTESTED
    PUBLIC
        KastenCoreDocument
    PRIVATE
        KastenCoreFileSystemIo
        KF5::I18n
        KF5::JobWidgets
        KF5::KIOCore
        KF5::WidgetsAddons
    KI18N_TRANSLATION_DOMAIN "libkasten"
)

kastencore_sublibrary_sources(KastenCoreSystem
    SUBDIR system
    BUILD_INCLUDEDIR
    PUBLIC
        AbstractUserMessagesHandler

        AbstractOverwriteDialog
        AbstractSaveDiscardDialog

        JobManager
        DocumentManager
        DocumentCreateManager
        DocumentSyncManager
        ModelCodecManager
)

okteta_add_cmakeconfig(Core NAMESPACE Kasten
    DEPS
        KF5CoreAddons ${KF5_VERSION}
        Qt5Core ${Qt5_VERSION}
)

if(BUILD_TESTING)
    add_subdirectory(tests)
endif()
