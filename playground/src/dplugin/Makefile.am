INCLUDES = -I../lib $(all_includes)

AM_CXXFLAGS = -DQT_PLUGIN

kde_widget_LTLIBRARIES = kheplugin.la
kheplugin_la_LDFLAGS = -module $(all_libraries) $(KDE_PLUGIN)
kheplugin_la_LIBADD = ../lib/libkhexedit.la $(LIB_KIO)
kheplugin_la_SOURCES = designerplugin.cpp


kheplugindata_DATA =  kbytesedit.png
kheplugindatadir = $(kde_datadir)/khexedit2/pics

EXTRA_DIST = $(kheplugindata_DATA)

METASOURCES = AUTO
