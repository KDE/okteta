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

#ifndef OFFSETCOLUMNTEXTRENDERER_H
#define OFFSETCOLUMNTEXTRENDERER_H

// lib
#include "abstractcolumntextrenderer.h"
// Okteta gui
#include <offsetformat.h>


namespace Kasten2
{

class OffsetColumnTextRenderer : public AbstractColumnTextRenderer
{
  public:
    OffsetColumnTextRenderer( int offsetFormat, int firstLineOffset, int delta );
    virtual ~OffsetColumnTextRenderer();

  public: // AbstractColumnTextRenderer API
    virtual void renderFirstLine( QTextStream *stream, int lineIndex ) const;
    virtual void renderNextLine( QTextStream* stream, bool isSubline ) const;

  protected:
    void renderLine( QTextStream* stream, bool isSubline ) const;

  protected:
    const int mFirstLineOffset;
    const int mDelta;
    const Okteta::OffsetFormat::print printFunction;
    const int mCodingWidth;

    /** the line we are in */
    mutable int mRenderLine;
    mutable char* mEncodedOffsetBuffer;
};

}

#endif
