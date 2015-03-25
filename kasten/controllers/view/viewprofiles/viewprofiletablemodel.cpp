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

#include "viewprofiletablemodel.h"

// Okteta Gui Kasten
#include <kasten/okteta/bytearrayviewprofilemanager.h>
// KF5
#include <KColorScheme>
#include <KLocalizedString>
// Qt
#include <QApplication>
#include <QVector>
#include <QIcon>


namespace Kasten
{

ViewProfileTableModel::ViewProfileTableModel( const ByteArrayViewProfileManager* viewProfileManager,
                                              QObject* parent )
  : QAbstractTableModel( parent )
  , mViewProfileManager( viewProfileManager )
{
    connect( viewProfileManager, &ByteArrayViewProfileManager::viewProfilesChanged,
             this, &ViewProfileTableModel::onViewProfilesChanged );
    connect( viewProfileManager, &ByteArrayViewProfileManager::viewProfilesRemoved,
             this, &ViewProfileTableModel::onViewProfilesChanged );
    connect( viewProfileManager, &ByteArrayViewProfileManager::defaultViewProfileChanged,
             this, &ViewProfileTableModel::onDefaultIndexChanged );
    connect( viewProfileManager, &ByteArrayViewProfileManager::viewProfilesLocked,
             this, &ViewProfileTableModel::onViewProfilesChanged );
    connect( viewProfileManager, &ByteArrayViewProfileManager::viewProfilesUnlocked,
             this, &ViewProfileTableModel::onViewProfileLocksChanged );
}

int ViewProfileTableModel::rowCount( const QModelIndex &parent ) const
{
    return (! parent.isValid() ) ? mViewProfileManager->viewProfilesCount() : 0;
}

int ViewProfileTableModel::columnCount( const QModelIndex &parent ) const
{
    return (! parent.isValid()) ? NoOfColumnIds : 0;
}

QVariant ViewProfileTableModel::data( const QModelIndex &index, int role ) const
{
    QVariant result;
    switch( role )
    {
    case Qt::DisplayRole:
    {
        const int viewProfileIndex = index.row();

        const int tableColumn = index.column();
        switch( tableColumn )
        {
            case NameColumnId:
            {
                result = mViewProfileManager->viewProfiles().at(viewProfileIndex).viewProfileTitle();
                break;
            }
            default:
                ;
        }
        break;
    }
    case Qt::DecorationRole:
    {
        const int tableColumn = index.column();
        if( tableColumn == CurrentColumnId )
        {
            const int viewProfileIndex = index.row();
            const ByteArrayViewProfile::Id viewProfileId =
                mViewProfileManager->viewProfiles().at(viewProfileIndex).id();

            if( mViewProfileManager->defaultViewProfileId() == viewProfileId )
                result = QIcon::fromTheme( QStringLiteral("arrow-right") );
        }
        break;
    }
    case Qt::ForegroundRole:
    {
        const int viewProfileIndex = index.row();
        const ByteArrayViewProfile::Id viewProfileId =
            mViewProfileManager->viewProfiles().at(viewProfileIndex).id();

        if( mViewProfileManager->isViewProfileLocked(viewProfileId) )
        {
            const QPalette& palette = QApplication::palette();
            const KColorScheme colorScheme( palette.currentColorGroup(), KColorScheme::View );
            result = colorScheme.foreground( KColorScheme::InactiveText );
        }
        break;
    }
    }

    return result;
}

ByteArrayViewProfile::Id
ViewProfileTableModel::viewProfileId( const QModelIndex& index ) const
{
    const int viewProfileIndex = index.row();
    const bool isValidIndex =
        (0 <= viewProfileIndex) && (viewProfileIndex < mViewProfileManager->viewProfilesCount());

    return isValidIndex ? mViewProfileManager->viewProfiles().at(viewProfileIndex).id() : ByteArrayViewProfile::Id();
}

int
ViewProfileTableModel::row( const ByteArrayViewProfile::Id& viewProfileId ) const
{
    int result = -1;

    const QList<ByteArrayViewProfile> viewProfiles = mViewProfileManager->viewProfiles();
    const int viewProfilesCount = viewProfiles.count();
    for( int i = 0; i< viewProfilesCount; ++i )
    {
        if( viewProfileId == viewProfiles.at( i ).id() )
        {
            result = i;
            break;
        }
    }

    return result;
}


void ViewProfileTableModel::onDefaultIndexChanged()
{
    // simply reset the whole column, does not happen often and not worth to cache the old default
    emit dataChanged( index(CurrentColumnId, 0),
                      index(CurrentColumnId, mViewProfileManager->viewProfiles().count()-1) );
}

void ViewProfileTableModel::onViewProfilesChanged()
{
    beginResetModel();
    endResetModel();
}

void ViewProfileTableModel::onViewProfileLocksChanged(const QList<ByteArrayViewProfile::Id>& viewProfileIds )
{
    const QList<ByteArrayViewProfile> viewProfiles = mViewProfileManager->viewProfiles();
    const int viewProfilesCount = viewProfiles.count();
    for( int i = 0; i< viewProfilesCount; ++i )
    {
        const ByteArrayViewProfile::Id viewProfileId = viewProfiles.at( i ).id();

        if( viewProfileIds.contains(viewProfileId) )
            emit dataChanged( index(CurrentColumnId, i),
                              index(NameColumnId,    i) );
    }
}

ViewProfileTableModel::~ViewProfileTableModel() {}

}
