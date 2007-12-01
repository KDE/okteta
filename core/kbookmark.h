/***************************************************************************
                          kbookmark.h  -  description
                             -------------------
    begin                : Fri Nov 30 2007
    copyright            : 2007 by Friedrich W. H. Kossebau
    email                : kossebau@kde.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU Library General Public           *
 *   License version 2 as published by the Free Software Foundation.       *
 *                                                                         *
 ***************************************************************************/


#ifndef KHE_CORE_KBOOKMARK_H
#define KHE_CORE_KBOOKMARK_H

// lib
#include "khe_export.h"


namespace KHECore
{

// TODO: do we need the invalid status?
class KHECORE_EXPORT KBookmark
{
  private:
    static const int InvalidOffset = -1;

  public:
    KBookmark( int offset ); // krazy:exclude=explicit
    KBookmark();
  public:
    int offset() const;
    bool isValid() const;
  public:
    void move( int offset );
  protected:
    int mOffset;
};

inline KBookmark::KBookmark( int offset ) : mOffset( offset ) {}
inline KBookmark::KBookmark() : mOffset( InvalidOffset ) {}
inline bool KBookmark::isValid() const { return mOffset != InvalidOffset; }
inline int KBookmark::offset() const { return mOffset; }
inline void KBookmark::move( int offset ) { mOffset += offset; }

}

#endif
