include(ECMAddQtDesignerPlugin)

set(ByteArrayRowView_CREATE_WIDGET_CODE
"
    auto* widget = new Okteta::ByteArrayRowView(parent);
    widget->setByteArrayModel(createFillerByteArrayModel(widget));
    return widget;
")
ecm_qtdesignerplugin_widget(ByteArrayRowView
    CLASS_NAME "Okteta::ByteArrayRowView"
    TOOLTIP "Row oriented Viewer/Editor for Byte Arrays"
    WHATSTHIS "A widget to show and edit byte arrays provided by subclasses of Okteta::AbstractByteArrayModel"
    CREATE_WIDGET_CODE_FROM_VARIABLE ByteArrayRowView_CREATE_WIDGET_CODE
)

set(ByteArrayColumnView_CREATE_WIDGET_CODE
"
    auto* widget = new Okteta::ByteArrayColumnView(parent);
    widget->setByteArrayModel(createFillerByteArrayModel(widget));
    return widget;
")
ecm_qtdesignerplugin_widget(Okteta::ByteArrayColumnView
    TOOLTIP "Column oriented Viewer/Editor for Byte Arrays"
    WHATSTHIS "A widget to show and edit byte arrays provided by subclasses of Okteta::AbstractByteArrayModel"
    CREATE_WIDGET_CODE_FROM_VARIABLE ByteArrayColumnView_CREATE_WIDGET_CODE
)

ecm_add_qtdesignerplugin(oktetadesignerplugin
    NAME OktetaWidgets
    OUTPUT_NAME oktetawidgets
    WIDGETS
        ByteArrayRowView
        Okteta::ByteArrayColumnView
    DEFAULT_GROUP "Okteta"
    DEFAULT_HEADER_EXTENSION "hpp"
    INCLUDE_FILES "\"fillerbytearraymodel.hpp\""
    SOURCES fillerbytearraymodel.cpp
    LINK_LIBRARIES
        OktetaGui
        OktetaCore
    INSTALL_DESTINATION "${KDE_INSTALL_QTPLUGINDIR}/designer"
)

if( OKTETA_BUILD_EXAMPLES )
    add_subdirectory(examples)
endif()
