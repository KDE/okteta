/*
    This file is part of the Okteta Kakao module, part of the KDE project.

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

#ifndef BYTEARRAYVIEWTEXTSTREAMENCODER_H
#define BYTEARRAYVIEWTEXTSTREAMENCODER_H

// lib
#include "kabstractbytearraystreamencoder.h"
// Okteta gui
// #include <koffsetformat.h>
// Okteta core
#include <khe.h>
// Qt
#include <QtCore/QString>

class ByteArrayViewTextStreamEncoderSettings
{
  public:
    ByteArrayViewTextStreamEncoderSettings();
  public:
//     KHEUI::KOffsetFormat::KFormat offsetFormat;
    int codingWidth;
    int firstLineOffset;
    int startOffset;
    int delta;
    KHECore::ValueCoding valueCoding;
    QString codecName;
    QChar undefinedChar;
    QChar substituteChar;
    QString separation;
//     KHECore::CharCodec *CharCodec;
//     KHEUI::KOffsetFormat::print printFunction;
};

// TODO: this could rather be one of the default cop
class ByteArrayViewTextStreamEncoder : public KAbstractByteArrayStreamEncoder
{
    Q_OBJECT

  public:
    ByteArrayViewTextStreamEncoder();
    virtual ~ByteArrayViewTextStreamEncoder();

  protected: // KAbstractByteArrayStreamEncoder API
    virtual bool encodeDataToStream( QIODevice *device,
                                     const KByteArrayDisplay* byteArrayView,
                                     const KHECore::AbstractByteArrayModel *byteArrayModel,
                                     const KHE::Section &section );

  protected:
    ByteArrayViewTextStreamEncoderSettings mSettings;
};

#endif
