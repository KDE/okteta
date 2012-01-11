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

#ifndef ABSTRACTBYTEARRAYCOLUMNTEXTRENDERER_H
#define ABSTRACTBYTEARRAYCOLUMNTEXTRENDERER_H

// lib
#include "abstractcolumntextrenderer.h"
// Okteta gui
#include <coordrange.h>


namespace Okteta {
class AbstractByteArrayModel;
}


namespace Kasten2
{

// TODO: offset should be set in renderFirstLine, calculated using coordRange,
// in constructor instead take startOffset
class AbstractByteArrayColumnTextRenderer : public AbstractColumnTextRenderer
{
  private:
    static const int DefaultTRByteSpacingWidth = 1;
    static const int TRGroupSpacingWidth = 2;

  public:
    AbstractByteArrayColumnTextRenderer( const Okteta::AbstractByteArrayModel* byteArrayModel, Okteta::Address offset,
        const Okteta::CoordRange& coordRange,
        int noOfBytesPerLine );
    virtual ~AbstractByteArrayColumnTextRenderer();

  public: // AbstractColumnTextRenderer API
    virtual void renderFirstLine( QTextStream* stream, int lineIndex ) const;
    virtual void renderNextLine( QTextStream* stream, bool isSubline ) const;

  protected: // API to be reimplemented by subclasses
    virtual void renderLine( QTextStream* stream, bool isSubline ) const = 0;

  protected:
    void setWidths( int byteWidth, int byteSpacingWidth, int noOfGroupedBytes );

  protected:
    const Okteta::AbstractByteArrayModel* mByteArrayModel;

    const Okteta::CoordRange mCoordRange;

    const int mNoOfBytesPerLine;

    /** Line to print */
    mutable int mRenderLine;
    /** Data to print */
    mutable Okteta::Address mOffset;

    /** buffered value of how many chars a line needs */
    int mNoOfCharsPerLine;
    // positions where to paint the
    int* const mLinePositions;
};

}

#endif
