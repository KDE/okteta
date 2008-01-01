/***************************************************************************
                          bytetabletool.h  -  description
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


#ifndef BYTETABLETOOL_H
#define BYTETABLETOOL_H


// Qt
#include <QtCore/QObject>

class ByteTableModel;

class KAbstractView;
namespace KHEUI {
class KByteArrayView;
}
namespace KHECore {
class KAbstractByteArrayModel;
}

/**
*/
class ByteTableTool : public QObject
{
  Q_OBJECT

  public:
    ByteTableTool();
    virtual ~ByteTableTool();

  public:
    void setView( KAbstractView *view );

  public:
    void insert( unsigned char byte, int count );

  public:
    ByteTableModel *byteTableModel() const;

  Q_SIGNALS:
    void modelChanged( bool hasModel );

  protected:
    ByteTableModel *mByteTableModel;

    KHEUI::KByteArrayView *mByteArrayView;
    KHECore::KAbstractByteArrayModel *mByteArrayModel;
};

#endif
