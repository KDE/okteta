/*
    This file is part of the Okteta Kakao module, part of the KDE project.

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

#ifndef KBYTEARRAYTEXTSTREAMENCODER_H
#define KBYTEARRAYTEXTSTREAMENCODER_H

// lib
#include "kabstractbytearraystreamencoder.h"
// Qt
#include <QtCore/QString>

class TextStreamEncoderSettings
{
  public:
    TextStreamEncoderSettings();
  public:
    QString codecName;
    QChar undefinedChar;
    QChar substituteChar;
};

// TODO: this could rather be one of the default cop
class KByteArrayTextStreamEncoder : public KAbstractByteArrayStreamEncoder
{
    Q_OBJECT

  public:
    KByteArrayTextStreamEncoder();
    virtual ~KByteArrayTextStreamEncoder();

  protected: // KAbstractByteArrayStreamEncoder API
    virtual bool encodeDataToStream( QIODevice *device,
                                     const KHEUI::KByteArrayView *byteArrayView,
                                     const KHECore::KAbstractByteArrayModel *byteArrayModel,
                                     const KHE::KSection &section );

  protected:
    TextStreamEncoderSettings mSettings;
};

#endif
