/*
    This file is part of the Okteta Kakao module, part of the KDE project.

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

#include "podtablemodel.h"

// lib
#include "poddecodertool.h"
// KDE
#include <KLocale>
#include <KApplication>
#include <KColorScheme>


PODTableModel::PODTableModel( PODDecoderTool* tool, QObject *parent )
 : QAbstractTableModel( parent ),
   mTool( tool ),
   mEmptyNote( '-' )
{
    connect( mTool, SIGNAL(dataChanged()), SLOT(onDataChanged()) );
}

void PODTableModel::onDataChanged()
{
    emit dataChanged( index(0,ValueId), index(mTool->podCount()-1,ValueId) );
}

int PODTableModel::rowCount( const QModelIndex& parent ) const
{
Q_UNUSED( parent )
    return mTool->podCount();
}

int PODTableModel::columnCount( const QModelIndex& parent ) const
{
Q_UNUSED( parent )
    return NoOfColumnIds;
}

QVariant PODTableModel::data( const QModelIndex& index, int role ) const
{
    QVariant result;
    switch( role )
    {
    case Qt::DisplayRole:
    {
        const int podId = index.row();
        const int column = index.column();
        switch( column )
        {
            case NameId:
            {
                result = mTool->nameOfPOD( podId );
                break;
            }
            case ValueId:
            {
                QString valueString = mTool->valueAsString( podId );
                if( valueString.isEmpty() )
                    valueString = mEmptyNote;
                result = valueString;
                break;
            }
            default:
                ;
        }
        break;
    }
    case Qt::EditRole:
    {
        const int podId = index.row();
        const int column = index.column();
        if( column == ValueId )
            result = mTool->valueAsString( podId );
        break;
    }
    case Qt::TextAlignmentRole:
    {
        const int column = index.column();
        result = ( column==NameId ) ? Qt::AlignRight: Qt::AlignLeft;
        break;
    }
    case Qt::ForegroundRole:
    {
        const int podId = index.row();
        const int column = index.column();
        if( column == ValueId )
        {
            QString valueString = mTool->valueAsString( podId );
            if( valueString.isEmpty() )
            {
                const QPalette &palette = KApplication::kApplication()->palette();
                const KColorScheme colorScheme( palette.currentColorGroup(), KColorScheme::View );
                result = colorScheme.foreground( KColorScheme::InactiveText );
            }
        }
    }
    default:
        break;
    }

    return result;
}

Qt::ItemFlags PODTableModel::flags( const QModelIndex& index ) const
{
    Qt::ItemFlags result = QAbstractTableModel::flags( index );
    const int column = index.column();
    if( column == ValueId )
        result |= Qt::ItemIsEditable;

    return result;
}

QModelIndex PODTableModel::buddy( const QModelIndex& index ) const
{
    QModelIndex result;

    const int column = index.column();
    if( column == NameId )
    {
        const int row = index.row();
        result = createIndex( row, ValueId );
    }
    else
        result = index;

    return result;
}


QVariant PODTableModel::headerData( int section, Qt::Orientation orientation, int role ) const
{
    QVariant result;

    if( role == Qt::DisplayRole )
    {
        const QString titel =
            section == NameId ?   i18nc("@title:column name of the datatype",                "Type") :
            section == ValueId ?  i18nc("@title:column value of the bytes for the datatype", "Value") :
            QString();
        result = titel;
    }
    else if( role == Qt::ToolTipRole )
    {
        const QString titel =
            section == NameId ?
                    i18nc("@info:tooltip for column Type",    "The type of data") :
            section == ValueId ?
                    i18nc("@info:tooltip for column Value",   "The value of the bytes for the datatype") :
            QString();
        result = titel;
    }
    else
        result = QAbstractTableModel::headerData( section, orientation, role );

    return result;
}

PODTableModel::~PODTableModel() {}
