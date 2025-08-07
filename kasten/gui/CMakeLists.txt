set(OKTETAKASTENGUI_VERSION ${OKTETAKASTEN_LIB_VERSION})
set(OKTETAKASTENGUI_SO_VERSION ${OKTETAKASTEN_LIB_SOVERSION})

okteta_add_library(Gui NAMESPACE Okteta Kasten
    PUBLIC
        OktetaKastenCore
        KastenGui
        OktetaGui
    PRIVATE
        KF5::Completion
        KF5::I18n
    VERSION     ${OKTETAKASTENGUI_VERSION}
    SOVERSION   ${OKTETAKASTENGUI_SO_VERSION}
    ABIVERSION  ${OKTETAKASTEN_ABI_VERSION} ${KASTEN_ABI_VERSION}
    NO_VERSIONED_INCLUDEDIR
    NO_VERSIONED_PACKAGE_NAME
    REVERSE_NAMESPACE_INCLUDEDIR
    REVERSE_NAMESPACE_LIB
    NO_TARGET_NAMESPACE
    NO_VERSIONED_EXPORTED_TARGET_NAME
    KI18N_TRANSLATION_DOMAIN "liboktetakasten"
)

ecm_qt_declare_logging_category(OktetaKastenGui
    HEADER "logging.hpp"
    IDENTIFIER "LOG_KASTEN_OKTETA_GUI"
    CATEGORY_NAME "kasten.okteta.gui"
    DESCRIPTION "Kasten Okteta Gui module"
    EXPORT OKTETA
)

macro(OKTETAKASTENGUI_SOURCES)
    okteta_library_sources(Gui NAMESPACE Okteta Kasten
        REVERSE_NAMESPACE_INCLUDEDIR
        ${ARGN}
    )
endmacro()

macro(OKTETAKASTENGUI_ADD_SUBLIBRARY _library_name)
    okteta_add_sublibrary(Gui NAMESPACE Okteta Kasten
        SUBLIBRARY ${_library_name}
        ${ARGN}
    )
endmacro()

macro(OKTETAKASTENGUI_SUBLIBRARY_SOURCES _library_name)
    okteta_sublibrary_sources(Gui NAMESPACE Okteta Kasten
        REVERSE_NAMESPACE_INCLUDEDIR
        SUBLIBRARY ${_library_name}
        ${ARGN}
    )
endmacro()

oktetakastengui_add_sublibrary(OktetaKastenGuiWidgets
    UNTESTED
    PUBLIC
        OktetaCore
        Qt5::Widgets
    PRIVATE
        KF5::Completion
        KF5::I18n
        Qt5::Qml
    KI18N_TRANSLATION_DOMAIN "liboktetakasten"
)

oktetakastengui_sublibrary_sources(OktetaKastenGuiWidgets
    SUBDIR liboktetawidgets
    BUILD_INCLUDEDIR
    PUBLIC
        AddressComboBox
        ByteArrayComboBox
    PRIVATE
        AddressValidator
        AbstractByteArrayStringValidator
        ByteArrayChar8StringDecoder
        ByteArrayChar8StringEncoder
        ByteArrayChar8StringValidator
        ByteArrayUtf8StringDecoder
        ByteArrayUtf8StringEncoder
        ByteArrayUtf8StringValidator
        ByteArrayValidator
        ByteArrayValueStringValidator
)

oktetakastengui_add_sublibrary(OktetaKastenGuiView
    UNTESTED
    PUBLIC
        OktetaKastenCore
        KastenGui
        OktetaGui
    PRIVATE
        KF5::I18n
    KI18N_TRANSLATION_DOMAIN "liboktetakasten"
)

oktetakastengui_sublibrary_sources(OktetaKastenGuiView
    SUBDIR view
    BUILD_INCLUDEDIR
    PUBLIC
        ByteArrayView
        ByteArrayViewFactory
        ByteArrayViewProfile
    PRIVATE
        ByteArrayJanusview
        UserMessagesOverlayLayout
        UserMessagesOverlayedWidget
)

