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

#ifndef STATISTICTABLEMODEL_H
#define STATISTICTABLEMODEL_H

// Okteta core
#include <oktetacore.h>
// Qt
#include <QtCore/QAbstractTableModel>

namespace Okteta {
class CharCodec;
class ValueCodec;
}


namespace Kasten2
{

class StatisticTableModel : public QAbstractTableModel
{
  Q_OBJECT

  public:
    enum ColumnIds
    {
        ValueId = 0,
        CharacterId = 1,
        CountId = 2,
        PercentId = 3,
        NoOfIds = 4 // TODO: what pattern is usually used to mark number of ids?
    };

  public:
    explicit StatisticTableModel( int *byteCount, QObject *parent = 0 );
    virtual ~StatisticTableModel();

  public: // QAbstractTableModel API
    virtual int rowCount( const QModelIndex &parent ) const;
    virtual int columnCount( const QModelIndex &parent ) const;
    virtual QVariant data( const QModelIndex &index, int role ) const;
    virtual QVariant headerData( int section, Qt::Orientation orientation, int role ) const;

  public:
    void update( int size );

  public Q_SLOTS:
    void setCharCodec( const QString &codecName );
    void setValueCoding( int valueCoding );
    void setUndefinedChar( QChar undefinedChar );

  Q_SIGNALS:
    void headerChanged();
    void sizeChanged( int size );

  private:
    int mSize;
    int *mByteCount;

    Okteta::ValueCoding mValueCoding;
    Okteta::ValueCodec *mValueCodec;
    Okteta::CharCodec *mCharCodec;
    QChar mUndefinedChar;
};

}

#endif
