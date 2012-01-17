/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    Copyright 2007 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#ifndef BYTETABLEMODEL_H
#define BYTETABLEMODEL_H

// Qt
#include <QtCore/QAbstractTableModel>

namespace Okteta {
class CharCodec;
class ValueCodec;
}


namespace Kasten2
{

class ByteTableModel : public QAbstractTableModel
{
  Q_OBJECT

  public:
    enum ColumnIds
    {
        DecimalId = 0,
        HexadecimalId = 1,
        OctalId = 2,
        BinaryId = 3,
        CharacterId = 4,
        NoOfIds = 5 // TODO: what pattern is usually used to mark number of ids?
    };

  public:
    explicit ByteTableModel( QObject *parent = 0 );
    virtual ~ByteTableModel();

  public: // QAbstractTableModel API
    virtual int rowCount( const QModelIndex &parent ) const;
    virtual int columnCount( const QModelIndex &parent ) const;
    virtual QVariant data( const QModelIndex &index, int role ) const;
    virtual QVariant headerData( int section, Qt::Orientation orientation, int role ) const;

  public Q_SLOTS:
    void setCharCodec( const QString &codecName );
    void setUndefinedChar( QChar undefinedChar );

  private:
    static const int NofOfValueCodings = 4;

  private:
    Okteta::ValueCodec *mValueCodec[NofOfValueCodings];
    Okteta::CharCodec *mCharCodec;
    QChar mUndefinedChar;
};

}

#endif
