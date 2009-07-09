/*
    This file is part of the Kasten Framework, part of the KDE project.

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

#include "versiontablemodel.h"

// Kasten core
#include <kiversionable.h>
#include <abstractmodel.h>
// KDE
#include <KLocale>
#include <KIcon>


namespace Kasten
{

VersionTableModel::VersionTableModel( AbstractModel* model, If::Versionable* versionControl, QObject* parent )
 : QAbstractTableModel( parent ),
   mModel( model ),
   mVersionControl( versionControl ),
   mVersionIndex( versionControl ? versionControl->versionIndex() : 0 )
{
    if( mModel )
    {
        connect( mModel, SIGNAL(revertedToVersionIndex( int )), SLOT(onRevertedToVersionIndex( int )) );
        connect( mModel, SIGNAL(headVersionChanged( int )), SLOT(onHeadVersionChanged( int )) );
        connect( mModel, SIGNAL(headVersionDataChanged( const KDocumentVersionData & )),
                 SLOT(onHeadVersionDataChanged( const KDocumentVersionData & )) );
    }
}

void VersionTableModel::setModel( AbstractModel* model, If::Versionable* versionControl )
{
    if( mModel ) mModel->disconnect( this );

    mModel = model;
    mVersionControl = versionControl;

    if( mModel )
    {
        connect( mModel, SIGNAL(revertedToVersionIndex( int )), SLOT(onRevertedToVersionIndex( int )) );
        connect( mModel, SIGNAL(headVersionChanged( int )), SLOT(onHeadVersionChanged( int )) );
        connect( mModel, SIGNAL(headVersionDataChanged( const Kasten::KDocumentVersionData & )),
                 SLOT(onHeadVersionDataChanged( const Kasten::KDocumentVersionData & )) );
    }
    mVersionIndex = versionControl ? versionControl->versionIndex() : 0;

    reset();
}

int VersionTableModel::rowCount( const QModelIndex &parent ) const
{
Q_UNUSED( parent )
    return mVersionControl ? mVersionControl->versionCount() : 0;;
}

int VersionTableModel::columnCount( const QModelIndex &parent ) const
{
Q_UNUSED( parent )
    return NoOfColumnIds;
}

QVariant VersionTableModel::data( const QModelIndex &index, int role ) const
{
    QVariant result;
    if( role == Qt::DisplayRole )
    {
        const int versionIndex = index.row();
        const KDocumentVersionData version = mVersionControl->versionData( versionIndex );

        const int tableColumn = index.column();
        switch( tableColumn )
        {
            case IdColumnId:
            {
                result = version.id();
                break;
            }
            case ChangeDescriptionColumnId:
                result = version.changeComment();
                break;
            default:
                ;
        }
    }
    else if( role == Qt::DecorationRole )
    {
        const int tableColumn = index.column();
        if( tableColumn == CurrentColumnId )
        {
            const int versionIndex = index.row();
            if( mVersionControl->versionIndex() == versionIndex )
                result = KIcon( "arrow-right" );
        }
    }

    return result;
}

QVariant VersionTableModel::headerData( int section, Qt::Orientation orientation, int role ) const
{
    QVariant result;

    if( role == Qt::DisplayRole )
    {
        const QString titel =
            section == IdColumnId ?                i18nc("@title:column Id of the version",         "Id") :
            section == ChangeDescriptionColumnId ? i18nc("@title:column description of the change", "Changes") :
            QString();
        result = titel;
    }
    else if( role == Qt::ToolTipRole )
    {
        const QString titel =
            section == IdColumnId ?                i18nc("@info:tooltip","Id of the version") :
            section == ChangeDescriptionColumnId ? i18nc("@info:tooltip","Description of what changed") :
            QString();
        result = titel;
    }
    else
        result = QAbstractTableModel::headerData( section, orientation, role );

    return result;
}

void VersionTableModel::onRevertedToVersionIndex( int versionIndex )
{
    if( mVersionIndex == versionIndex )
        return;

    const int oldVersionIndex = mVersionIndex;
    mVersionIndex = versionIndex;

    emit dataChanged( index(versionIndex,CurrentColumnId), index(versionIndex,CurrentColumnId) );
    emit dataChanged( index(oldVersionIndex,CurrentColumnId), index(oldVersionIndex,CurrentColumnId) );
}

void VersionTableModel::onHeadVersionChanged( int newHeadVersionIndex )
{
    mVersionIndex = newHeadVersionIndex;
    // TODO: try to understand how this whould be done with {begin,end}{Insert,Remove}Columns
    reset();
}

void VersionTableModel::onHeadVersionDataChanged( const KDocumentVersionData &versionData )
{
    Q_UNUSED( versionData )
    const int headVersionIndex = mVersionControl->versionCount() - 1;
    emit dataChanged( index(headVersionIndex,CurrentColumnId), index(headVersionIndex,ChangeDescriptionColumnId) );
}

VersionTableModel::~VersionTableModel() {}

}
