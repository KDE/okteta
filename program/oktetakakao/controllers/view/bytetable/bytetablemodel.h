/***************************************************************************
                          bytetablemodel.h  -  description
                             -------------------
    begin                : Mon Dec 31 2007
    copyright            : 2007 by Friedrich W. H. Kossebau
    email                : kossebau@kde.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU General Public License           *
 *   version 2 as published by the Free Software Foundation.               *
 *                                                                         *
 ***************************************************************************/


#ifndef BYTETABLEMODEL_H
#define BYTETABLEMODEL_H


// Qt
#include <QtCore/QAbstractTableModel>

namespace KHECore {
class KCharCodec;
class KByteCodec; // TODO: rename to KValueCodec
}


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

  public:
    void setUndefinedChar( const QChar &undefinedChar );

  public Q_SLOTS:
    void setCharCodec( const QString &codecName );

  protected:
    KHECore::KByteCodec *mDecimalCodec;
    KHECore::KByteCodec *mHexadecimalCodec;
    KHECore::KByteCodec *mOctalCodec;
    KHECore::KByteCodec *mBinaryCodec;
    KHECore::KCharCodec *mCharCodec;
    QChar mUndefinedChar;
};

#endif
