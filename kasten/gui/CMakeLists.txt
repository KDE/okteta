add_definitions(-DTRANSLATION_DOMAIN=\"liboktetakasten\")

ecm_setup_version( ${OKTETAKASTEN_LIB_VERSION}
    VARIABLE_PREFIX OKTETAKASTENGUI
    SOVERSION ${OKTETAKASTEN_LIB_SOVERSION}
    PACKAGE_VERSION_FILE "${CMAKE_CURRENT_BINARY_DIR}/OktetaKastenGuiConfigVersion.cmake"
)

macro(OKTETAKASTENGUI_GENERATE_HEADERS _SUBLIB _RELATIVE)
    ecm_generate_headers("${_SUBLIB}_CCHDRS"
        HEADER_NAMES ${ARGN}
        RELATIVE ${_RELATIVE}
        PREFIX Kasten/Okteta
        REQUIRED_HEADERS "${_SUBLIB}_HDRS"
    )
endmacro()

include_directories(
  system
  io
  view
  io/streamencoder
  liboktetawidgets
)

# if(KDE4_BUILD_TESTS)
#   add_subdirectory(tests)
# endif(KDE4_BUILD_TESTS)

set( oktetawidgets_LIB_SRCS
  liboktetawidgets/bytearrayvalidator.cpp
  liboktetawidgets/bytearraycombobox_p.cpp
  liboktetawidgets/bytearraycombobox.cpp
  liboktetawidgets/addressvalidator.cpp
  liboktetawidgets/addresscombobox_p.cpp
  liboktetawidgets/addresscombobox.cpp
)
oktetakastengui_generate_headers(oktetawidgets liboktetawidgets
  AddressValidator
  AddressComboBox
  ByteArrayComboBox
)

set( okteta_values_streamencoder_SRCS
  io/streamencoder/values/bytearrayvaluesstreamencoderconfigeditor.cpp
  io/streamencoder/values/bytearrayvaluesstreamencoderconfigeditorfactory.cpp
  io/streamencoder/values/bytearrayvaluesstreamencoder.cpp
)

set( okteta_base32_streamencoder_SRCS
  io/streamencoder/base32/bytearraybase32streamencoderconfigeditor.cpp
  io/streamencoder/base32/bytearraybase32streamencoderconfigeditorfactory.cpp
  io/streamencoder/base32/bytearraybase32streamencoder.cpp
)

set( okteta_uuencoding_streamencoder_SRCS
  io/streamencoder/uuencoding/bytearrayuuencodingstreamencoderconfigeditor.cpp
  io/streamencoder/uuencoding/bytearrayuuencodingstreamencoderconfigeditorfactory.cpp
  io/streamencoder/uuencoding/bytearrayuuencodingstreamencoder.cpp
)

set( okteta_xxencoding_streamencoder_SRCS
  io/streamencoder/xxencoding/bytearrayxxencodingstreamencoderconfigeditor.cpp
  io/streamencoder/xxencoding/bytearrayxxencodingstreamencoderconfigeditorfactory.cpp
  io/streamencoder/xxencoding/bytearrayxxencodingstreamencoder.cpp
)

set( okteta_sourcecode_streamencoder_SRCS
  io/streamencoder/sourcecode/bytearraysourcecodestreamencoderconfigeditor.cpp
  io/streamencoder/sourcecode/bytearraysourcecodestreamencoderconfigeditorfactory.cpp
  io/streamencoder/sourcecode/bytearraysourcecodestreamencoder.cpp
)

set( okteta_srec_streamencoder_SRCS
  io/streamencoder/srec/bytearraysrecstreamencoderconfigeditor.cpp
  io/streamencoder/srec/bytearraysrecstreamencoderconfigeditorfactory.cpp
  io/streamencoder/srec/bytearraysrecstreamencoder.cpp
)

set( okteta_ihex_streamencoder_SRCS
  io/streamencoder/ihex/bytearrayihexstreamencoderconfigeditor.cpp
  io/streamencoder/ihex/bytearrayihexstreamencoderconfigeditorfactory.cpp
  io/streamencoder/ihex/bytearrayihexstreamencoder.cpp
)

set( okteta_viewtext_streamencoder_SRCS
  io/streamencoder/viewtext/abstractcolumntextrenderer.cpp
  io/streamencoder/viewtext/offsetcolumntextrenderer.cpp
  io/streamencoder/viewtext/bordercolumntextrenderer.cpp
  io/streamencoder/viewtext/abstractbytearraycolumntextrenderer.cpp
  io/streamencoder/viewtext/valuebytearraycolumntextrenderer.cpp
  io/streamencoder/viewtext/charbytearraycolumntextrenderer.cpp
  io/streamencoder/viewtext/bytearrayrowscolumntextrenderer.cpp
  io/streamencoder/viewtext/bytearrayviewtextstreamencoder.cpp
)

set( okteta_streamencoder_SRCS
  io/streamencoder/abstractbytearraystreamencoder.cpp
  io/streamencoder/bytearraytextstreamencoderpreview.cpp
  ${okteta_values_streamencoder_SRCS}
  io/streamencoder/chars/bytearraycharsstreamencoder.cpp
  io/streamencoder/base64/bytearraybase64streamencoder.cpp
  ${okteta_base32_streamencoder_SRCS}
  io/streamencoder/base85/bytearraybase85streamencoder.cpp
  ${okteta_uuencoding_streamencoder_SRCS}
  ${okteta_xxencoding_streamencoder_SRCS}
  ${okteta_srec_streamencoder_SRCS}
  ${okteta_ihex_streamencoder_SRCS}
  ${okteta_sourcecode_streamencoder_SRCS}
  ${okteta_viewtext_streamencoder_SRCS}
  io/bytearraystreamencoderfactory.cpp
  io/bytearraystreamencoderconfigeditorfactoryfactory.cpp
)
oktetakastengui_generate_headers(okteta_streamencoder io/streamencoder
  AbstractByteArrayStreamEncoder
)
oktetakastengui_generate_headers(okteta_streamencoder io
  ByteArrayStreamEncoderFactory
  ByteArrayStreamEncoderConfigEditorFactoryFactory
)

set( okteta_pattern_datagenerator_SRCS
  io/generator/pattern/bytearraypatterngeneratorconfigeditor.cpp
  io/generator/pattern/bytearraypatterngeneratorconfigeditorfactory.cpp
  io/generator/pattern/bytearraypatterngenerator.cpp
)

set( okteta_randomdata_datagenerator_SRCS
  io/generator/randomdata/bytearrayrandomdatageneratorconfigeditor.cpp
  io/generator/randomdata/bytearrayrandomdatageneratorconfigeditorfactory.cpp
  io/generator/randomdata/bytearrayrandomdatagenerator.cpp
)

set( okteta_sequence_datagenerator_SRCS
  io/generator/sequence/bytearraysequencegenerator.cpp
)

set( okteta_datagenerator_SRCS
  ${okteta_randomdata_datagenerator_SRCS}
  ${okteta_pattern_datagenerator_SRCS}
  ${okteta_sequence_datagenerator_SRCS}
  io/bytearraydatageneratorfactory.cpp
  io/bytearraydatageneratorconfigeditorfactoryfactory.cpp
)
oktetakastengui_generate_headers(okteta_datagenerator io
  ByteArrayDataGeneratorFactory
  ByteArrayDataGeneratorConfigEditorFactoryFactory
)

set( okteta_viewprofile_io_SRCS
  io/bytearrayviewprofilesynchronizer.cpp
)
oktetakastengui_generate_headers(okteta_viewprofile_io io
  ByteArrayViewProfileSynchronizer
)

set( okteta_kastengui_io_SRCS
  ${okteta_streamencoder_SRCS}
  ${okteta_datagenerator_SRCS}
  ${okteta_viewprofile_io_SRCS}
)
set( okteta_kastengui_io_HDRS
  ${okteta_streamencoder_HDRS}
  ${okteta_datagenerator_HDRS}
  ${okteta_viewprofile_io_HDRS}
)
set( okteta_kastengui_io_CCHDRS
  ${okteta_streamencoder_CCHDRS}
  ${okteta_datagenerator_CCHDRS}
  ${okteta_viewprofile_io_CCHDRS}
)

set( okteta_kastengui_view_SRCS
  view/bytearrayjanusview.cpp
  view/bytearrayviewfactory.cpp
  view/bytearrayview.cpp
  view/bytearrayviewprofile.cpp
)
oktetakastengui_generate_headers(okteta_kastengui_view view
  ByteArrayViewFactory
  ByteArrayView
  ByteArrayViewProfile
)

set( okteta_kastengui_system_SRCS
  system/bytearrayviewprofilelock.cpp
  system/bytearrayviewprofilemanager.cpp
)
oktetakastengui_generate_headers(okteta_kastengui_system system
  ByteArrayViewProfileLock
  ByteArrayViewProfileManager
)

set( okteta_kastengui_LIB_SRCS
  oktetakastengui.cpp
  ${oktetawidgets_LIB_SRCS}
  ${okteta_kastengui_view_SRCS}
  ${okteta_kastengui_io_SRCS}
  ${okteta_kastengui_system_SRCS}
)

set( okteta_kastengui_LIB_HDRS
  ${CMAKE_CURRENT_BINARY_DIR}/oktetakastengui_export.h
  ${oktetawidgets_LIB_HDRS}
  ${okteta_kastengui_view_HDRS}
  ${okteta_kastengui_io_HDRS}
  ${okteta_kastengui_system_HDRS}
)
set( okteta_kastengui_LIB_CCHDRS
  ${oktetawidgets_LIB_CCHDRS}
  ${okteta_kastengui_view_CCHDRS}
  ${okteta_kastengui_io_CCHDRS}
  ${okteta_kastengui_system_CCHDRS}
)

add_library( OktetaKastenGui  SHARED ${okteta_kastengui_LIB_SRCS} )
generate_export_header( OktetaKastenGui )

target_link_libraries( OktetaKastenGui
PUBLIC
  OktetaKastenCore
  KastenGui
  OktetaGui
PRIVATE
  Qt5::Script
  KF5::I18n
  KF5::Completion
)

target_include_directories(OktetaKastenGui PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/kasten/okteta>")

set_target_properties( OktetaKastenGui  PROPERTIES
  EXPORT_NAME "OktetaKastenGui"
  OUTPUT_NAME kasten${KASTEN_ABI_VERSION}okteta${OKTETAKASTEN_ABI_VERSION}gui
  VERSION     ${OKTETAKASTENGUI_VERSION}
  SOVERSION   ${OKTETAKASTENGUI_SOVERSION}
)

install( TARGETS OktetaKastenGui  EXPORT OktetaKastenGuiTargets ${INSTALL_TARGETS_DEFAULT_ARGS} )

install( FILES ${okteta_kastengui_LIB_HDRS}
  DESTINATION ${OKTETAKASTEN_INCLUDE_INSTALL_DIR}
  COMPONENT Devel
)
install( FILES ${okteta_kastengui_LIB_CCHDRS}
  DESTINATION ${OKTETAKASTEN_CCINCLUDE_INSTALL_DIR}
  COMPONENT Devel
)

# create a Config.cmake and a ConfigVersion.cmake file and install them
set(CMAKECONFIG_INSTALL_DIR "${CMAKECONFIG_INSTALL_PREFIX}/OktetaKasten")

ecm_configure_package_config_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/OktetaKastenGuiConfig.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/OktetaKastenGuiConfig.cmake"
  INSTALL_DESTINATION ${CMAKECONFIG_INSTALL_DIR}
)

install( FILES
    "${CMAKE_CURRENT_BINARY_DIR}/OktetaKastenGuiConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/OktetaKastenGuiConfigVersion.cmake"
  DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
  COMPONENT Devel
)

install(EXPORT OktetaKastenGuiTargets
  DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
  FILE OktetaKastenGuiTargets.cmake
)
