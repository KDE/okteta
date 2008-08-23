project( processdocuments )

set( PROCESSDOCUMENTS_SRCS
  abstractprocessdocument.cpp
  processdocumentmanager.cpp
  processdocumenttablemodel.cpp
  processdocumentconnectdialog.cpp
)

kde4_add_library( processdocuments STATIC ${PROCESSDOCUMENTS_SRCS} )
