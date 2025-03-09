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
)

target_compile_definitions(OktetaKastenGui PRIVATE -DTRANSLATION_DOMAIN=\"liboktetakasten\")

ecm_qt_declare_logging_category(OktetaKastenGui
    HEADER "logging.hpp"
    IDENTIFIER "LOG_KASTEN_OKTETA_GUI"
    CATEGORY_NAME "kasten.okteta.gui"
    DESCRIPTION "Kasten Okteta Gui module"
    EXPORT OKTETA
)

macro(OKTETAKASTENGUI_LIBRARY_SOURCES _subdir)
    okteta_library_sources(Gui NAMESPACE Okteta Kasten
        REVERSE_NAMESPACE_INCLUDEDIR
        SUBDIR ${_subdir}
        ${ARGN}
    )
endmacro()

macro(OKTETAKASTENGUI_SUBLIBRARY_SOURCES _subdir)
    okteta_sublibrary_sources(Gui NAMESPACE Okteta Kasten
        REVERSE_NAMESPACE_INCLUDEDIR
        SUBDIR ${_subdir}
        ${ARGN}
    )
endmacro()

okteta_add_sublibrary(Gui NAMESPACE Okteta Kasten
    SUBLIBRARY oktetawidgets
    UNTESTED
    PUBLIC
        OktetaCore
        Qt5::Widgets
    PRIVATE
        KF5::Completion
        KF5::I18n
        Qt5::Qml
)
okteta_sublibrary_compile_definitions(Gui NAMESPACE Okteta Kasten
    SUBLIBRARY oktetawidgets
    PRIVATE
        TRANSLATION_DOMAIN=\"liboktetakasten\"
)

oktetakastengui_sublibrary_sources(liboktetawidgets
    SUBLIBRARY oktetawidgets
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

okteta_add_sublibrary(Gui NAMESPACE Okteta Kasten
    SUBLIBRARY bytearrayview
    UNTESTED
    PUBLIC
        OktetaKastenCore
        KastenGui
        OktetaGui
    PRIVATE
        KF5::I18n
)
okteta_sublibrary_compile_definitions(Gui NAMESPACE Okteta Kasten
    SUBLIBRARY bytearrayview
    PRIVATE
        TRANSLATION_DOMAIN=\"liboktetakasten\"
)

oktetakastengui_sublibrary_sources(view
    SUBLIBRARY bytearrayview
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

oktetakastengui_sublibrary_sources(system
    SUBLIBRARY bytearrayview
    BUILD_INCLUDEDIR
    PUBLIC
        ByteArrayViewProfileLock
        ByteArrayViewProfileManager
)

oktetakastengui_sublibrary_sources(io
    SUBLIBRARY bytearrayview
    BUILD_INCLUDEDIR
    PUBLIC
        ByteArrayViewProfileSynchronizer
)


okteta_add_sublibrary(Gui NAMESPACE Okteta Kasten
    SUBLIBRARY bytearraystreamencoder
    UNTESTED
    PUBLIC
        OktetaKastenCore
        KastenGui
        OktetaGui
    PRIVATE
        oktetawidgets
        bytearrayview
        KF5::Completion
        KF5::I18n
)
okteta_sublibrary_compile_definitions(Gui NAMESPACE Okteta Kasten
    SUBLIBRARY bytearraystreamencoder
    PRIVATE
        TRANSLATION_DOMAIN=\"liboktetakasten\"
)
macro(OKTETAKASTENGUI_STREAMENCODER_SOURCES _subdir)
    okteta_sublibrary_sources(Gui NAMESPACE Okteta Kasten
        REVERSE_NAMESPACE_INCLUDEDIR
        SUBLIBRARY bytearraystreamencoder
        SUBDIR ${_subdir}
        ${ARGN}
    )
endmacro()

oktetakastengui_streamencoder_sources(io/streamencoder/values
    PRIVATE
        ByteArrayValuesStreamEncoderConfigEditor
        ByteArrayValuesStreamEncoderConfigEditorFactory
        ByteArrayValuesStreamEncoder
)

oktetakastengui_streamencoder_sources(io/streamencoder/base32
    PRIVATE
        ByteArrayBase32StreamEncoderConfigEditor
        ByteArrayBase32StreamEncoderConfigEditorFactory
        ByteArrayBase32StreamEncoder
)

oktetakastengui_streamencoder_sources(io/streamencoder/uuencoding
    PRIVATE
        ByteArrayUuencodingStreamEncoderConfigEditor
        ByteArrayUuencodingStreamEncoderConfigEditorFactory
        ByteArrayUuencodingStreamEncoder
)

oktetakastengui_streamencoder_sources(io/streamencoder/xxencoding
    PRIVATE
        ByteArrayXxencodingStreamEncoderConfigEditor
        ByteArrayXxencodingStreamEncoderConfigEditorFactory
        ByteArrayXxencodingStreamEncoder
)

oktetakastengui_streamencoder_sources(io/streamencoder/sourcecode
    PRIVATE
        ByteArraySourceCodeStreamEncoderConfigEditor
        ByteArraySourceCodeStreamEncoderConfigEditorFactory
        ByteArraySourceCodeStreamEncoder
)

oktetakastengui_streamencoder_sources(io/streamencoder/srec
    PRIVATE
        ByteArraySRecStreamEncoderConfigEditor
        ByteArraySRecStreamEncoderConfigEditorFactory
        ByteArraySRecStreamEncoder
)

oktetakastengui_streamencoder_sources(io/streamencoder/ihex
    PRIVATE
        ByteArrayIHexStreamEncoderConfigEditor
        ByteArrayIHexStreamEncoderConfigEditorFactory
        ByteArrayIHexStreamEncoder
)

oktetakastengui_streamencoder_sources(io/streamencoder/viewhtml
    PRIVATE
        AbstractColumnHtmlRenderer
        OffsetColumnHtmlRenderer
        AbstractByteArrayColumnHtmlRenderer
        ValueByteArrayColumnHtmlRenderer
        CharByteArrayColumnHtmlRenderer
        ByteArrayRowsColumnHtmlRenderer
        ByteArrayViewHtmlStreamEncoder
)

oktetakastengui_streamencoder_sources(io/streamencoder/viewtext
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

oktetakastengui_streamencoder_sources(io/streamencoder/chars
    PRIVATE
        ByteArrayCharsStreamEncoder
)
oktetakastengui_streamencoder_sources(io/streamencoder/base64
    PRIVATE
        ByteArrayBase64StreamEncoder
)
oktetakastengui_streamencoder_sources(io/streamencoder/base85
    PRIVATE
        ByteArrayBase85StreamEncoder
)

oktetakastengui_streamencoder_sources(io/streamencoder
    BUILD_INCLUDEDIR
    PUBLIC
        AbstractByteArrayStreamEncoder
    PRIVATE
        ByteArrayTextStreamEncoderPreview
)

oktetakastengui_streamencoder_sources(io
    BUILD_INCLUDEDIR
    PUBLIC
        ByteArrayStreamEncoderFactory
        ByteArrayStreamEncoderConfigEditorFactoryFactory
)

okteta_add_sublibrary(Gui NAMESPACE Okteta Kasten
    SUBLIBRARY bytearraygenerator
    UNTESTED
    PUBLIC
        OktetaKastenCore
        KastenGui
        OktetaGui
    PRIVATE
        oktetawidgets
        bytearrayview
        KF5::Completion
        KF5::I18n
)
okteta_sublibrary_compile_definitions(Gui NAMESPACE Okteta Kasten
    SUBLIBRARY bytearraygenerator
    PRIVATE
        TRANSLATION_DOMAIN=\"liboktetakasten\"
)
macro(OKTETAKASTENGUI_GENERATOR_SOURCES _subdir)
    okteta_sublibrary_sources(Gui NAMESPACE Okteta Kasten
        REVERSE_NAMESPACE_INCLUDEDIR
        SUBLIBRARY bytearraygenerator
        SUBDIR ${_subdir}
        ${ARGN}
    )
endmacro()

oktetakastengui_generator_sources(io/generator/pattern
    PRIVATE
        ByteArrayPatternGeneratorconfigEditor
        ByteArrayPatternGeneratorconfigEditorFactory
        ByteArrayPatternGenerator
)

oktetakastengui_generator_sources(io/generator/randomdata
    PRIVATE
        ByteArrayRandomDataGeneratorconfigEditor
        ByteArrayRandomDataGeneratorconfigEditorFactory
        ByteArrayRandomDataGenerator
)

oktetakastengui_generator_sources(io/generator/sequence
    PRIVATE
        ByteArraySequenceGenerator
)

oktetakastengui_generator_sources(io
    BUILD_INCLUDEDIR
    PUBLIC
        ByteArrayDataGeneratorFactory
        ByteArrayDataGeneratorConfigEditorFactoryFactory
)


okteta_library_sources(Gui NAMESPACE Okteta Kasten
    REVERSE_NAMESPACE_INCLUDEDIR
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
