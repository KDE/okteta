/***************************************************************************
                          statistictablemodel.h  -  description
                             -------------------
    begin                : Wed Jan 2 2008
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


#ifndef STATISTICTABLEMODEL_H
#define STATISTICTABLEMODEL_H


// Qt
#include <QtCore/QAbstractTableModel>
// Okteta core
#include <khe.h>

namespace KHECore {
class KCharCodec;
class KByteCodec; // TODO: rename to KValueCodec
}


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
    void setUndefinedChar( const QChar &undefinedChar );

  public Q_SLOTS:
    void setCharCodec( const QString &codecName );
    void setValueCoding( int coding );

  Q_SIGNALS:
    void headerChanged();
    void sizeChanged( int size );

  protected:
    int mSize;
    int *mByteCount;

    KHECore::KCoding mValueCoding;
    KHECore::KByteCodec *mValueCodec;
    KHECore::KCharCodec *mCharCodec;
    QChar mUndefinedChar;
};

#endif
