set(OKTETAKASTENGUI_VERSION ${OKTETAKASTEN_LIB_VERSION})
set(OKTETAKASTENGUI_SO_VERSION ${OKTETAKASTEN_LIB_SOVERSION})

okteta_add_library(Gui NAMESPACE Okteta Kasten
    PUBLIC
        OktetaKastenCore
        KastenGui
        OktetaGui
    PRIVATE
        Qt5::Qml
        KF5::I18n
        KF5::Completion
    VERSION     ${OKTETAKASTENGUI_VERSION}
    SOVERSION   ${OKTETAKASTENGUI_SO_VERSION}
    ABIVERSION  ${OKTETAKASTEN_ABI_VERSION} ${KASTEN_ABI_VERSION}
    NO_VERSIONED_INCLUDEDIR
    NO_VERSIONED_PACKAGE_NAME
    REVERSE_NAMESPACE_INCLUDEDIR
    REVERSE_NAMESPACE_LIB
    NO_TARGET_NAMESPACE
)

target_compile_definitions(OktetaKastenGui PRIVATE -DTRANSLATION_DOMAIN=\"liboktetakasten\")

ecm_qt_declare_logging_category(OktetaKastenGui
    HEADER "logging.hpp"
    IDENTIFIER "LOG_KASTEN_OKTETA_GUI"
    CATEGORY_NAME "kasten.okteta.gui"
    DESCRIPTION "Kasten Okteta Gui module"
    EXPORT OKTETA
)

macro(OKTETAKASTENGUI_ADD_SUBLIBRARY _subdir)
    okteta_add_sublibrary(Gui NAMESPACE Okteta Kasten
        REVERSE_NAMESPACE_INCLUDEDIR
        SUBDIR ${_subdir}
        ${ARGN}
    )
endmacro()

oktetakastengui_add_sublibrary(liboktetawidgets
    BUILD_INCLUDEDIR
    PUBLIC
        AddressValidator
        AddressComboBox
        ByteArrayComboBox
    PRIVATE
        ByteArrayValidator
)

oktetakastengui_add_sublibrary(io/streamencoder/values
    PRIVATE
        ByteArrayValuesStreamEncoderConfigEditor
        ByteArrayValuesStreamEncoderConfigEditorFactory
        ByteArrayValuesStreamEncoder
)

oktetakastengui_add_sublibrary(io/streamencoder/base32
    PRIVATE
        ByteArrayBase32StreamEncoderConfigEditor
        ByteArrayBase32StreamEncoderConfigEditorFactory
        ByteArrayBase32StreamEncoder
)

oktetakastengui_add_sublibrary(io/streamencoder/uuencoding
    PRIVATE
        ByteArrayUuencodingStreamEncoderConfigEditor
        ByteArrayUuencodingStreamEncoderConfigEditorFactory
        ByteArrayUuencodingStreamEncoder
)

oktetakastengui_add_sublibrary(io/streamencoder/xxencoding
    PRIVATE
        ByteArrayXxencodingStreamEncoderConfigEditor
        ByteArrayXxencodingStreamEncoderConfigEditorFactory
        ByteArrayXxencodingStreamEncoder
)

oktetakastengui_add_sublibrary(io/streamencoder/sourcecode
    PRIVATE
        ByteArraySourceCodeStreamEncoderConfigEditor
        ByteArraySourceCodeStreamEncoderConfigEditorFactory
        ByteArraySourceCodeStreamEncoder
)

oktetakastengui_add_sublibrary(io/streamencoder/srec
    PRIVATE
        ByteArraySRecStreamEncoderConfigEditor
        ByteArraySRecStreamEncoderConfigEditorFactory
        ByteArraySRecStreamEncoder
)

oktetakastengui_add_sublibrary(io/streamencoder/ihex
    PRIVATE
        ByteArrayIHexStreamEncoderConfigEditor
        ByteArrayIHexStreamEncoderConfigEditorFactory
        ByteArrayIHexStreamEncoder
)

oktetakastengui_add_sublibrary(io/streamencoder/viewtext
    PRIVATE
        AbstractColumnTextRenderer
        OffsetColumnTextRenderer
        BorderColumnTextRenderer
        AbstractByteArrayColumnTextRenderer
        ValueByteArrayColumnTextRenderer
        CharByteArrayColumnTextRenderer
        ByteArrayrowsColumnTextRenderer
        ByteArrayViewTextStreamEncoder
)

oktetakastengui_add_sublibrary(io/streamencoder/chars
    PRIVATE
        ByteArrayCharsStreamEncoder
)
oktetakastengui_add_sublibrary(io/streamencoder/base64
    PRIVATE
        ByteArrayBase64StreamEncoder
)
oktetakastengui_add_sublibrary(io/streamencoder/base85
    PRIVATE
        ByteArrayBase85StreamEncoder
)

oktetakastengui_add_sublibrary(io/streamencoder
    BUILD_INCLUDEDIR
    PUBLIC
        AbstractByteArrayStreamEncoder
    PRIVATE
        ByteArrayTextStreamEncoderPreview
)

oktetakastengui_add_sublibrary(io/generator/pattern
    PRIVATE
        ByteArrayPatternGeneratorconfigEditor
        ByteArrayPatternGeneratorconfigEditorFactory
        ByteArrayPatternGenerator
)

oktetakastengui_add_sublibrary(io/generator/randomdata
    PRIVATE
        ByteArrayRandomDataGeneratorconfigEditor
        ByteArrayRandomDataGeneratorconfigEditorFactory
        ByteArrayRandomDataGenerator
)

oktetakastengui_add_sublibrary(io/generator/sequence
    PRIVATE
        ByteArraySequenceGenerator
)

oktetakastengui_add_sublibrary(io
    BUILD_INCLUDEDIR
    PUBLIC
        ByteArrayStreamEncoderFactory
        ByteArrayStreamEncoderConfigEditorFactoryFactory

        ByteArrayDataGeneratorFactory
        ByteArrayDataGeneratorConfigEditorFactoryFactory

        ByteArrayViewProfileSynchronizer
)

oktetakastengui_add_sublibrary(view
    BUILD_INCLUDEDIR
    PUBLIC
        ByteArrayView
        ByteArrayViewFactory
        ByteArrayViewProfile
    PRIVATE
        ByteArrayJanusview
)

oktetakastengui_add_sublibrary(system
    BUILD_INCLUDEDIR
    PUBLIC
        ByteArrayViewProfileLock
        ByteArrayViewProfileManager
)

okteta_add_sublibrary(Gui NAMESPACE Okteta Kasten
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
