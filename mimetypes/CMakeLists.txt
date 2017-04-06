# shared-mime-info 0.40 is mandatory for generic-icon
set( SHARED_MIME_INFO_MINIMUM_VERSION "0.40" )
find_package( SharedMimeInfo REQUIRED )

install( FILES okteta.xml  DESTINATION ${KDE_INSTALL_MIMEDIR} )
update_xdg_mimetypes( ${KDE_INSTALL_MIMEDIR} )
