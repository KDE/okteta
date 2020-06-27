/*
    This file is part of the Okteta Core library, made within the KDE community.

    SPDX-FileCopyrightText: 2007 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef OKTETACORE_TEST_UTIL_FILL_HPP
#define OKTETACORE_TEST_UTIL_FILL_HPP

//
#include <addressrange.hpp>
#include <byte.hpp>

namespace Okteta {
class AbstractByteArrayModel;
}
class QByteArray;

// some defines
static constexpr Okteta::Byte FirstMarkChar = 2;
static constexpr Okteta::Byte SecondMarkChar = 3;
static constexpr Okteta::Byte PaintChar = 1;
static constexpr Okteta::Byte BlankChar = '\0';

void textureByteArrayModel(Okteta::AbstractByteArrayModel* ByteArrayModel,
                           unsigned char b = 1, unsigned char e = 255,
                           unsigned int From = 0, int To = -1);

void textureByteArrayModel(Okteta::AbstractByteArrayModel* byteArrayModel,
                           unsigned char b, unsigned char e,
                           const Okteta::AddressRange& range);

void textureByteArray(QByteArray* byteArray,
                      unsigned char b = 1, unsigned char e = 255,
                      unsigned int From = 0, int To = -1);

void textureByteArray(QByteArray* byteArray,
                      unsigned char b, unsigned char e,
                      const Okteta::AddressRange& range);

#endif
