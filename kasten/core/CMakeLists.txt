set(OKTETAKASTENCORE_VERSION ${OKTETAKASTEN_LIB_VERSION})
set(OKTETAKASTENCORE_SO_VERSION ${OKTETAKASTEN_LIB_SOVERSION})

okteta_add_library(Core NAMESPACE Okteta Kasten
    PUBLIC
        KastenCore
        OktetaCore
    PRIVATE
        KF6::I18n
        KF6::Bookmarks
    VERSION     ${OKTETAKASTENCORE_VERSION}
    SOVERSION   ${OKTETAKASTENCORE_SO_VERSION}
    ABIVERSION  ${OKTETAKASTEN_ABI_VERSION} ${KASTEN_ABI_VERSION}
    NO_TARGET_NAMESPACE
    NO_VERSIONED_INCLUDEDIR
    REVERSE_NAMESPACE_INCLUDEDIR
    REVERSE_NAMESPACE_LIB
)

target_compile_definitions(OktetaKastenCore PRIVATE -DTRANSLATION_DOMAIN=\"liboktetakasten\")

macro(OKTETAKASTENCORE_LIBRARY_SOURCES _subdir)
    okteta_library_sources(Core NAMESPACE Okteta Kasten
        REVERSE_NAMESPACE_INCLUDEDIR
        SUBDIR ${_subdir}
        ${ARGN}
    )
endmacro()

oktetakastencore_library_sources(document
    BUILD_INCLUDEDIR
    PUBLIC
        ByteArrayDocument
        ByteArrayDocumentFactory
        ByteArraySelection
)

oktetakastencore_library_sources(io/filesystem
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
