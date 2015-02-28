add_definitions(-DTRANSLATION_DOMAIN=\"liboktetakasten\")

ecm_setup_version( ${OKTETAKASTEN_LIB_VERSION}
    VARIABLE_PREFIX OKTETAKASTENCORE
    SOVERSION ${OKTETAKASTEN_LIB_SOVERSION}
    PACKAGE_VERSION_FILE "${CMAKE_CURRENT_BINARY_DIR}/OktetaKastenCoreConfigVersion.cmake"
)

include_directories(
  document
)

set( okteta_kastencore_document_SRCS
  document/bytearraydocumentfactory.cpp
  document/bytearraydocument.cpp
  document/bytearrayselection.cpp
)

set( okteta_kastencore_io_filesystem_SRCS
#   io/filesystem/externalbookmarkstorage.cpp
  io/filesystem/bytearrayrawfileloadthread.cpp
  io/filesystem/bytearrayrawfilereloadthread.cpp
  io/filesystem/bytearrayrawfilewritethread.cpp
  io/filesystem/bytearrayrawfileloadjob.cpp
  io/filesystem/bytearrayrawfileconnectjob.cpp
  io/filesystem/bytearrayrawfilewritejob.cpp
  io/filesystem/bytearrayrawfilereloadjob.cpp
  io/filesystem/bytearrayrawfilewritetojob.cpp
  io/filesystem/bytearrayrawfilesynchronizer.cpp
  io/filesystem/bytearrayrawfilesynchronizerfactory.cpp
)


set( okteta_kastencore_io_SRCS
  ${okteta_kastencore_io_filesystem_SRCS}
)

set( okteta_kastencore_LIB_SRCS
  ${okteta_kastencore_document_SRCS}
  ${okteta_kastencore_io_SRCS}
)

set( okteta_kastencore_LIB_HDRS
  ${CMAKE_CURRENT_BINARY_DIR}/oktetakastencore_export.h
  document/bytearrayselection.h
  document/bytearraydocument.h
  document/bytearraydocumentfactory.h
  io/filesystem/bytearrayrawfilesynchronizer.h
  io/filesystem/bytearrayrawfilesynchronizerfactory.h
)

add_library( OktetaKastenCore  SHARED ${okteta_kastencore_LIB_SRCS} )
generate_export_header( OktetaKastenCore )

target_link_libraries( OktetaKastenCore
PUBLIC
  KastenCore
  OktetaCore
PRIVATE
  KF5::I18n
  KF5::Bookmarks
)

set_target_properties( OktetaKastenCore PROPERTIES
  EXPORT_NAME "OktetaKastenCore"
  OUTPUT_NAME kasten${KASTEN_ABI_VERSION}okteta${OKTETAKASTEN_ABI_VERSION}core
  VERSION     ${OKTETAKASTENCORE_VERSION}
  SOVERSION   ${OKTETAKASTENCORE_SOVERSION}
)

install( TARGETS OktetaKastenCore  EXPORT OktetaKastenCoreTargets ${INSTALL_TARGETS_DEFAULT_ARGS} )

install( FILES ${okteta_kastencore_LIB_HDRS}
  DESTINATION ${OKTETAKASTEN_INCLUDE_INSTALL_DIR}
  COMPONENT Devel
)

# create a Config.cmake and a ConfigVersion.cmake file and install them
set(CMAKECONFIG_INSTALL_DIR "${CMAKECONFIG_INSTALL_PREFIX}/OktetaKasten")

ecm_configure_package_config_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/OktetaKastenCoreConfig.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/OktetaKastenCoreConfig.cmake"
  INSTALL_DESTINATION ${CMAKECONFIG_INSTALL_DIR}
)

install( FILES
    "${CMAKE_CURRENT_BINARY_DIR}/OktetaKastenCoreConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/OktetaKastenCoreConfigVersion.cmake"
  DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
  COMPONENT Devel
)

install(EXPORT OktetaKastenCoreTargets
  DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
  FILE OktetaKastenCoreTargets.cmake
)


if(BUILD_TESTING)
  add_subdirectory(document/tests)
  add_subdirectory(io/tests)
endif(BUILD_TESTING)
