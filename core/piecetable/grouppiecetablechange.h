/*
    This file is part of the Okteta Core library, part of the KDE project.

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
#include <QtCore/QList>
#include <QtCore/QString>

namespace KHE
{
class KSectionList;
class ArrayChangeMetricsList;
}

namespace KPieceTable
{

/** class
  *@author Friedrich W. H. Kossebau
  */

class GroupPieceTableChange : public AbstractPieceTableChange
{
  public:
    GroupPieceTableChange( const QString &description );
    virtual ~GroupPieceTableChange();

  public: // AbstractPieceTableChange API
    virtual int type() const;
    virtual QString description() const;
    virtual bool merge( const AbstractPieceTableChange *other );
//     virtual KHE::KSection apply( PieceTable *pieceTable ) const;
//     virtual KHE::KSection revert( PieceTable *pieceTable ) const;
//     virtual KHE::ArrayChangeMetrics metrics() const;
    virtual int dataSize() const;

  public:
    void setDescription( const QString &description );

    KHE::KSectionList applyGroup( PieceTable *pieceTable ) const;
    KHE::KSectionList revertGroup( PieceTable *pieceTable ) const;
    KHE::ArrayChangeMetricsList groupMetrics() const;

  protected:
    QList<AbstractPieceTableChange*> mChangeList;
    QString mDescription;
};

inline GroupPieceTableChange::GroupPieceTableChange( const QString &description )
 : mDescription( description )
{}

inline void GroupPieceTableChange::setDescription( const QString &description ) { mDescription = description; }

}

#endif
