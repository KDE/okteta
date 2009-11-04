/*
    This file is part of the Okteta Kasten module, part of the KDE project.

    Copyright 2007-2008 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#ifndef PODDECODERTOOL_H
#define PODDECODERTOOL_H

// lib
#include "oktetakastencontrollers_export.h"
// tool
#include "poddata.h"
// Kasten core
#include <abstracttool.h>
// Okteta core
#include <oktetacore.h>
#include <address.h>
// Qt
#include <QtCore/QVector>

namespace Okteta {
class CharCodec;
class AbstractByteArrayModel;
}
class QTextCodec;


namespace Kasten
{

class ByteArrayView;


class OKTETAKASTENCONTROLLERS_EXPORT PODDecoderTool : public AbstractTool
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

    virtual ~PODDecoderTool();

  public: // AbstractTool API
//     virtual AbstractModel* targetModel() const;
    virtual QString title() const;

    virtual void setTargetModel( AbstractModel* model );

  public:
    QString valueAsString( int podId ) const;
    QString nameOfPOD( int podId ) const;
    int podCount() const;

    bool isUnsignedAsHex() const;
    int byteOrder() const;

  public:
    void markPOD( int podId );
    void unmarkPOD();

  public Q_SLOTS:
    void setUnsignedAsHex( bool unsignedAsHex );
    void setByteOrder( int byteOrder );

  Q_SIGNALS: // changtes to the setting currently not signalled, because only controlled by view
    void dataChanged();

  protected:
    void updateData();
    void setupDecoder();

  protected Q_SLOTS:
    void onCursorPositionChange( Okteta::Address pos );
    void onContentsChange();

    void onCharCodecChange( const QString& codecName );
//     void onUndefinedCharChanged( const QChar& undefinedChar );

  protected: // source
    ByteArrayView* mByteArrayView;
    Okteta::AbstractByteArrayModel* mByteArrayModel;
    Okteta::Address mCursorIndex;
    Okteta::CharCodec* mCharCodec;
    QChar mUndefinedChar;

  protected: // settings
    bool mUnsignedAsHex;

  protected: // decoded data
    PODData mPODData;
    QVector<QString> mDecoderNameList;
    QVector<QString> mDecoderValueList;
    QVector<int> mDecoderByteLengthList;
    QTextCodec* mUtf8Codec;
};


inline bool PODDecoderTool::isUnsignedAsHex() const { return mUnsignedAsHex; }
inline int PODDecoderTool::byteOrder()        const { return mPODData.byteOrder(); }

}

#endif
