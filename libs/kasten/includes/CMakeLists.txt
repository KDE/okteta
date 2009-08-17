set( kastencore_CamelCase_HDRS
  AbstractModel
  AbstractModelSelection
  AbstractDocument
  AbstractDocumentFactory
  AbstractTool
  UserListable
  Versionable

  AbstractModelSynchronizer
  AbstractModelSynchronizerFactory
  AbstractSyncToRemoteJob
  AbstractSyncWithRemoteJob
  AbstractSyncFromRemoteJob
  AbstractConnectJob
  AbstractLoadJob
  AbstractModelStreamEncoder
  AbstractModelExporter
  AbstractExportJob

  AbstractModelFileSystemSynchronizer
  AbstractFileSystemSyncWithRemoteJob
  AbstractFileSystemSyncFromRemoteJob
  AbstractFileSystemSyncToRemoteJob
  AbstractFileSystemConnectJob
  AbstractFileSystemLoadJob
  AbstractFileSystemExportJob
  ModelEncoderFileSystemExporter
  ModelEncoderFileSystemExportJob
  ModelStreamEncodeThread

  DocumentVersionData
  Person

  JobManager
  DocumentManager
  DocumentCreateManager
  DocumentSyncManager
  ModelCodecManager
)

set( kastengui_CamelCase_HDRS
  AbstractView
  AbstractViewFactory
  AbstractSelectionView
  AbstractToolView
  DataSelectable
  SelectedDataWriteable
  Zoomable

  AbstractXmlGuiController
  AbstractModelStreamEncoderConfigEditor
  AbstractModelStreamEncoderConfigEditorFactory
  AbstractModelExporterConfigEditor
  AbstractModelExporterConfigEditorFactory
  ModelEncoderFileSystemExporterConfigEditor
  ModelEncoderFileSystemExporterConfigEditorFactory
  AbstractToolInlineView
  AbstractViewArea
  AbstractGroupedViews
  ViewAreaSplitable
  ToolInlineViewable
  WidgetsDockable

  TabbedViews
  MultiViewAreas
  ShellWindow

  DummyView
  AbstractLinePopup
  StatusBar
  ToggleButton

  ViewManager
  ModelCodecViewManager
)

set( kastencontrollers_CamelCase_HDRS
  ReadOnlyBarController
  ReadOnlyController
  VersionViewTool
  VersionViewToolView
  CloseController
  DocumentsTool
  DocumentsToolView
  CreatorController
  LoaderController
  FileSystemBrowserTool
  FileSystemBrowserToolView
  ExportController
  SetRemoteController
  CopyAsController
  ClipboardController
  SynchronizeController
  SelectController
  ZoomController
  ZoomBarController
  VersionController
  FullScreenController
  ToolListMenuController
  SwitchViewController
  ViewListMenuController
  QuitController
)

set( kasten_CamelCase_HDRS
  ${kastencore_CamelCase_HDRS}
  ${kastengui_CamelCase_HDRS}
  ${kastencontrollers_CamelCase_HDRS}
)

install( FILES ${kasten_CamelCase_HDRS}  DESTINATION ${INCLUDE_INSTALL_DIR}/KDE/Kasten COMPONENT Devel)
