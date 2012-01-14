/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    Copyright 2010,2012 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#ifndef VIEWPROFILETABLEMODEL_H
#define VIEWPROFILETABLEMODEL_H

// Okteta Gui Kasten
#include <bytearrayviewprofile.h>
// Qt
#include <QtCore/QAbstractTableModel>

template< class C > class QList;


namespace Kasten2
{
class ByteArrayViewProfileManager;


class ViewProfileTableModel : public QAbstractTableModel
{
  Q_OBJECT

  public:
    enum ColumnIds
    {
        CurrentColumnId = 0,
        NameColumnId = 1,
        NoOfColumnIds = 2 // TODO: what pattern is usually used to mark number of ids?
    };

  public:
    explicit ViewProfileTableModel( const ByteArrayViewProfileManager* viewProfileManager,
                                    QObject* parent = 0 );
    virtual ~ViewProfileTableModel();

  public: // QAbstractTableModel API
    virtual int rowCount( const QModelIndex& parent ) const;
    virtual int columnCount( const QModelIndex& parent ) const;
    virtual QVariant data( const QModelIndex& index, int role ) const;

  public:
    ByteArrayViewProfile::Id viewProfileId( const QModelIndex& index ) const;
    int row( const ByteArrayViewProfile::Id& viewProfileId ) const;

  protected Q_SLOTS:
    void onDefaultIndexChanged();
    void onViewProfilesChanged();
    void onViewProfileLocksChanged(const QList<Kasten2::ByteArrayViewProfile::Id>& viewProfileIds );

  protected:
    const ByteArrayViewProfileManager* mViewProfileManager;
};

}

#endif
