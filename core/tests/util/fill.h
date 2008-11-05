/*
    This file is part of the Okteta Core library, part of the KDE project.

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

#ifndef OKTETACORE_TEST_UTIL_FILL_H
#define OKTETACORE_TEST_UTIL_FILL_H


class QByteArray;
namespace KHE { class Section; }
namespace KHECore { class AbstractByteArrayModel; }


// some defines
static const char FirstMarkChar = 2;
static const char SecondMarkChar = 3;
static const char PaintChar = 1;
static const char BlankChar = '\0';

void textureByteArrayModel( KHECore::AbstractByteArrayModel *ByteArrayModel,
                            unsigned char b = 1, unsigned char e = 255,
                            unsigned int From = 0, int To = -1 );

void textureByteArrayModel( KHECore::AbstractByteArrayModel *byteArrayModel,
                            unsigned char b, unsigned char e,
                            const KHE::Section &section );

void textureByteArray( QByteArray *byteArray,
                       unsigned char b = 1, unsigned char e = 255,
                       unsigned int From = 0, int To = -1 );

void textureByteArray( QByteArray *byteArray,
                       unsigned char b, unsigned char e,
                       const KHE::Section &section );

#endif
