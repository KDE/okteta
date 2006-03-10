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
#noinst_LTLIBRARIES =  libkcontroller.la
#libkcontroller_la_SOURCES = kcontroller.cpp ktabcontroller.cpp knavigator.cpp \
#                            keditor.cpp kvalueeditor.cpp kchareditor.cpp
#
## no public API
#noinst_HEADERS = kcontroller.h ktabcontroller.h knavigator.h \
#                 keditor.h kvalueeditor.h kchareditor.h
