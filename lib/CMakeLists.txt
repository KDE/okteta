kde4_header()

add_subdirectory( controller ) 
add_subdirectory( codecs ) 

include_directories( ${KDE4_INCLUDE_DIR} ${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}  )


########### next target ###############
set(libcontroler_SRCS 
	${CMAKE_SOURCE_DIR}/khexedit/lib/controller/kchareditor.cpp
	${CMAKE_SOURCE_DIR}/khexedit/lib/controller/kcontroller.cpp
	${CMAKE_SOURCE_DIR}/khexedit/lib/controller/keditor.cpp
	${CMAKE_SOURCE_DIR}/khexedit/lib/controller/knavigator.cpp
	${CMAKE_SOURCE_DIR}/khexedit/lib/controller/ktabcontroller.cpp
	${CMAKE_SOURCE_DIR}/khexedit/lib/controller/kvalueeditor.cpp
)

set(libcodecs_SRCS
    ${CMAKE_SOURCE_DIR}/khexedit/lib/codecs/kbinarybytecodec.cpp
    ${CMAKE_SOURCE_DIR}/khexedit/lib/codecs/kbytecodec.cpp
    ${CMAKE_SOURCE_DIR}/khexedit/lib/codecs/kcharcodec.cpp
    ${CMAKE_SOURCE_DIR}/khexedit/lib/codecs/kdecimalbytecodec.cpp
    ${CMAKE_SOURCE_DIR}/khexedit/lib/codecs/kebcdic1047charcodec.cpp
    ${CMAKE_SOURCE_DIR}/khexedit/lib/codecs/khexadecimalbytecodec.cpp
    ${CMAKE_SOURCE_DIR}/khexedit/lib/codecs/koctalbytecodec.cpp
    ${CMAKE_SOURCE_DIR}/khexedit/lib/codecs/ktextcharcodec.cpp
)

set(khexeditcommon_LIB_SRCS ${libcontroler_SRCS} ${libcodecs_SRCS}
   kcolumn.cpp 
   kbordercolumn.cpp 
   koffsetcolumn.cpp 
   kbuffercolumn.cpp 
   kvaluecolumn.cpp 
   kcharcolumn.cpp 
   kcolumnsview.cpp 
   khexedit.cpp 
   kbytesedit.cpp 
   koffsetformat.cpp 
   kdatabuffer.cpp 
   kwrappingrobuffer.cpp 
   kplainbuffer.cpp 
   kfixedsizebuffer.cpp 
   kbigbuffer.cpp 
   kbuffercursor.cpp 
   kbufferlayout.cpp 
   kbufferranges.cpp 
   kcursor.cpp 
   kbufferdrag.cpp 
   kwordbufferservice.cpp 
   ksectionlist.cpp 
   kcoordrangelist.cpp 
   kbordercoltextexport.cpp 
   koffsetcoltextexport.cpp 
   kbuffercoltextexport.cpp 
   kvaluecoltextexport.cpp 
   kcharcoltextexport.cpp )

kde4_automoc(${khexeditcommon_LIB_SRCS})

kde4_add_library(khexeditcommon SHARED ${khexeditcommon_LIB_SRCS})

target_link_libraries(khexeditcommon  ${KDE4_KDE3SUPPORT_LIBS}  )

set_target_properties(khexeditcommon PROPERTIES VERSION 1.0.0 SOVERSION 1 )
install_targets(${LIB_INSTALL_DIR} khexeditcommon )


########### install files ###############


kde4_footer()



#original Makefile.am contents follow:

#SUBDIRS = controller codecs
#
#INCLUDES    = $(all_includes)
#
#METASOURCES = AUTO
#
## this library is used by the kbytesedit part and the khepart part
#lib_LTLIBRARIES =  libkhexeditcommon.la
#libkhexeditcommon_la_LDFLAGS = $(all_libraries) -no-undefined
#libkhexeditcommon_la_LIBADD =  controller/libkcontroller.la codecs/libkhecodecs.la $(LIB_KDECORE)
#libkhexeditcommon_la_SOURCES = kcolumn.cpp kbordercolumn.cpp koffsetcolumn.cpp \
#                               kbuffercolumn.cpp kvaluecolumn.cpp kcharcolumn.cpp \
#                               kcolumnsview.cpp khexedit.cpp kbytesedit.cpp \
#                               koffsetformat.cpp \
#                               kdatabuffer.cpp kwrappingrobuffer.cpp \
#                               kplainbuffer.cpp kfixedsizebuffer.cpp kbigbuffer.cpp \
#                               kbuffercursor.cpp kbufferlayout.cpp kbufferranges.cpp \
#                               kcursor.cpp kbufferdrag.cpp \
#                               kwordbufferservice.cpp \
#                               ksectionlist.cpp kcoordrangelist.cpp \
#                               kbordercoltextexport.cpp koffsetcoltextexport.cpp \
#                               kbuffercoltextexport.cpp kvaluecoltextexport.cpp kcharcoltextexport.cpp
#
## no public API for a direct use by now
#noinst_HEADERS = kcolumn.h kbordercolumn.h koffsetcolumn.h \
#                 kbuffercolumn.h kvaluecolumn.h kcharcolumn.h \
#                 kbytecodec.h koffsetformat.h khexedit_export.h \
#                 kbuffercursor.h kbufferlayout.h kbufferranges.h \
#                 kbuffercoord.h kselection.h \
#                 kcursor.h kbufferdrag.h \
#                 kcoordrange.h ksectionlist.h kcoordrangelist.h \
#                 khechar.h kcharcodec.h \
#                 kcoltextexport.h kbordercoltextexport.h koffsetcoltextexport.h \
#                 kbuffercoltextexport.h kvaluecoltextexport.h kcharcoltextexport.h \
#                 kadds.h khe.h krange.h ksection.h \
#                 kwordbufferservice.h \
#                 kcolumnsview.h khexedit.h kbytesedit.h \
#                 kplainbuffer.h kfixedsizebuffer.h kbigbuffer.h \
#                 kdatabuffer.h kreadonlybuffer.h kwrappingrobuffer.h
