noinst_HEADERS = byteseditapp.h

check_PROGRAMS = byteseditapp

# set the include path for X, qt and KDE
INCLUDES = $(all_includes) 
#-I$(srcdir)/../../interface

byteseditapp_LDFLAGS = $(KDE_RPATH) $(all_libraries)
byteseditapp_LDADD = $(LIB_KFILE)

byteseditapp_SOURCES = main.cpp byteseditapp.cpp

# this is where the XML-GUI resource file goes
rcdir = $(kde_datadir)/byteseditapp
rc_DATA = byteseditappui.rc

METASOURCES = AUTO

