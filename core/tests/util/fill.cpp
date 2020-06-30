/*
    This file is part of the Okteta Core library, made within the KDE community.

    SPDX-FileCopyrightText: 2007 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "fill.hpp"

//
#include <abstractbytearraymodel.hpp>
// Qt
#include <QByteArray>

using namespace Okteta;

// fills the buffer with char from b to e
void textureByteArrayModel(AbstractByteArrayModel* byteArrayModel,
                           unsigned char b, unsigned char e,
                           unsigned int From, int To)
{
    const Size size = byteArrayModel->size();

    // check
    if (To == -1 || To >= size) {
        To = size - 1;
    }
    if (b > e) {
        b = e;
    }

    // do
    unsigned char c = b;
    for (int i = From; i <= To; ++i) {
        byteArrayModel->setByte(i, c);
        if (c == e) {
            c = b;
        } else {
            c++;
        }
    }
}

void textureByteArrayModel(AbstractByteArrayModel* byteArrayModel,
                           unsigned char b, unsigned char e,
                           const AddressRange& range)
{
    textureByteArrayModel(byteArrayModel, b, e, range.start(), range.end());
}

void textureByteArray(QByteArray* byteArray,
                      unsigned char b, unsigned char e,
                      unsigned int From, int To)
{
    const int size = byteArray->size();

    // check
    if (To == -1 || To >= size) {
        To = size - 1;
    }
    if (b > e) {
        b = e;
    }

    // do
    unsigned char c = b;
    for (int i = From; i <= To; ++i) {
        (*byteArray)[i] = c;
        if (c == e) {
            c = b;
        } else {
            c++;
        }
    }
}

void textureByteArray(QByteArray* byteArray,
                      unsigned char b, unsigned char e,
                      const AddressRange& range)
{
    textureByteArray(byteArray, b, e, range.start(), range.end());
}

/*
static void list(AbstractByteArrayModel* byteArrayModel, const char* Name)
{
   unsigned int size = byteArrayModel->size();
   for (unsigned int i=0; i<size; ++i) {
        kdDebug() << Name<<":"<<i<<":"<<byteArrayModel->datum(i) << endl;
   }
}
*/
