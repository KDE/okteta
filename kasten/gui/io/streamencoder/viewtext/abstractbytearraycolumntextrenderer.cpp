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

#include "abstractbytearraycolumntextrenderer.h"

// Qt
#include <QtCore/QTextStream>


namespace Kasten2
{

AbstractByteArrayColumnTextRenderer::AbstractByteArrayColumnTextRenderer( const Okteta::AbstractByteArrayModel *byteArrayModel, Okteta::Address offset, const Okteta::CoordRange& coordRange,
    int noOfBytesPerLine )
 : mByteArrayModel( byteArrayModel ),
   mCoordRange( coordRange ),
   mNoOfBytesPerLine( noOfBytesPerLine ),
   mOffset( offset ),
   mNoOfCharsPerLine( 0 ),
   mLinePositions( new int[mNoOfBytesPerLine] )
{
}

void AbstractByteArrayColumnTextRenderer::setWidths( int byteWidth, int byteSpacingWidth, int noOfGroupedBytes )
{
    // TODO: remove this hack and make it more general
    if( byteSpacingWidth > 0 )
        byteSpacingWidth = DefaultTRByteSpacingWidth;

    int spacingTrigger = noOfGroupedBytes-1;
    if( spacingTrigger < 0 )
        spacingTrigger = mNoOfBytesPerLine; // ensures to never trigger the group spacing

    int N = 0;
    int p = 0;
    int gs = 0;
    int *P = mLinePositions;
    for( ; P<&mLinePositions[mNoOfBytesPerLine]; ++P, ++p, ++gs )
    {
        *P = N;
        N += byteWidth;

        // is there a space behind the actual byte (if it is not the last)?
        if( gs == spacingTrigger )
        {
            N += TRGroupSpacingWidth;
            gs = -1;
        }
        else
            N += byteSpacingWidth;
    }
    N -= (gs==0)?TRGroupSpacingWidth:byteSpacingWidth;

    mNoOfCharsPerLine = N;
}


void AbstractByteArrayColumnTextRenderer::renderFirstLine( QTextStream *stream, int lineIndex ) const
{
    mRenderLine = lineIndex;
    renderLine( stream, false );
}


void AbstractByteArrayColumnTextRenderer::renderNextLine( QTextStream* stream, bool isSubline ) const
{
    renderLine( stream, isSubline );
}

AbstractByteArrayColumnTextRenderer::~AbstractByteArrayColumnTextRenderer()
{
    delete [] mLinePositions;
}

}
