/*
    This file is part of the Okteta Kasten module, made within the KDE community.

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

#include "containedstringtablemodel.h"

// KDE
#include <KLocale>


ContainedStringTableModel::ContainedStringTableModel( const QList<ContainedString> *containedStringList, QObject *parent )
 : QAbstractTableModel( parent ),
   mContainedStringList( containedStringList )
{
    mPrintFunction = Okteta::OffsetFormat::printFunction( Okteta::OffsetFormat::Hexadecimal );
}

void ContainedStringTableModel::update()
{
    reset();
}


int ContainedStringTableModel::rowCount( const QModelIndex &parent ) const
{
    return (! parent.isValid()) ? mContainedStringList->size() : 0;
}

int ContainedStringTableModel::columnCount( const QModelIndex &parent ) const
{
    return (! parent.isValid()) ? NoOfColumnIds : 0;
}

QVariant ContainedStringTableModel::data( const QModelIndex &index, int role ) const
{
    QVariant result;
    if( role == Qt::DisplayRole
        || role == Qt::ToolTipRole )
    {
        const int stringIndex = index.row();

        if( 0 <= stringIndex && stringIndex < mContainedStringList->size() )
        {
            const ContainedString &string = mContainedStringList->at( stringIndex );

            const int column = index.column();
            switch( column )
            {
                case OffsetColumnId:
                {
                    if( role == Qt::DisplayRole )
                    {
                        mPrintFunction( mCodedOffset, string.offset() );

                        result = QString::fromLatin1( mCodedOffset );
                    }
                    break;
                }
                case StringColumnId:
                {
                    result = string.string();
                    break;
                }
                default:
                    ;
            }
        }
    }

    return result;
}

QVariant ContainedStringTableModel::headerData( int section, Qt::Orientation orientation, int role ) const
{
    QVariant result;

    if( role == Qt::DisplayRole )
    {
        const QString titel =
            section == OffsetColumnId ?   i18nc("@title:column offset of the extracted string",       "Offset") :
            section == StringColumnId ?   i18nc("@title:column string extracted from the byte array", "String") :
            QString();
        result = titel;
    }
    else
        result = QAbstractTableModel::headerData( section, orientation, role );

    return result;
}

ContainedStringTableModel::~ContainedStringTableModel() {}
