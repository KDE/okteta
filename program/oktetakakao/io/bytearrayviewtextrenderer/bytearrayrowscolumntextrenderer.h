/*
    This file is part of the Okteta Kakao module, part of the KDE project.

    Copyright 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#ifndef BYTEARRAYROWSCOLUMNTEXTRENDERER_H
#define BYTEARRAYROWSCOLUMNTEXTRENDERER_H

// lib
#include "abstractcolumntextrenderer.h"
// Okteta gui
#include <coordrange.h>
// Okteta core
#include <khe.h>
// Qt
#include <QtCore/QChar>

namespace KHECore {
class ValueCodec;
class CharCodec;
class AbstractByteArrayModel;
}

// TODO: offset should be set in renderFirstLine, calculated using coordRange,
// in constructor instead take startOffset
class ByteArrayRowsColumnTextRenderer : public AbstractColumnTextRenderer
{
  public:
    ByteArrayRowsColumnTextRenderer( const KHECore::AbstractByteArrayModel* byteArrayModel, int offset,
        const KHEUI::CoordRange& coordRange,
        int noOfBytesPerLine, int byteSpacingWidth, int noOfGroupedBytes,
        int visibleCodings,
        KHECore::ValueCoding valueCoding,
        const QString& charCodecName, QChar substituteChar, QChar undefinedChar );
    virtual ~ByteArrayRowsColumnTextRenderer();

  public: // AbstractColumnTextRenderer API
    virtual void renderFirstLine( QTextStream* stream, int lineIndex ) const;
    virtual void renderNextLine( QTextStream* stream, bool isSubline ) const;
    virtual int noOfSublinesNeeded() const;

  protected:
    void renderLine( QTextStream* stream, bool isSubline ) const;

  protected:
    void setWidths( int byteWidth, int byteSpacingWidth, int noOfGroupedBytes );

  protected:
    const KHECore::AbstractByteArrayModel* mByteArrayModel;

    const KHEUI::CoordRange mCoordRange;

    const int mNoOfBytesPerLine;

    const int mVisibleCodings;
    const KHECore::ValueCodec* mValueCodec;
    const KHECore::CharCodec* mCharCodec;
    const QChar mSubstituteChar;
    const QChar mUndefinedChar;

    /** Line to print */
    mutable int mRenderLine;
    /** Data to print */
    mutable int mOffset;

    /** buffered value of how many chars a line needs */
    int mNoOfCharsPerLine;
    // positions where to paint the
    int* const mLinePositions;
};

#endif
