/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    Copyright 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#ifndef BYTEARRAYBASE64STREAMENCODER_HPP
#define BYTEARRAYBASE64STREAMENCODER_HPP

// lib
#include "abstractbytearraystreamencoder.hpp"
// Okteta core
#include <okteta/oktetacore.hpp>
// Qt
#include <QString>

namespace Kasten {
extern const char base64EncodeMap[64];

class ByteArrayBase64StreamEncoder : public AbstractByteArrayStreamEncoder
{
    Q_OBJECT

public:
    static const int inputGroupLength = 3;

    static const int outputLineLength = 76;
    static const int outputGroupLength = 4;
    static const int maxOutputGroupsPerLine = outputLineLength / outputGroupLength;

    enum class InputByteIndex
    {
        First = 0,
        Second,
        Third
    };

public:
    ByteArrayBase64StreamEncoder();
    ~ByteArrayBase64StreamEncoder() override;

protected: // AbstractByteArrayStreamEncoder API
    bool encodeDataToStream(QIODevice* device,
                            const ByteArrayView* byteArrayView,
                            const Okteta::AbstractByteArrayModel* byteArrayModel,
                            const Okteta::AddressRange& range) override;
};

}

#endif
