/*
    This file is part of the Okteta Kakao module, part of the KDE project.

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
#include <khechar.h>
// Qt
#include <QtCore/QTextStream>


CharByteArrayColumnTextRenderer::CharByteArrayColumnTextRenderer(
        const KHECore::AbstractByteArrayModel *byteArrayModel, int offset,
        const KHEUI::CoordRange &coordRange,
        int noOfBytesPerLine, int byteSpacingWidth, int noOfGroupedBytes,
        const QString &charCodecName, QChar substituteChar, QChar undefinedChar )
 : AbstractByteArrayColumnTextRenderer( byteArrayModel, offset, coordRange,
        noOfBytesPerLine ),
   mCharCodec( KHECore::CharCodec::createCodec(charCodecName) ),
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

    const QChar tabChar( '\t' );
    const QChar returnChar( '\n' );

    // draw individual chars
    uint e = 0;
    for( ; p<pEnd; ++p, ++mOffset )
    {
        // get next position
        const uint t = mLinePositions[p];
        // clear spacing
        *stream << whiteSpace( t-e );

        // print char
        const KHECore::KChar byteChar = mCharCodec->decode( mByteArrayModel->datum(mOffset) );

        const QChar streamChar = byteChar.isUndefined() ?      KHECore::KChar(mUndefinedChar) :
                                 (!byteChar.isPrint()
                                  || byteChar == tabChar
                                  || byteChar == returnChar) ? KHECore::KChar(mSubstituteChar) :
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
