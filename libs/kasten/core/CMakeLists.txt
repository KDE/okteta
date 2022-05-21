set(KASTENCORE_VERSION ${KASTEN_LIB_VERSION})
set(KASTENCORE_SO_VERSION ${KASTEN_LIB_SOVERSION})

okteta_add_library(Core NAMESPACE Kasten
    PUBLIC
        KF6::CoreAddons
        Qt6::Core
    VERSION     ${KASTENCORE_VERSION}
    SOVERSION   ${KASTENCORE_SO_VERSION}
    ABIVERSION  ${KASTEN_ABI_VERSION}
    NO_TARGET_NAMESPACE
    NO_VERSIONED_INCLUDEDIR
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

macro(KASTENCORE_SUBLIBRARY_SOURCES _subdir)
    okteta_sublibrary_sources(Core NAMESPACE Kasten
        SUBDIR ${_subdir}
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

kastencore_add_sublibrary(kastenusermessages
    UNTESTED
    PUBLIC
        Qt6::Core
    PRIVATE
        KF6::WidgetsAddons
)

kastencore_sublibrary_sources(usermessages
    SUBLIBRARY kastenusermessages
    BUILD_INCLUDEDIR
    PUBLIC
        UserErrorReport
        UserNotification
        UserResponseOption
        UserQuery
)

kastencore_add_sublibrary(kastendocument
    UNTESTED
    PUBLIC
        KF6::CoreAddons
        Qt6::Core
)

okteta_sublibrary_sources(Core NAMESPACE Kasten
    SUBLIBRARY kastendocument
    PUBLIC
        KastenCore
)

kastencore_sublibrary_sources(document
    SUBLIBRARY kastendocument
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

kastencore_sublibrary_sources(io
    SUBLIBRARY kastendocument
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

kastencore_add_sublibrary(kastenfilesystemio
    UNTESTED
    PUBLIC
        kastendocument
    PRIVATE
        KF6::WidgetsAddons
        KF6::KIOCore
        Qt6::Network
)

kastencore_sublibrary_sources(io/filesystem
    SUBLIBRARY kastenfilesystemio
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

kastencore_add_sublibrary(kastencoresystem
    UNTESTED
    PUBLIC
        kastendocument
    PRIVATE
        kastenfilesystemio
        KF6::I18n
        KF6::KIOCore
        KF6::WidgetsAddons
    KI18N_TRANSLATION_DOMAIN "libkasten"
)

kastencore_sublibrary_sources(system
    SUBLIBRARY kastencoresystem
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
        KF6CoreAddons ${KF6_VERSION}
        Qt6Core ${Qt6_VERSION}
)

if(BUILD_TESTING)
    add_subdirectory(tests)
endif()
