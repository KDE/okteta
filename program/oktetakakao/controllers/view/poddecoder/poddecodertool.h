/***************************************************************************
                          poddecodertool.h  -  description
                             -------------------
    begin                : Wed Dec 26 2007
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


#ifndef PODDECODERTOOL_H
#define PODDECODERTOOL_H

// tool
#include "poddata.h"
// Qt
#include <QtCore/QObject>

class KAbstractView;
namespace KHEUI {
class KByteArrayView;
}
namespace KHECore {
class KAbstractByteArrayModel;
}

//TODO: remove if signal works again
class KPrimitiveTypesView;

class PODDecoderTool : public QObject
{
  Q_OBJECT

  public:
    // TODO: add PDP endianess
    enum ByteOrder
    {
        LittleEndianOrder = 0, // Intel, Alpha, ...
        BigEndianOrder =    1 // Sun, Motorola, ...
    };

    static const int MaxPODSize = sizeof(double);
    // ensure strict alignment for double as needed on some architectures (e.g. PA-RISC)
    typedef union { unsigned char Data[MaxPODSize]; double Dummy; } Aligned64Bit;

  public:
    PODDecoderTool();

  public:
    void setView( KAbstractView *view );
    //TODO: remove if signal works again
    void setPODView( KPrimitiveTypesView *view );

  public:
    PODData &podData();

  Q_SIGNALS:
    void dataChanged( const unsigned char *data );

  protected:
    void updateData();

  protected Q_SLOTS:
    void onCursorPositionChange( int pos );
    void onContentsChange( int start, int end );

  protected:
    KHEUI::KByteArrayView *mByteArrayView;
    KHECore::KAbstractByteArrayModel *mByteArrayModel;
    int mCursorIndex;

    //TODO: remove if signal works again
    KPrimitiveTypesView *mPODView;

    PODData mPODData;
};

#endif
