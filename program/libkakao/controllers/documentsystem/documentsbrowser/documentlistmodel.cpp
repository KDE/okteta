/*
    This file is part of the Kakao Framework, part of the KDE project.

    Copyright 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "documentlistmodel.h"

// lib
#include "documentstool.h"
// Kakao core
#include <kabstractdocument.h>
// KDE
#include <KLocale>
#include <KIcon>


DocumentListModel::DocumentListModel( DocumentsTool* documentsTool, QObject* parent )
 : QAbstractTableModel( parent ),
   mDocumentsTool( documentsTool )
{
    connect( mDocumentsTool, SIGNAL(documentAdded( KAbstractDocument* )), SLOT(onDocumentAdded( KAbstractDocument* )) );
    connect( mDocumentsTool, SIGNAL(documentClosing( KAbstractDocument* )), SLOT(onDocumentClosing( KAbstractDocument* )) );
    connect( mDocumentsTool, SIGNAL(focussedDocumentChanged( KAbstractDocument* )), SLOT(onFocussedDocumentChanged( KAbstractDocument* )) );
}

int DocumentListModel::rowCount( const QModelIndex& parent ) const
{
Q_UNUSED( parent )
    return mDocumentsTool->documents().size();
}

int DocumentListModel::columnCount( const QModelIndex& parent ) const
{
Q_UNUSED( parent )
    return NoOfColumnIds;
}

QVariant DocumentListModel::data( const QModelIndex& index, int role ) const
{
    QVariant result;

    if( role == Qt::DisplayRole )
    {
        const int documentIndex = index.row();
        const KAbstractDocument* document = mDocumentsTool->documents().at( documentIndex );

        const int tableColumn = index.column();
        switch( tableColumn )
        {
            case TitleColumnId:
                result = document->title();
                break;
            default:
                ;
        }
    }
    else if( role == Qt::DecorationRole )
    {
        const int documentIndex = index.row();
        const KAbstractDocument* document = mDocumentsTool->documents().at( documentIndex );

        const int tableColumn = index.column();
        switch( tableColumn )
        {
            case CurrentColumnId:
                if( document == mDocumentsTool->focussedDocument() )
                    result = KIcon( "arrow-right" );
                break;
            case ModifiedColumnId:
                if( document->synchronizationStates() != KAbstractDocument::InSync )
                    result = KIcon( "document-save" );
                break;
            default:
                ;
        }
    }

    return result;
}

QVariant DocumentListModel::headerData( int section, Qt::Orientation orientation, int role ) const
{
    QVariant result;

    if( role == Qt::DisplayRole )
    {
        const QString titel =
//             section == ModifiedColumnId ?  i18nc("@title:column Id of the version",         "Id") :
            section == TitleColumnId ?     i18nc("@title:column description of the change", "Title") :
            QString();
        result = titel;
    }
    else if( role == Qt::ToolTipRole )
    {
        const QString titel =
//             section == ModifiedColumnId ?                i18nc("@info:tooltip","Id of the version") :
            section == TitleColumnId ? i18nc("@info:tooltip","Title of the document") :
            QString();
        result = titel;
    }
    else
        result = QAbstractTableModel::headerData( section, orientation, role );

    return result;
}

void DocumentListModel::onFocussedDocumentChanged( KAbstractDocument* document )
{
Q_UNUSED( document )

    reset();
// TODO: store current focused document, then only emit for changed
#if 0
    const int oldVersionIndex = mVersionIndex;
    mVersionIndex = versionIndex;

    emit dataChanged( index(versionIndex,CurrentColumnId), index(versionIndex,CurrentColumnId) );
    emit dataChanged( index(oldVersionIndex,CurrentColumnId), index(oldVersionIndex,CurrentColumnId) );
#endif
}

void DocumentListModel::onDocumentAdded( KAbstractDocument* document )
{
    connect( document, SIGNAL(modified( KAbstractDocument::SynchronizationStates )),
             SLOT(onModifiedChanged()) );
    // TODO: try to understand how this whould be done with {begin,end}{Insert,Remove}Columns
    reset();
}

void DocumentListModel::onDocumentClosing( KAbstractDocument* document )
{
Q_UNUSED( document )
    // TODO: try to understand how this whould be done with {begin,end}{Insert,Remove}Columns
    reset();
}

void DocumentListModel::onModifiedChanged()
{
    // TODO: try to understand how this whould be done with {begin,end}{Insert,Remove}Columns
    reset();
}


DocumentListModel::~DocumentListModel() {}
