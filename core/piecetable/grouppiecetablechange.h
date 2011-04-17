/*
    This file is part of the Okteta Core library, made within the KDE community.

    Copyright 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) version 3, or any
    later version accepted by the membership of KDE e.V. (or its
    successor approved by the membership of KDE e.V.), which shall
    act as a proxy defined in Section 6 of version 3 of the license.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef KPIECETABLE_GROUPPIECETABLECHANGE_H
#define KPIECETABLE_GROUPPIECETABLECHANGE_H

// lib
#include "abstractpiecetablechange.h"
// Qt
#include <QtCore/QStack>
#include <QtCore/QString>


namespace Okteta {
class AddressRangeList;
class ArrayChangeMetricsList;
}

namespace KPieceTable
{

  typedef Okteta::AddressRangeList AddressRangeList;
  typedef Okteta::ArrayChangeMetricsList ArrayChangeMetricsList;


/** class
  *@author Friedrich W. H. Kossebau
  */
class GroupPieceTableChange : public AbstractPieceTableChange
{
  public:
    GroupPieceTableChange( GroupPieceTableChange* parent, const QString& description );

    virtual ~GroupPieceTableChange();

  public: // AbstractPieceTableChange API
    virtual int type() const;
    virtual QString description() const;
    virtual bool merge( const AbstractPieceTableChange* other );
    virtual AddressRange apply( PieceTable* pieceTable ) const;
    virtual AddressRange revert( PieceTable* pieceTable ) const;
    virtual ArrayChangeMetrics metrics() const;
    virtual Size dataSize() const;

  public:
    void setDescription( const QString& description );

// TODO: think about a function to compress a group, that is not going to be handled in detail anymore
// e.g. several replaces of the same byte or an insert and replace of the inserted data


  public:
    AddressRangeList applyGroup( PieceTable* pieceTable ) const;
    AddressRangeList revertGroup( PieceTable* pieceTable ) const;
    ArrayChangeMetricsList groupMetrics( bool reverted = false ) const;
    GroupPieceTableChange* parent() const;

  public: // TODO: this interface part is shared with PieceTableChangeHistory, try to use this fact
    bool appendChange( AbstractPieceTableChange *change );
    void finishChange();
  public:
    /// @return number of changes in the history
    int count() const;
    /// @return number of changes currently applied
    int appliedChangesCount() const;
    /// @return description of the change with the id changeId
    QString changeDescription( int changeId ) const;
    /// @return description of the change at the head, empty if there is none
    QString headChangeDescription() const;
    /// @return true if the current change is the base
//     bool isAtBase() const;
    /// @return size of the data used by the applied changes
    int appliedChangesDataSize() const;

  protected:
    QStack<AbstractPieceTableChange*> mChangeStack;
    GroupPieceTableChange* mParent;

    QString mDescription;
    ///
    int mAppliedChangesCount;
    ///
    Size mAppliedChangesDataSize;
    /// if true, try to merge changes
    bool mTryToMergeAppendedChange;
};


inline GroupPieceTableChange::GroupPieceTableChange( GroupPieceTableChange* parent, const QString& description )
  : mParent( parent ),
    mDescription( description ),
    mAppliedChangesCount( 0 ),
    mAppliedChangesDataSize( 0 ),
    mTryToMergeAppendedChange( true )
{}

inline void GroupPieceTableChange::setDescription( const QString& description ) { mDescription = description; }
inline GroupPieceTableChange* GroupPieceTableChange::parent() const { return mParent; }
inline void GroupPieceTableChange::finishChange() { mTryToMergeAppendedChange = false; }
inline int GroupPieceTableChange::count()                     const { return mChangeStack.count(); }
inline int GroupPieceTableChange::appliedChangesCount()       const { return mAppliedChangesCount; }
inline QString GroupPieceTableChange::headChangeDescription() const { return changeDescription( count()-1 ); }
inline QString GroupPieceTableChange::changeDescription( int changeId ) const
{
    const AbstractPieceTableChange* change = mChangeStack.value( changeId );

    return change ? change->description() : QString();
}

}

#endif
