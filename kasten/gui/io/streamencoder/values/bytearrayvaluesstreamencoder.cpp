/*
    This file is part of the Okteta Kasten module, made within the KDE community.

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

#include "bytearrayvaluesstreamencoder.hpp"

// lib
#include <bytearrayview.hpp>
// Okteta core
#include <Okteta/AbstractByteArrayModel>
#include <Okteta/ValueCodec>
// KF
#include <KLocalizedString>
// Qt
#include <QTextStream>

namespace Kasten {

ValuesStreamEncoderSettings::ValuesStreamEncoderSettings()
    : separation(QStringLiteral(" "))
{}

ByteArrayValuesStreamEncoder::ByteArrayValuesStreamEncoder()
    : AbstractByteArrayStreamEncoder(i18nc("name of the encoding target", "Values"), QStringLiteral("text/plain"))
{}

ByteArrayValuesStreamEncoder::~ByteArrayValuesStreamEncoder() = default;

bool ByteArrayValuesStreamEncoder::encodeDataToStream(QIODevice* device,
                                                      const ByteArrayView* byteArrayView,
                                                      const Okteta::AbstractByteArrayModel* byteArrayModel,
                                                      const Okteta::AddressRange& range)
{
    bool success = true;

    // settings
    mSettings.undefinedChar = byteArrayView->undefinedChar();
    mSettings.substituteChar = byteArrayView->substituteChar();
    mSettings.valueCoding = (Okteta::ValueCoding)byteArrayView->valueCoding();

    // encode
    QTextStream textStream(device);

    Okteta::ValueCodec* valueCodec = Okteta::ValueCodec::createCodec(mSettings.valueCoding);

    // prepare
    QString valueString;
    valueString.resize(valueCodec->encodingWidth());

    for (Okteta::Address i = range.start(); i <= range.end(); ++i) {
        if (i > range.start()) {
            textStream << mSettings.separation;
        }

        valueCodec->encode(&valueString, 0, byteArrayModel->byte(i));

        textStream << valueString;
    }

    // clean up
    delete valueCodec;

    return success;
}

}
