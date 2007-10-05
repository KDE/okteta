/***************************************************************************
                          fill.h  -  description
                            -------------------
    begin                : Wed Mai 30 2007
    copyright            : 2007 by Friedrich W. H. Kossebau
    email                : kossebau@kde.org
***************************************************************************/

/***************************************************************************
*                                                                         *
*   This library is free software; you can redistribute it and/or         *
*   modify it under the terms of the GNU Library General Public           *
*   License version 2 as published by the Free Software Foundation.       *
*                                                                         *
***************************************************************************/


#ifndef OKTETACORE_TEST_UTIL_FILL_H
#define OKTETACORE_TEST_UTIL_FILL_H


class QByteArray;
namespace KHE { class KSection; }
namespace KHECore { class KAbstractByteArrayModel; }


// some defines
static const char FirstMarkChar = 2;
static const char SecondMarkChar = 3;
static const char PaintChar = 1;
static const char BlankChar = '\0';

void textureByteArrayModel( KHECore::KAbstractByteArrayModel *ByteArrayModel,
                            unsigned char b = 1, unsigned char e = 255,
                            unsigned int From = 0, int To = -1 );

void textureByteArrayModel( KHECore::KAbstractByteArrayModel *byteArrayModel,
                            unsigned char b, unsigned char e,
                            const KHE::KSection &section );

void textureByteArray( QByteArray *byteArray,
                       unsigned char b = 1, unsigned char e = 255,
                       unsigned int From = 0, int To = -1 );

void textureByteArray( QByteArray *byteArray,
                       unsigned char b, unsigned char e,
                       const KHE::KSection &section );

#endif
