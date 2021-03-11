set(OKTETAKASTENCORE_VERSION ${OKTETAKASTEN_LIB_VERSION})
set(OKTETAKASTENCORE_SO_VERSION ${OKTETAKASTEN_LIB_SOVERSION})

add_definitions(-DTRANSLATION_DOMAIN=\"liboktetakasten\")

macro(OKTETAKASTENCORE_ADD_SUBLIBRARY _subdir)
    okteta_add_sublibrary(Core NAMESPACE Okteta Kasten
        REVERSE_NAMESPACE_INCLUDEDIR
        SUBDIR ${_subdir}
        ${ARGN}
    )
endmacro()

oktetakastencore_add_sublibrary(document
    PUBLIC
        ByteArrayDocument
        ByteArrayDocumentFactory
        ByteArraySelection
)

oktetakastencore_add_sublibrary(io/filesystem
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
    SOURCES   ${oktetakastencore_LIB_SRCS}
    HEADERS   ${oktetakastencore_LIB_HDRS}
    CCHEADERS ${oktetakastencore_LIB_CCHDRS}
    NO_TARGET_NAMESPACE
    NO_VERSIONED_INCLUDEDIR
    NO_VERSIONED_PACKAGE_NAME
    REVERSE_NAMESPACE_INCLUDEDIR
    REVERSE_NAMESPACE_LIB
)
target_include_directories(OktetaKastenCore
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/document>
)


okteta_add_cmakeconfig(Core NAMESPACE Okteta Kasten
)

if(BUILD_TESTING)
  add_subdirectory(document/tests)
  add_subdirectory(io/tests)
endif()
