kde4_header()

include_directories( ${KDE4_INCLUDE_DIR} ${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}  )


########### next target ###############


########### install files ###############


kde4_footer()



#original Makefile.am contents follow:

#INCLUDES    = -I$(srcdir)/.. $(all_includes)
#
#METASOURCES = AUTO
#
## 
#noinst_LTLIBRARIES =  libkhecodecs.la
#libkhecodecs_la_SOURCES = kcharcodec.cpp kbytecodec.cpp \
#                          ktextcharcodec.cpp kebcdic1047charcodec.cpp \
#                          kbinarybytecodec.cpp koctalbytecodec.cpp \
#                          kdecimalbytecodec.cpp khexadecimalbytecodec.cpp
#
## no public API
#noinst_HEADERS = ktextcharcodec.h kebcdic1047charcodec.h \
#                 kbinarybytecodec.h koctalbytecodec.h \
#                 kdecimalbytecodec.h khexadecimalbytecodec.h