/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    Copyright 2003,2008 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "charbytearraycolumntextrenderer.h"

// Okteta core
#include <abstractbytearraymodel.h>
#include <charcodec.h>
#include <character.h>
// Qt
#include <QtCore/QTextStream>


namespace Kasten2
{

CharByteArrayColumnTextRenderer::CharByteArrayColumnTextRenderer(
        const Okteta::AbstractByteArrayModel* byteArrayModel, Okteta::Address offset,
        const Okteta::CoordRange& coordRange,
        int noOfBytesPerLine, int byteSpacingWidth, int noOfGroupedBytes,
        const QString& charCodecName, QChar substituteChar, QChar undefinedChar )
 : AbstractByteArrayColumnTextRenderer( byteArrayModel, offset, coordRange,
        noOfBytesPerLine ),
   mCharCodec( Okteta::CharCodec::createCodec(charCodecName) ),
   mSubstituteChar( substituteChar ),
   mUndefinedChar( undefinedChar )
{
    setWidths( 1, byteSpacingWidth, noOfGroupedBytes );
}


void CharByteArrayColumnTextRenderer::renderLine( QTextStream* stream, bool isSubline ) const
{
Q_UNUSED( isSubline )

    int p = 0;
    int pEnd = mNoOfBytesPerLine;
    // correct boundaries
    if( mRenderLine == mCoordRange.start().line() )
        p = mCoordRange.start().pos();
    if( mRenderLine == mCoordRange.end().line() )
        pEnd = mCoordRange.end().pos()+1;

    const QChar tabChar = QLatin1Char( '\t' );
    const QChar returnChar = QLatin1Char( '\n' );

    // draw individual chars
    uint e = 0;
    for( ; p<pEnd; ++p, ++mOffset )
    {
        // get next position
        const uint t = mLinePositions[p];
        // clear spacing
        *stream << whiteSpace( t-e );

        // print char
        const Okteta::Character byteChar = mCharCodec->decode( mByteArrayModel->byte(mOffset) );

        const QChar streamChar = byteChar.isUndefined() ?      Okteta::Character(mUndefinedChar) :
                                 (!byteChar.isPrint()
                                  || byteChar == tabChar
                                  || byteChar == returnChar) ? Okteta::Character(mSubstituteChar) :
                                                               byteChar;
        *stream << streamChar;

        e = t + 1;
    }

    *stream << whiteSpace( mNoOfCharsPerLine-e );

    ++mRenderLine;
}


CharByteArrayColumnTextRenderer::~CharByteArrayColumnTextRenderer()
{
    delete mCharCodec;
}

}
