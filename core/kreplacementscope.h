/***************************************************************************
                          kreplacementscope.h  -  description
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


#ifndef KHE_CORE_REPLACEMENTSCOPE_H
#define KHE_CORE_REPLACEMENTSCOPE_H

// lib
#include "khe_export.h"


namespace KHE
{

// TODO: do we need the invalid status?
class KHECORE_EXPORT ReplacementScope
{
  private:
    static const int InvalidOffset = -1;

  public:
    ReplacementScope( int offset, int removeLength, int insertLength );
    ReplacementScope();

  public:
    bool operator==( const ReplacementScope &other ) const;

  public:
    int offset() const;
    int removeLength() const;
    int insertLength() const;
    int lengthChange() const;
    bool isValid() const;

  protected:
    int mOffset;
    int mRemoveLength;
    int mInsertLength;
};

inline ReplacementScope::ReplacementScope( int offset, int removeLength, int insertLength )
 : mOffset( offset ), mRemoveLength( removeLength ), mInsertLength( insertLength )
{}
inline ReplacementScope::ReplacementScope() : mOffset( InvalidOffset ), mRemoveLength( 0 ), mInsertLength( 0 ) {}
inline bool ReplacementScope::operator==( const ReplacementScope &other ) const
{ return mOffset == other.mOffset && mRemoveLength == other.mRemoveLength && mInsertLength == other.mInsertLength; }
inline bool ReplacementScope::isValid() const { return mOffset != InvalidOffset; }
inline int ReplacementScope::offset() const { return mOffset; }
inline int ReplacementScope::removeLength() const { return mRemoveLength; }
inline int ReplacementScope::insertLength() const { return mInsertLength; }
inline int ReplacementScope::lengthChange() const { return mInsertLength-mRemoveLength; }

}

#endif
