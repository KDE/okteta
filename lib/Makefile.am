#messages: rc.cpp
#	$(XGETTEXT) *.cpp -o $(podir)/libkhexedit.pot

INCLUDES    = $(all_includes)

lib_LTLIBRARIES =  libkhexedit.la
libkhexedit_la_LDFLAGS = -version-info 1:7:1 $(all_libraries) $(KDE_RPATH)
libkhexedit_la_SOURCES = kcolumn.cpp kbordercolumn.cpp koffsetcolumn.cpp kbuffercolumn.cpp khexcolumn.cpp ktextcolumn.cpp \
                         kcolumnsview.cpp khexedit.cpp kbytesedit.cpp \
                         kbytecodec.cpp koffsetformat.cpp \
                         kdatabuffer.cpp kwrappingrobuffer.cpp \
                         kplainbuffer.cpp kfixedsizebuffer.cpp kbigbuffer.cpp \
                         kbuffercursor.cpp kbufferlayout.cpp kbufferranges.cpp \
                         kcursor.cpp kbufferdrag.cpp \
                         ksectionlist.cpp kcoordrangelist.cpp \
                         kbordercoltextexport.cpp koffsetcoltextexport.cpp \
                         kbuffercoltextexport.cpp khexcoltextexport.cpp ktextcoltextexport.cpp

noinst_HEADERS = kcolumn.h kbordercolumn.h koffsetcolumn.h kbuffercolumn.h khexcolumn.h ktextcolumn.h \
                 kbytecodec.h koffsetformat.h \
                 kbuffercursor.h kbufferlayout.h kbufferranges.h \
                 kbuffercoord.h kselection.h \
                 kcursor.h kbufferdrag.h \
                 kcoordrange.h ksectionlist.h kcoordrangelist.h \
                 kcoltextexport.h kbordercoltextexport.h koffsetcoltextexport.h \
                 kbuffercoltextexport.h khexcoltextexport.h ktextcoltextexport.h \
                 kadds.h khe.h krange.h ksection.h \
                 kcolumnsview.h khexedit.h kbytesedit.h \
                 kplainbuffer.h kfixedsizebuffer.h kbigbuffer.h \
                 kdatabuffer.h kreadonlybuffer.h kwrappingrobuffer.h

METASOURCES = AUTO