oktetakastengui_sublibrary_sources(OktetaKastenGuiView
    SUBDIR system
    BUILD_INCLUDEDIR
    PUBLIC
        ByteArrayViewProfileLock
        ByteArrayViewProfileManager
)

oktetakastengui_sublibrary_sources(OktetaKastenGuiView
    SUBDIR io
    BUILD_INCLUDEDIR
    PUBLIC
        ByteArrayViewProfileSynchronizer
)


oktetakastengui_add_sublibrary(OktetaKastenGuiStreamEncoder
    UNTESTED
    PUBLIC
        OktetaKastenCore
        KastenGui
        OktetaGui
    PRIVATE
        OktetaKastenGuiWidgets
        OktetaKastenGuiView
        KF5::Completion
        KF5::I18n
        zstd::libzstd
    KI18N_TRANSLATION_DOMAIN "liboktetakasten"
)

macro(OKTETAKASTENGUI_STREAMENCODER_SOURCES)
    okteta_sublibrary_sources(Gui NAMESPACE Okteta Kasten
        REVERSE_NAMESPACE_INCLUDEDIR
        SUBLIBRARY OktetaKastenGuiStreamEncoder
        ${ARGN}
    )
endmacro()

macro(OKTETAKASTENGUI_STREAMCOMPRESSOR_SOURCES)
    # for now handle as stream encoders
    oktetakastengui_streamencoder_sources(
        ${ARGN}
    )
endmacro()

oktetakastengui_streamencoder_sources(
    SUBDIR io/streamencoder/values
    PRIVATE
        ByteArrayValuesStreamEncoderConfigEditor
        ByteArrayValuesStreamEncoderConfigEditorFactory
        ByteArrayValuesStreamEncoder
)

oktetakastengui_streamencoder_sources(
    SUBDIR io/streamencoder/base32
    PRIVATE
        ByteArrayBase32StreamEncoderConfigEditor
        ByteArrayBase32StreamEncoderConfigEditorFactory
        ByteArrayBase32StreamEncoder
)

oktetakastengui_streamencoder_sources(
    SUBDIR io/streamencoder/uuencoding
    PRIVATE
        ByteArrayUuencodingStreamEncoderConfigEditor
        ByteArrayUuencodingStreamEncoderConfigEditorFactory
        ByteArrayUuencodingStreamEncoder
)

oktetakastengui_streamencoder_sources(
    SUBDIR io/streamencoder/xxencoding
    PRIVATE
        ByteArrayXxencodingStreamEncoderConfigEditor
        ByteArrayXxencodingStreamEncoderConfigEditorFactory
        ByteArrayXxencodingStreamEncoder
)

oktetakastengui_streamencoder_sources(
    SUBDIR io/streamencoder/sourcecode
    PRIVATE
        ByteArraySourceCodeStreamEncoderConfigEditor
        ByteArraySourceCodeStreamEncoderConfigEditorFactory
        ByteArraySourceCodeStreamEncoder
)

oktetakastengui_streamencoder_sources(
    SUBDIR io/streamencoder/srec
    PRIVATE
        ByteArraySRecStreamEncoderConfigEditor
        ByteArraySRecStreamEncoderConfigEditorFactory
        ByteArraySRecStreamEncoder
)

oktetakastengui_streamencoder_sources(
    SUBDIR io/streamencoder/ihex
    PRIVATE
        ByteArrayIHexStreamEncoderConfigEditor
        ByteArrayIHexStreamEncoderConfigEditorFactory
        ByteArrayIHexStreamEncoder
)

oktetakastengui_streamencoder_sources(
    SUBDIR io/streamencoder/viewhtml
    PRIVATE
        AbstractColumnHtmlRenderer
        OffsetColumnHtmlRenderer
        AbstractByteArrayColumnHtmlRenderer
        ValueByteArrayColumnHtmlRenderer
        CharByteArrayColumnHtmlRenderer
        ByteArrayRowsColumnHtmlRenderer
        ByteArrayViewHtmlStreamEncoder
)

