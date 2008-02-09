/***************************************************************************
                          versiontablemodel.cpp  -  description
                             -------------------
    begin                : Sat Feb 9 2008
    copyright            : 2008 by Friedrich W. H. Kossebau
    email                : kossebau@kde.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU General Public License           *
 *   version 2 as published by the Free Software Foundation.               *
 *                                                                         *
 ***************************************************************************/


#include "versiontablemodel.h"

// Kakao
#include <kiversionable.h>
#include <kabstractdocument.h>
// KDE
#include <KLocale>
#include <KIcon>

VersionTableModel::VersionTableModel( KAbstractDocument *document, KDE::If::Versionable *versionControl, QObject *parent )
 : QAbstractTableModel( parent ),
   mDocument( document ),
   mVersionControl( versionControl ),
   mVersionIndex( versionControl ? versionControl->versionIndex() : 0 )
{
    if( mDocument )
    {
        connect( mDocument, SIGNAL(revertedToVersionIndex( int )), SLOT(onRevertedToVersionIndex( int )) );
        connect( mDocument, SIGNAL(headVersionChanged( int )), SLOT(onHeadVersionChanged( int )) );
        connect( mDocument, SIGNAL(headVersionDataChanged( const KDocumentVersionData & )),
                 SLOT(onHeadVersionDataChanged( const KDocumentVersionData & )) );
    }
}

void VersionTableModel::setDocument( KAbstractDocument *document, KDE::If::Versionable *versionControl )
{
    if( mDocument ) mDocument->disconnect( this );

    mDocument = document;
    mVersionControl = versionControl;

    if( mDocument )
    {
        connect( mDocument, SIGNAL(revertedToVersionIndex( int )), SLOT(onRevertedToVersionIndex( int )) );
        connect( mDocument, SIGNAL(headVersionChanged( int )), SLOT(onHeadVersionChanged( int )) );
        connect( mDocument, SIGNAL(headVersionDataChanged( const KDocumentVersionData & )),
                 SLOT(onHeadVersionDataChanged( const KDocumentVersionData & )) );
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
            section == IdColumnId ?                i18nc("@title Id of the version",         "Id") :
            section == ChangeDescriptionColumnId ? i18nc("@title description of the change", "Changes") :
            QString();
        result = titel;
    }
    else if( role == Qt::ToolTipRole )
    {
        const QString titel =
            section == IdColumnId ?                i18n("Id of the version") :
            section == ChangeDescriptionColumnId ? i18n("Description of what changed") :
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
