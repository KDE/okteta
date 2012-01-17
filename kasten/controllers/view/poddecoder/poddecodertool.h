/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    Copyright 2007-2010 Friedrich W. H. Kossebau <kossebau@kde.org>

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
class AbstractTypeCodec;
class CharCodec;
class AbstractByteArrayModel;
}


namespace Kasten2
{
class AbstractDifferentSizeDialog;
class ByteArrayView;


class OKTETAKASTENCONTROLLERS_EXPORT PODDecoderTool : public AbstractTool
{
  Q_OBJECT

  public:
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
    bool isApplyable() const; // candidate for AbstractTool API
    bool isReadOnly() const;
    QVariant value( int podId ) const;
    QString nameOfPOD( int podId ) const;
    int podCount() const;

    bool isUnsignedAsHex() const;
    Okteta::ByteOrder byteOrder() const;
    Okteta::CharCodec* charCodec() const;

  public:
    void setData( const QVariant& data, int podId );
    void markPOD( int podId );
    void unmarkPOD();

    void setDifferentSizeDialog( AbstractDifferentSizeDialog* differentSizeDialog );

  public Q_SLOTS:
    void setUnsignedAsHex( bool unsignedAsHex );
    void setByteOrder( int byteOrder );

  Q_SIGNALS: // changes to the setting currently not signalled, because only controlled by view
    void isApplyableChanged( bool isApplyable );  // candidate for AbstractTool API
    void readOnlyChanged( bool isReadOnly );
    void dataChanged();

  private:
    void updateData();
    void setupDecoder();

  private Q_SLOTS:
    void onCursorPositionChange( Okteta::Address pos );
    void onContentsChange();
    void onReadOnlyChanged();

    void onCharCodecChange( const QString& codecName );
//     void onUndefinedCharChanged( const QChar& undefinedChar );

  private: // source
    ByteArrayView* mByteArrayView;
    Okteta::AbstractByteArrayModel* mByteArrayModel;
    Okteta::Address mCursorIndex;

    bool mReadOnly :1;
    bool mIsPodMarked :1;

    QVector<Okteta::AbstractTypeCodec*> mTypeCodecs;
    Okteta::CharCodec* mCharCodec;
    AbstractDifferentSizeDialog* mDifferentSizeDialog;

  private: // settings
    bool mUnsignedAsHex :1;

  private: // decoded data
    Okteta::PODData mPODData;
    QVector<QVariant> mDecodedValueList;
    QVector<int> mDecodedValueByteCountList;
};


inline bool PODDecoderTool::isUnsignedAsHex() const { return mUnsignedAsHex; }
inline Okteta::ByteOrder PODDecoderTool::byteOrder() const { return mPODData.byteOrder(); }
inline Okteta::CharCodec* PODDecoderTool::charCodec() const { return mCharCodec; }

}

#endif
