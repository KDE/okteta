SUBDIRS = controller codecs

INCLUDES    = $(all_includes)

METASOURCES = AUTO

# this library is used by the kbytesedit part and the khepart part
lib_LTLIBRARIES =  libkhexeditcommon.la
libkhexeditcommon_la_LDFLAGS = $(all_libraries) -no-undefined
libkhexeditcommon_la_LIBADD =  controller/libkcontroller.la codecs/libkhecodecs.la $(LIB_KDECORE)
libkhexeditcommon_la_SOURCES = kcolumn.cpp kbordercolumn.cpp koffsetcolumn.cpp \
                               kbuffercolumn.cpp kvaluecolumn.cpp kcharcolumn.cpp \
                               kcolumnsview.cpp khexedit.cpp kbytesedit.cpp \
                               koffsetformat.cpp \
                               kdatabuffer.cpp kwrappingrobuffer.cpp \
                               kplainbuffer.cpp kfixedsizebuffer.cpp kbigbuffer.cpp \
                               kbuffercursor.cpp kbufferlayout.cpp kbufferranges.cpp \
                               kcursor.cpp kbufferdrag.cpp \
                               kwordbufferservice.cpp \
                               ksectionlist.cpp kcoordrangelist.cpp \
                               kbordercoltextexport.cpp koffsetcoltextexport.cpp \
                               kbuffercoltextexport.cpp kvaluecoltextexport.cpp kcharcoltextexport.cpp

# no public API for a direct use by now
noinst_HEADERS = kcolumn.h kbordercolumn.h koffsetcolumn.h \
                 kbuffercolumn.h kvaluecolumn.h kcharcolumn.h \
                 kbytecodec.h koffsetformat.h khexedit_export.h \
                 kbuffercursor.h kbufferlayout.h kbufferranges.h \
                 kbuffercoord.h kselection.h \
                 kcursor.h kbufferdrag.h \
                 kcoordrange.h ksectionlist.h kcoordrangelist.h \
                 khechar.h kcharcodec.h \
                 kcoltextexport.h kbordercoltextexport.h koffsetcoltextexport.h \
                 kbuffercoltextexport.h kvaluecoltextexport.h kcharcoltextexport.h \
                 kadds.h khe.h krange.h ksection.h \
                 kwordbufferservice.h \
                 kcolumnsview.h khexedit.h kbytesedit.h \
                 kplainbuffer.h kfixedsizebuffer.h kbigbuffer.h \
                 kdatabuffer.h kreadonlybuffer.h kwrappingrobuffer.h
