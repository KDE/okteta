/*
    This file is part of the Kakao Framework, part of the KDE project.

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

#include "processdocumenttablemodel.h"

// lib
#include "processdocumentmanager.h"
#include "abstractprocessdocument.h"
// KDE
#include <KLocale>
#include <KIcon>

ProcressDocumentTableModel::ProcressDocumentTableModel( QObject* parent )
 : QAbstractTableModel( parent )
{
    ProcessDocumentManager* accessableDocumentManager = ProcessDocumentManager::self();

    connect( accessableDocumentManager, SIGNAL(added( AbstractProcessDocument* )),
             SLOT(onListChange()) );
    connect( accessableDocumentManager, SIGNAL(closing( AbstractProcessDocument* )),
             SLOT(onListChange()) );
}

AbstractProcessDocument* ProcressDocumentTableModel::document( int index ) const
{
    return ProcessDocumentManager::self()->processDocumentList().at( index );
}

int ProcressDocumentTableModel::rowCount( const QModelIndex& parent ) const
{
Q_UNUSED( parent )
    return ProcessDocumentManager::self()->processDocumentList().size();
}

int ProcressDocumentTableModel::columnCount( const QModelIndex& parent ) const
{
Q_UNUSED( parent )
    return NoOfColumnIds;
}

QVariant ProcressDocumentTableModel::data( const QModelIndex& index, int role ) const
{
    QVariant result;
    ProcessDocumentManager* accessableDocumentManager = ProcessDocumentManager::self();

    if( role == Qt::DisplayRole )
    {
        const int documentIndex = index.row();
        AbstractProcessDocument* processDocument = accessableDocumentManager->processDocumentList().at( documentIndex );

        const int tableColumn = index.column();
        switch( tableColumn )
        {
            case IdColumnId:
            {
                result = processDocument->id();
                break;
            }
            case TitleColumnId:
                result = processDocument->title();
                break;
            default:
                ;
        }
    }
    else if( role == Qt::DecorationRole )
    {
        const int tableColumn = index.column();
        if( tableColumn == TitleColumnId )
        {
            result = KIcon( "document" );
        }
    }

    return result;
}

QVariant ProcressDocumentTableModel::headerData( int section, Qt::Orientation orientation, int role ) const
{
    QVariant result;

    if( role == Qt::DisplayRole )
    {
        const QString titel =
            section == IdColumnId ?    i18nc("@title:column Id of the version",         "Id") :
            section == TitleColumnId ? i18nc("@title:column description of the change", "Title") :
            QString();
        result = titel;
    }
    else if( role == Qt::ToolTipRole )
    {
        const QString titel =
            section == IdColumnId ?    i18nc("@info:tooltip","Id of the document") :
            section == TitleColumnId ? i18nc("@info:tooltip","Title of the document") :
            QString();
        result = titel;
    }
    else
        result = QAbstractTableModel::headerData( section, orientation, role );

    return result;
}

void ProcressDocumentTableModel::onListChange()
{
    reset();
}


ProcressDocumentTableModel::~ProcressDocumentTableModel() {}
