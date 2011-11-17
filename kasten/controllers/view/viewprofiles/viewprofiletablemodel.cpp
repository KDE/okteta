/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    Copyright 2010 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "viewprofiletablemodel.h"

// Okteta Gui Kasten
#include <bytearrayviewprofile.h>
// KDE
#include <KLocale>
#include <KIcon>
// Qt
#include <QtCore/QVector>


namespace Kasten1
{

ViewProfileTableModel::ViewProfileTableModel( const QList<ByteArrayViewProfile>* viewProfileList,
                                              int defaultIndex,
                                              QObject* parent )
 : QAbstractTableModel( parent ),
   mViewProfileList( viewProfileList ),
   mDefaultIndex( defaultIndex )
{
}

int ViewProfileTableModel::rowCount( const QModelIndex &parent ) const
{
    return (! parent.isValid() ) ? mViewProfileList->count() : 0;
}

int ViewProfileTableModel::columnCount( const QModelIndex &parent ) const
{
    return (! parent.isValid()) ? NoOfColumnIds : 0;
}

QVariant ViewProfileTableModel::data( const QModelIndex &index, int role ) const
{
    QVariant result;
    if( role == Qt::DisplayRole )
    {
        const int viewProfileIndex = index.row();

        const int tableColumn = index.column();
        switch( tableColumn )
        {
            case NameColumnId:
            {
                result = mViewProfileList->at(viewProfileIndex).viewProfileTitle();
                break;
            }
            default:
                ;
        }
    }
    else if( role == Qt::DecorationRole )
    {
        const int tableColumn = index.column();
        if( tableColumn == CurrentColumnId )
        {
            const int viewProfileIndex = index.row();
            if( mDefaultIndex == viewProfileIndex )
                result = KIcon( "arrow-right" );
        }
    }

    return result;
}

void ViewProfileTableModel::setDefaultIndex( int defaultIndex )
{
    mDefaultIndex = defaultIndex;

    reset();
}

void ViewProfileTableModel::handleViewProfileAdded( int viewProfileIndex )
{
    Q_UNUSED( viewProfileIndex );
    // TODO: {begin,end}{Insert,Remove}Columns
    reset();
}

void ViewProfileTableModel::handleViewProfileRemoved( int viewProfileIndex )
{
    Q_UNUSED( viewProfileIndex );
    // TODO: {begin,end}{Insert,Remove}Columns
    reset();
}

ViewProfileTableModel::~ViewProfileTableModel() {}

}
