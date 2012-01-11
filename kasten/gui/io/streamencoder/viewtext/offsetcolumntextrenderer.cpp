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

#include "offsetcolumntextrenderer.h"

// Qt
#include <QtCore/QString>
#include <QtCore/QTextStream>


namespace Kasten2
{

OffsetColumnTextRenderer::OffsetColumnTextRenderer( int offsetFormat, int firstLineOffset, int delta )
  : mFirstLineOffset( firstLineOffset),
    mDelta( delta ),
    printFunction( Okteta::OffsetFormat::printFunction(offsetFormat) ),
    mCodingWidth( Okteta::OffsetFormat::codingWidth(offsetFormat) )
{
    mEncodedOffsetBuffer = new char[mCodingWidth+1];
}


void OffsetColumnTextRenderer::renderFirstLine( QTextStream *stream, int lineIndex ) const
{
    mRenderLine = lineIndex;
    renderLine( stream, false );
}

void OffsetColumnTextRenderer::renderNextLine( QTextStream* stream, bool isSubline ) const
{
    renderLine( stream, isSubline );
}

void OffsetColumnTextRenderer::renderLine( QTextStream* stream, bool isSubline ) const
{
    if( isSubline )
        *stream << whiteSpace( mCodingWidth );
    else
    {
        // TODO: fix me (no more printFunction)
        printFunction( mEncodedOffsetBuffer, mFirstLineOffset + mDelta*mRenderLine );
        *stream << mEncodedOffsetBuffer;

        ++mRenderLine;
    }
}

OffsetColumnTextRenderer::~OffsetColumnTextRenderer()
{
    delete [] mEncodedOffsetBuffer;
}

}
