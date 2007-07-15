/***************************************************************************
                          khe_export.h  -  description
                             -------------------
    begin                : 22 Nov 2004
    copyright            : (C) 2004 Dirk Mueller
    email                : mueller@kde.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU Library General Public           *
 *   License version 2 as published by the Free Software Foundation.       *
 *                                                                         *
 ***************************************************************************/


#ifndef KHE_EXPORT_H
#define KHE_EXPORT_H

#include <kdemacros.h>

#ifdef MAKE_OKTETACORE_LIB
# define KHECORE_EXPORT KDE_EXPORT
#else
# define KHECORE_EXPORT KDE_IMPORT
#endif

#ifdef MAKE_OKTETAGUI_LIB
# define KHEUI_EXPORT KDE_EXPORT
#else
# define KHEUI_EXPORT KDE_IMPORT
#endif

#endif
