set(OKTETAKASTENCORE_VERSION ${OKTETAKASTEN_LIB_VERSION})
set(OKTETAKASTENCORE_SO_VERSION ${OKTETAKASTEN_LIB_SOVERSION})

set(LIBOKTETAWIDGETS_REL_DIR ../gui/liboktetawidgets)

okteta_add_library(Core NAMESPACE Okteta Kasten
    PUBLIC
        KastenCore
        OktetaCore
    PRIVATE
        KF5::I18n
        KF5::Bookmarks
    VERSION     ${OKTETAKASTENCORE_VERSION}
    SOVERSION   ${OKTETAKASTENCORE_SO_VERSION}
    ABIVERSION  ${OKTETAKASTEN_ABI_VERSION} ${KASTEN_ABI_VERSION}
    NO_TARGET_NAMESPACE
    NO_VERSIONED_EXPORTED_TARGET_NAME
    NO_VERSIONED_INCLUDEDIR
    NO_VERSIONED_PACKAGE_NAME
    REVERSE_NAMESPACE_INCLUDEDIR
    REVERSE_NAMESPACE_LIB
    KI18N_TRANSLATION_DOMAIN "liboktetakasten"
)

macro(OKTETAKASTENCORE_SOURCES)
    okteta_library_sources(Core NAMESPACE Okteta Kasten
        REVERSE_NAMESPACE_INCLUDEDIR
        ${ARGN}
    )
endmacro()

# TODO: move to oktetacore, no longer duplicate
oktetakastencore_sources(
    SUBDIR ${LIBOKTETAWIDGETS_REL_DIR}
    BUILD_INCLUDEDIR
    PRIVATE
        ByteArrayChar8StringDecoder
        ByteArrayValueStringDecoder
)

oktetakastencore_sources(
    SUBDIR document
    BUILD_INCLUDEDIR
    PUBLIC
        ByteArrayDocument
        ByteArrayDocumentFactory
        ByteArraySelection
)

oktetakastencore_sources(
    SUBDIR io/filesystem
    PUBLIC
        ByteArrayRawFileSynchronizer
        ByteArrayRawFileSynchronizerFactory
    PRIVATE
        #   io/filesystem/externalbookmarkstorage
        ByteArrayRawFileLoadThread
        ByteArrayRawFilereloadThread
        ByteArrayRawFilewriteThread
        ByteArrayRawFileLoadJob
        ByteArrayRawFileConnectjob
        ByteArrayRawFileWriteJob
        ByteArrayRawFileReloadJob
        ByteArrayRawFileWriteToJob
)

okteta_add_cmakeconfig(Core NAMESPACE Okteta Kasten
    DEPS
        KastenCore
        OktetaCore
)

if(BUILD_TESTING)
    add_subdirectory(document/tests)
    add_subdirectory(io/tests)
endif()
