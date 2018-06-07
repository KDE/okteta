add_definitions(-DTRANSLATION_DOMAIN=\"liboktetakasten\")

ecm_setup_version( ${OKTETAKASTEN_LIB_VERSION}
    VARIABLE_PREFIX OKTETAKASTENCORE
    SOVERSION ${OKTETAKASTEN_LIB_SOVERSION}
    PACKAGE_VERSION_FILE "${CMAKE_CURRENT_BINARY_DIR}/OktetaKastenCoreConfigVersion.cmake"
)

include_directories(
  document
)

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
    SOVERSION   ${OKTETAKASTENCORE_SOVERSION}
    ABIVERSION  ${OKTETAKASTEN_ABI_VERSION} ${KASTEN_ABI_VERSION}
    SOURCES   ${oktetakastencore_LIB_SRCS}
    HEADERS   ${oktetakastencore_LIB_HDRS}
    CCHEADERS ${oktetakastencore_LIB_CCHDRS}
    INCLUDE_INSTALL_DIR ${KDE_INSTALL_INCLUDEDIR}
    NO_TARGET_NAMESPACE
    NO_VERSIONED_INCLUDEDIR
    REVERSE_NAMESPACE_INCLUDEDIR
    LOWERCASE_LIB
    REVERSE_NAMESPACE_LIB
    EXPORT OktetaKastenCoreTargets
)

okteta_add_cmakeconfig(Core NAMESPACE Okteta Kasten
    NO_TARGET_NAMESPACE
    CONFIG_TEMPLATE "${CMAKE_CURRENT_SOURCE_DIR}/OktetaKastenCoreConfig.cmake.in"
    EXPORT OktetaKastenCoreTargets
#     ABIVERSION ${KASTEN_ABI_VERSION}
    FILES
        "${CMAKE_CURRENT_BINARY_DIR}/OktetaKastenCoreConfigVersion.cmake"
)

if(BUILD_TESTING)
  add_subdirectory(document/tests)
  add_subdirectory(io/tests)
endif()
