if (${ECM_VERSION} VERSION_GREATER "5.61.0")
include(ECMAddQtDesignerPlugin)

ecm_qtdesignerplugin_widget(ByteArrayRowView
    CLASS_NAME "Okteta::ByteArrayRowView"
    TOOLTIP "Row oriented Viewer/Editor for Byte Arrays"
    WHATSTHIS "A widget to show and edit byte arrays provided by subclasses of Okteta::AbstractByteArrayModel"
    CREATE_WIDGET_CODE
"
    auto* widget = new Okteta::ByteArrayRowView(parent)@SEMICOLON@
    widget->setByteArrayModel(createFillerByteArrayModel(widget))@SEMICOLON@
    return widget@SEMICOLON@
"
)

ecm_qtdesignerplugin_widget(Okteta::ByteArrayColumnView
    TOOLTIP "Column oriented Viewer/Editor for Byte Arrays"
    WHATSTHIS "A widget to show and edit byte arrays provided by subclasses of Okteta::AbstractByteArrayModel"
    CREATE_WIDGET_CODE
 "
         auto* widget = new Okteta::ByteArrayColumnView(parent)@SEMICOLON@
         widget->setByteArrayModel(createFillerByteArrayModel(widget))@SEMICOLON@
         return widget@SEMICOLON@
 "
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

else()

set( oktetadesignerplugin_SRCS
  bytearraycolumnviewdesignerfactory.cpp
  bytearrayrowviewdesignerfactory.cpp
  oktetawidgetdesignerfactorycollection.cpp
  fillerbytearraymodel.cpp
)


add_library( oktetadesignerplugin  MODULE ${oktetadesignerplugin_SRCS} )

target_link_libraries( oktetadesignerplugin
  OktetaGui
  OktetaCore
  Qt5::Widgets
  Qt5::UiPlugin
)

set_target_properties( oktetadesignerplugin PROPERTIES
    OUTPUT_NAME oktetawidgets
)

install( TARGETS oktetadesignerplugin  DESTINATION ${KDE_INSTALL_QTPLUGINDIR}/designer )

endif()

if( OKTETA_BUILD_EXAMPLES )
add_subdirectory( examples )
endif()
