/***************************************************************************
                          helper.h  -  description
                             -------------------
    begin                : Do Nov 25 2004
    copyright            : (C) 2004 by Friedrich W. H. Kossebau
    email                : Friedrich.W.H@Kossebau.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU Library General Public           *
 *   License version 2 as published by the Free Software Foundation.       *
 *                                                                         *
 ***************************************************************************/

#ifndef KHE_KHECHAR_H
#define KHE_KHECHAR_H

// qt specific
#include <qstring.h>

namespace KHE
{

class KHEChar : public QChar
{
  public:
    KHEChar( QChar C );
    KHEChar( QChar C, bool U );
  public:
    bool isUndefined() const;
  protected:
    // the byte is not defined
    bool IsUndefined:1;
};

inline KHEChar::KHEChar( QChar C ) : QChar( C ), IsUndefined( false ) {}
inline KHEChar::KHEChar( QChar C, bool U ) : QChar( C ), IsUndefined( U ) {}
inline bool KHEChar::isUndefined() const { return IsUndefined; }

}

#endif
