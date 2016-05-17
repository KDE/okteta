ecm_setup_version( ${KASTEN_LIB_VERSION}
    VARIABLE_PREFIX KASTENGUI
    SOVERSION ${KASTEN_LIB_SOVERSION}
    PACKAGE_VERSION_FILE "${CMAKE_CURRENT_BINARY_DIR}/KastenGuiConfigVersion.cmake"
)

add_definitions(-DTRANSLATION_DOMAIN=\"libkasten\")

include_directories(
  view
  io
  system
  shell
  controller
)


set( kastengui_view_SRCS
  view/abstractview.cpp
  view/abstractselectionview.cpp
  view/abstracttoolinlineview.cpp
  view/abstracttoolview.cpp
)
ecm_generate_headers(kastengui_view_CCHDRS
  HEADER_NAMES
    AbstractViewFactory
    SelectedDataWriteable
    AbstractToolInlineView
    AbstractView
    AbstractSelectionView
    AbstractToolView
    AbstractToolViewFactory
    DataSelectable
    Zoomable
  RELATIVE view
  PREFIX Kasten
  REQUIRED_HEADERS kastengui_view_HDRS
)

set( kastengui_io_configeditor_SRCS
  io/filesystem/modelencoderfilesystemexporterconfigeditorfactory.cpp
  io/filesystem/modelencoderfilesystemexporterconfigeditor.cpp
  io/abstractmodelstreamencoderconfigeditor.cpp
  io/abstractmodelexporterconfigeditor.cpp
  io/abstractmodeldatageneratorconfigeditor.cpp
)
set( kastengui_io_SRCS
  ${kastengui_io_configeditor_SRCS}
)
ecm_generate_headers(kastengui_io_CCHDRS
  HEADER_NAMES
    ModelEncoderFileSystemExporterConfigEditor
    ModelEncoderFileSystemExporterConfigEditorFactory
  RELATIVE io/filesystem
  PREFIX Kasten
  REQUIRED_HEADERS kastengui_io_HDRS
)
ecm_generate_headers(kastengui_io_CCHDRS
  HEADER_NAMES
    AbstractModelStreamEncoderConfigEditor
    AbstractModelExporterConfigEditor
    AbstractModelDataGeneratorConfigEditor
    AbstractModelStreamEncoderConfigEditorFactory
    AbstractModelExporterConfigEditorFactory
    AbstractModelDataGeneratorConfigEditorFactory
  RELATIVE io
  PREFIX Kasten
  REQUIRED_HEADERS kastengui_io_HDRS
)


set( kastengui_system_SRCS
  system/abstractdocumentstrategy.cpp
  system/createdialog.cpp
  system/dialoghandler.cpp
  system/dummyview.cpp
  system/modelcodecviewmanager.cpp
  system/multidocumentstrategy_p.cpp
  system/multidocumentstrategy.cpp
  system/singledocumentstrategy_p.cpp
  system/singledocumentstrategy.cpp
  system/viewmanager.cpp
)
ecm_generate_headers(kastengui_system_CCHDRS
  HEADER_NAMES
    AbstractDocumentStrategy
    DialogHandler
    DummyView
    ModelCodecViewManager
    MultiDocumentStrategy
    SingleDocumentStrategy
    ViewManager
  RELATIVE system
  PREFIX Kasten
  REQUIRED_HEADERS kastengui_system_HDRS
)

set( kastengui_shell_SRCS
  shell/viewbox.cpp
  shell/toolinlineviewwidget.cpp
  shell/toolviewdockwidget.cpp
  shell/abstractviewarea.cpp
  shell/abstractgroupedviews.cpp
  shell/viewareabox.cpp
  shell/singleviewarea_p.cpp
  shell/singleviewarea.cpp
  shell/tabbedviews_p.cpp
  shell/tabbedviews.cpp
  shell/multiviewareas_p.cpp
  shell/multiviewareas.cpp
  shell/singleviewwindow_p.cpp
  shell/singleviewwindow.cpp
  shell/shellwindow_p.cpp
  shell/shellwindow.cpp
  shell/statusbar.cpp
  shell/statusbarlayout.cpp
)
ecm_generate_headers(kastengui_shell_CCHDRS
  HEADER_NAMES
    ShellWindow
    AbstractViewArea
    AbstractGroupedViews
    StatusBar
    ViewAreaBox
    ViewBox
    SingleViewArea
    SingleViewWindow
    TabbedViews
    MultiViewAreas
    ToolInlineViewWidget
    ToolViewDockWidget
    StatusBarLayout
    ToolInlineViewable
    WidgetsDockable
    ViewAreaSplitable
  RELATIVE shell
  PREFIX Kasten
  REQUIRED_HEADERS kastengui_shell_HDRS
)

set( kastengui_controller_widgets_SRCS
  controller/togglebutton_p.cpp
  controller/togglebutton.cpp
  controller/abstractlinepopup_p.cpp
  controller/abstractlinepopup.cpp
  controller/abstracttoolwidget_p.cpp
  controller/abstracttoolwidget.cpp
)
set( kastengui_controller_SRCS
  ${kastengui_controller_widgets_SRCS}
  controller/abstractcontroller.cpp
  controller/abstractxmlguicontroller.cpp
)
ecm_generate_headers(kastengui_controller_CCHDRS
  HEADER_NAMES
    AbstractController
    AbstractXmlGuiController
    AbstractLinePopup
    ToggleButton
    AbstractToolWidget
  RELATIVE controller
  PREFIX Kasten
  REQUIRED_HEADERS kastengui_controller_HDRS
)

set( kastengui_SRCS
  kastengui.cpp
)
ecm_generate_headers(kastengui_CCHDRS
  HEADER_NAMES
    KastenGui
  PREFIX Kasten
  REQUIRED_HEADERS kastengui_HDRS
)

set( kastengui_LIB_SRCS
  ${kastengui_SRCS}
  ${kastengui_view_SRCS}
  ${kastengui_io_SRCS}
  ${kastengui_system_SRCS}
  ${kastengui_shell_SRCS}
  ${kastengui_controller_SRCS}
)

set( kastengui_LIB_HDRS
  ${CMAKE_CURRENT_BINARY_DIR}/kasten/kastengui_export.h
  ${kastengui_HDRS}
  ${kastengui_view_HDRS}
  ${kastengui_io_HDRS}
  ${kastengui_system_HDRS}
  ${kastengui_shell_HDRS}
  ${kastengui_controller_HDRS}
)

set( kastengui_LIB_CCHDRS
  ${kastengui_CCHDRS}
  ${kastengui_view_CCHDRS}
  ${kastengui_io_CCHDRS}
  ${kastengui_system_CCHDRS}
  ${kastengui_shell_CCHDRS}
  ${kastengui_controller_CCHDRS}
)

add_library( KastenGui SHARED ${kastengui_LIB_SRCS} )

generate_export_header( KastenGui EXPORT_FILE_NAME ${CMAKE_CURRENT_BINARY_DIR}/kasten/kastengui_export.h)

target_link_libraries( KastenGui
PUBLIC
  KastenCore
  Qt5::Widgets
  KF5::XmlGui
PRIVATE
  KF5::I18n
)

target_include_directories(KastenGui PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>")

set_target_properties( KastenGui  PROPERTIES
  EXPORT_NAME "KastenGui"
  OUTPUT_NAME kasten${KASTEN_ABI_VERSION}gui
  VERSION     ${KASTENGUI_VERSION}
  SOVERSION   ${KASTENGUI_SOVERSION}
)

install( TARGETS KastenGui  EXPORT KastenGuiTargets ${INSTALL_TARGETS_DEFAULT_ARGS} )

install( FILES ${kastengui_LIB_HDRS}
  DESTINATION ${KASTEN_INCLUDE_INSTALL_DIR}
  COMPONENT   Devel
)
install( FILES ${kastengui_LIB_CCHDRS}
  DESTINATION ${KASTEN_CCINCLUDE_INSTALL_DIR}
  COMPONENT Devel
)

# create a Config.cmake and a ConfigVersion.cmake file and install them
set(CMAKECONFIG_INSTALL_DIR "${CMAKECONFIG_INSTALL_PREFIX}/KastenGui")

ecm_configure_package_config_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/KastenGuiConfig.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/KastenGuiConfig.cmake"
  INSTALL_DESTINATION ${CMAKECONFIG_INSTALL_DIR}
)

install( FILES
    "${CMAKE_CURRENT_BINARY_DIR}/KastenGuiConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/KastenGuiConfigVersion.cmake"
  DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
  COMPONENT Devel
)

install(EXPORT KastenGuiTargets
  DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
  FILE KastenGuiTargets.cmake
)
