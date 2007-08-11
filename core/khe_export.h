/*  This file is part of the KDE project
    Copyright (C) 2007 David Faure <faure@kde.org>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.
*/

#ifndef KHE_EXPORT_H
#define KHE_EXPORT_H

/* needed for KDE_EXPORT and KDE_IMPORT macros */
#include <kdemacros.h>

#ifndef KHECORE_EXPORT
# if defined(MAKE_OKTETACORE_LIB)
   /* We are building this library */ 
#  define KHECORE_EXPORT KDE_EXPORT
# else
   /* We are using this library */ 
#  define KHECORE_EXPORT KDE_IMPORT
# endif
#endif

# ifndef KHECORE_EXPORT_DEPRECATED
#  define KHECORE_EXPORT_DEPRECATED KDE_DEPRECATED KHECORE_EXPORT
# endif

#ifndef KHEUI_EXPORT
# if defined(MAKE_OKTETAGUI_LIB)
   /* We are building this library */
#  define KHEUI_EXPORT KDE_EXPORT
# else
   /* We are using this library */
#  define KHEUI_EXPORT KDE_IMPORT
# endif
#endif

# ifndef KHEUI_EXPORT_DEPRECATED
#  define KHEUI_EXPORT_DEPRECATED KDE_DEPRECATED KHEUI_EXPORT
# endif


#endif