oktetakastengui_streamencoder_sources(
    SUBDIR io/streamencoder/viewtext
    PRIVATE
        AbstractColumnTextRenderer
        OffsetColumnTextRenderer
        BorderColumnTextRenderer
        AbstractByteArrayColumnTextRenderer
        ValueByteArrayColumnTextRenderer
        CharByteArrayColumnTextRenderer
        ByteArrayRowsColumnTextRenderer
        ByteArrayViewTextStreamEncoder
)

oktetakastengui_streamencoder_sources(
    SUBDIR io/streamencoder/chars
    PRIVATE
        ByteArrayCharsStreamEncoder
)
oktetakastengui_streamencoder_sources(
    SUBDIR io/streamencoder/base64
    PRIVATE
        ByteArrayBase64StreamEncoder
)
oktetakastengui_streamencoder_sources(
    SUBDIR io/streamencoder/base85
    PRIVATE
        ByteArrayBase85StreamEncoder
)

oktetakastengui_streamcompressor_sources(
    SUBDIR io/streamcompressor/zstd
    PRIVATE
        ByteArrayZStdStreamCompressorConfigEditor
        ByteArrayZStdStreamCompressorConfigEditorFactory
        ByteArrayZStdStreamCompressor
        ZStdCompressionStream
)

oktetakastengui_streamcompressor_sources(
    SUBDIR io/streamcompressor
    BUILD_INCLUDEDIR
    PRIVATE
        AbstractByteArrayOutput
        IoDeviceByteArrayOutput
        AbstractCompressionStream
        AbstractByteArrayStreamCompressor
)

oktetakastengui_streamencoder_sources(
    SUBDIR io/streamencoder
    BUILD_INCLUDEDIR
    PUBLIC
        AbstractByteArrayStreamEncoder
    PRIVATE
        ByteArrayTextStreamEncoderPreview
)

oktetakastengui_streamencoder_sources(
    SUBDIR io
    BUILD_INCLUDEDIR
    PUBLIC
        ByteArrayStreamEncoderFactory
        ByteArrayStreamEncoderConfigEditorFactoryFactory
)

oktetakastengui_add_sublibrary(OktetaKastenGuiGenerator
    UNTESTED
    PUBLIC
        OktetaKastenCore
        KastenGui
        OktetaGui
    PRIVATE
        OktetaKastenGuiWidgets
        OktetaKastenGuiView
        KF5::Completion
        KF5::I18n
    KI18N_TRANSLATION_DOMAIN "liboktetakasten"
)

macro(OKTETAKASTENGUI_GENERATOR_SOURCES)
    okteta_sublibrary_sources(Gui NAMESPACE Okteta Kasten
        REVERSE_NAMESPACE_INCLUDEDIR
        SUBLIBRARY OktetaKastenGuiGenerator
        ${ARGN}
    )
endmacro()

oktetakastengui_generator_sources(
    SUBDIR io/generator/pattern
    PRIVATE
        ByteArrayPatternGeneratorconfigEditor
        ByteArrayPatternGeneratorconfigEditorFactory
        ByteArrayPatternGenerator
)

oktetakastengui_generator_sources(
    SUBDIR io/generator/randomdata
    PRIVATE
        ByteArrayRandomDataGeneratorconfigEditor
        ByteArrayRandomDataGeneratorconfigEditorFactory
        ByteArrayRandomDataGenerator
)

oktetakastengui_generator_sources(
    SUBDIR io/generator/sequence
    PRIVATE
        ByteArraySequenceGenerator
)

oktetakastengui_generator_sources(
    SUBDIR io
    BUILD_INCLUDEDIR
    PUBLIC
        ByteArrayDataGeneratorFactory
        ByteArrayDataGeneratorConfigEditorFactoryFactory
)


oktetakastengui_sources(
    PUBLIC
        OktetaKastenGui
)

okteta_add_cmakeconfig(Gui NAMESPACE Okteta Kasten
    DEPS
        OktetaKastenCore
        KastenGui
        OktetaGui
)

if(BUILD_TESTING)
    add_subdirectory(liboktetawidgets/tests)
endif()
