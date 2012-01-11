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

#include "valuebytearraycolumntextrenderer.h"

// Okteta core
#include <valuecodec.h>
#include <abstractbytearraymodel.h>
// Qt
#include <QtCore/QTextStream>


namespace Kasten2
{

ValueByteArrayColumnTextRenderer::ValueByteArrayColumnTextRenderer(
    const Okteta::AbstractByteArrayModel* byteArrayModel, Okteta::Address offset,
        const Okteta::CoordRange& coordRange,
        int noOfBytesPerLine, int byteSpacingWidth, int noOfGroupedBytes,
        Okteta::ValueCoding valueCoding )
  : AbstractByteArrayColumnTextRenderer( byteArrayModel, offset, coordRange,
    noOfBytesPerLine ),
   mValueCodec( Okteta::ValueCodec::createCodec(valueCoding) )
{
    setWidths( mValueCodec->encodingWidth(), byteSpacingWidth, noOfGroupedBytes );
}


void ValueByteArrayColumnTextRenderer::renderLine( QTextStream* stream, bool isSubline ) const
{
Q_UNUSED( isSubline )

    int p = 0;
    int pEnd = mNoOfBytesPerLine;
    // correct boundaries
    if( mRenderLine == mCoordRange.start().line() )
        p = mCoordRange.start().pos();
    if( mRenderLine == mCoordRange.end().line() )
        pEnd = mCoordRange.end().pos()+1;

    QString E;
    E.resize( mValueCodec->encodingWidth() );
    // draw individual chars
    uint e = 0;
    for( ; p<pEnd; ++p, ++mOffset )
    {
        // get next position
        const uint t = mLinePositions[p];
        // clear spacing
        *stream << whiteSpace( t-e );
        mValueCodec->encode( E, 0, mByteArrayModel->byte(mOffset) );
        *stream << E;
        e = t + mValueCodec->encodingWidth();
    }

    *stream << whiteSpace( mNoOfCharsPerLine-e );
    ++mRenderLine;
}

ValueByteArrayColumnTextRenderer::~ValueByteArrayColumnTextRenderer()
{
    delete mValueCodec;
}

}
