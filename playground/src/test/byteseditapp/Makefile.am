noinst_HEADERS = byteseditapp.h byteseditappview.h pref.h

# this is the program that gets installed.  it's name is used for all
# of the other Makefile.am variables
bin_PROGRAMS = byteseditapp byteseditapp_client

# set the include path for X, qt and KDE
INCLUDES = -I../../lib $(all_includes)

byteseditapp_LDFLAGS = $(KDE_RPATH) $(all_libraries)
byteseditapp_LDADD = ../../lib/libkhexedit.la $(LIB_KFILE)

byteseditapp_SOURCES = main.cpp byteseditapp.cpp byteseditappview.cpp \
                       pref.cpp byteseditappiface.skel

# client stuff
byteseditapp_client_LDFLAGS = $(KDE_RPATH) $(all_libraries)
byteseditapp_client_LDADD   = $(LIB_KDECORE)
byteseditapp_client_SOURCES = byteseditapp_client.cpp

# this is where the kdelnk file will go
kdelnkdir   = $(kde_appsdir)/Utilities
kdelnk_DATA = byteseditapp.desktop

# this is where the XML-GUI resource file goes
rcdir = $(kde_datadir)/byteseditapp
rc_DATA = byteseditappui.rc

METASOURCES = AUTO

