/***************************************************************************
                          helper.h  -  description
                             -------------------
    begin                : Do Nov 25 2004
    copyright            : (C) 2004 by Friedrich W. H. Kossebau
    email                : kossebau@kde.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU Library General Public           *
 *   License version 2 as published by the Free Software Foundation.       *
 *                                                                         *
 ***************************************************************************/


#ifndef KHE_CORE_KHECHAR_H
#define KHE_CORE_KHECHAR_H

// lib
#include "khe_export.h"
// Qt
#include <QtCore/QChar>


namespace KHECore
{

class KHECORE_EXPORT KChar : public QChar
{
  public:
    KChar( QChar C ); // krazy:exclude=explicit
    KChar( QChar C, bool U );
  public:
    bool isUndefined() const;
  protected:
    // the byte is not defined
    bool IsUndefined:1;
};

inline KChar::KChar( QChar C ) : QChar( C ), IsUndefined( false ) {}
inline KChar::KChar( QChar C, bool U ) : QChar( C ), IsUndefined( U ) {}
inline bool KChar::isUndefined() const { return IsUndefined; }

}

#endif
