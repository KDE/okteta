# - Try to find QCA2 (Qt Cryptography Architecture 2)
# Once done this will define
#
#  QCA2_FOUND - system has QCA2
#  QCA2_INCLUDE_DIR - the QCA2 include directory
#  QCA2_LIBRARIES - the libraries needed to use QCA2
#  QCA2_DEFINITIONS - Compiler switches required for using QCA2
#
# use pkg-config to get the directories and then use these values
# in the FIND_PATH() and FIND_LIBRARY() calls

# Copyright (c) 2006, Michael Larouche, <michael.larouche@kdemail.net>
# Copyright (c) 2014, Friedrich W. H. Kossebau, <kossebau@kde.org>
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.

if (NOT WIN32)
  find_package(PkgConfig QUIET)
  pkg_check_modules(PC_QCA2 QUIET qca2)
  set(QCA2_DEFINITIONS ${PC_QCA2_CFLAGS_OTHER})
endif (NOT WIN32)

find_library(QCA2_LIBRARIES
             WIN32_DEBUG_POSTFIX d
             NAMES qca
             HINTS ${PC_QCA2_LIBDIR} ${PC_QCA2_LIBRARY_DIRS}
)

find_path(QCA2_INCLUDE_DIR QtCrypto
          HINTS ${PC_QCA2_INCLUDEDIR} ${PC_QCA2_INCLUDE_DIRS}
          PATH_SUFFIXES QtCrypto
)

find_file(QCA2_VERSION_FILE
          QtCrypto/qca_version.h
          HINTS ${QCA2_INCLUDE_DIR}
)

if(QCA2_VERSION_FILE)
  file(READ ${QCA2_VERSION_FILE} QCA2_VERSION_CONTENT)
  string (REGEX MATCH "QCA_VERSION_STR \".*\"\n" QCA2_VERSION_MATCH "${QCA2_VERSION_CONTENT}")

  if(QCA2_VERSION_MATCH)
    string(REGEX REPLACE "QCA_VERSION_STR \"(.*)\"\n" "\\1" QCA2_VERSION ${QCA2_VERSION_MATCH})
  endif(QCA2_VERSION_MATCH)
endif(QCA2_VERSION_FILE)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(QCA2  REQUIRED_VARS QCA2_LIBRARIES QCA2_INCLUDE_DIR
                                        VERSION_VAR  QCA2_VERSION)

mark_as_advanced(QCA2_INCLUDE_DIR QCA2_LIBRARIES)
