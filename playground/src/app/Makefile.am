INCLUDES = $(all_includes)

bin_PROGRAMS = khexedit2 khexedit2_client

khexedit2_LDFLAGS = $(KDE_RPATH) $(all_libraries)
khexedit2_LDADD   = $(LIB_KFILE) $(LIB_KDEPRINT)

khexedit2_SOURCES = main.cpp khexedit2.cpp pref.cpp

noinst_HEADERS = khexedit2.h pref.h

khexedit2_client_LDFLAGS = $(KDE_RPATH) $(all_libraries)
khexedit2_client_LDADD   = $(LIB_KDECORE)
khexedit2_client_SOURCES = khexedit2_client.cpp

METASOURCES = AUTO

messages: rc.cpp
	$(XGETTEXT) *.cpp -o $(podir)/khexedit.pot


# this is where the kdelnk file will go
kdelnkdir = $(kde_appsdir)/Utilities
kdelnk_DATA = khexedit2.desktop

# this is where the XML-GUI resource file goes
rcdir = $(kde_datadir)/khexedit2
rc_DATA = khexedit2ui.rc
