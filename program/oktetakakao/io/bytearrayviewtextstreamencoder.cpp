/*
    This file is part of the Okteta Kakao module, part of the KDE project.

    Copyright 2007-2008 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "bytearrayviewtextstreamencoder.h"

// lib
#include "bytearrayviewtextrenderer/offsetcolumntextrenderer.h"
#include "bytearrayviewtextrenderer/bordercolumntextrenderer.h"
#include "bytearrayviewtextrenderer/valuebytearraycolumntextrenderer.h"
#include "bytearrayviewtextrenderer/charbytearraycolumntextrenderer.h"
#include <kbytearraydisplay.h>
// Okteta gui
#include <bytearraytablelayout.h>
#include <abstractbytearrayview.h>
// Okteta core
#include <kabstractbytearraymodel.h>
#include <khechar.h>
// KDE
#include <KLocale>
// Qt
#include <QtCore/QTextStream>


static const KHEUI::KOffsetFormat::KFormat DefaultOffsetFormat = KHEUI::KOffsetFormat::Hexadecimal;

ByteArrayViewTextStreamEncoderSettings::ByteArrayViewTextStreamEncoderSettings()
 : /*offsetFormat(DefaultOffsetFormat),*/ valueCoding( KHECore::HexadecimalCoding), codecName(), undefinedChar('?'), substituteChar( '.' ),
   separation( QLatin1String(" ") )
{}

ByteArrayViewTextStreamEncoder::ByteArrayViewTextStreamEncoder()
 : KAbstractByteArrayStreamEncoder( i18nc("name of the encoding target","View in Plain Text"), QLatin1String("text/plain") )
{}


bool ByteArrayViewTextStreamEncoder::encodeDataToStream( QIODevice *device,
                                                         const KByteArrayDisplay* byteArrayView,
                                                         const KHECore::KAbstractByteArrayModel *byteArrayModel,
                                                         const KHE::KSection &section )
{
    bool success = true;

    // settings
    mSettings.codecName = byteArrayView->charCodingName();
    mSettings.valueCoding = (KHECore::ValueCoding)byteArrayView->valueCoding();
    mSettings.undefinedChar = byteArrayView->undefinedChar();
    mSettings.substituteChar = byteArrayView->substituteChar();
    mSettings.firstLineOffset = byteArrayView->firstLineOffset();
    mSettings.startOffset = byteArrayView->startOffset();
    mSettings.delta = byteArrayView->noOfBytesPerLine();

    // setup
    KHEUI::ByteArrayTableLayout layout( byteArrayView->noOfBytesPerLine(), mSettings.firstLineOffset,
                                        mSettings.startOffset, byteArrayModel->size() );

    KHEUI::CoordRange coordRange;
    coordRange.set( layout.coordRangeOfIndizes(section) );

    const int noOfBytesPerLine = byteArrayView->noOfBytesPerLine();
    const int byteSpacingWidth = byteArrayView->byteSpacingWidth();
    const int noOfGroupedBytes = byteArrayView->noOfGroupedBytes();

    QList<AbstractColumnTextRenderer*> columnTextRendererList;

    if( byteArrayView->offsetColumnVisible() )
    {
        columnTextRendererList.append(
            new OffsetColumnTextRenderer(KHEUI::KOffsetFormat::Hexadecimal,mSettings.firstLineOffset,mSettings.delta) );
        columnTextRendererList.append( new BorderColumnTextRenderer() );
    }
    const int visibleByteArrayCodings = byteArrayView->visibleByteArrayCodings();
    if( visibleByteArrayCodings & KHEUI::AbstractByteArrayView::ValueCodingId )
        columnTextRendererList.append(
            new ValueByteArrayColumnTextRenderer(byteArrayModel,section.start(),coordRange,
                                                 noOfBytesPerLine, byteSpacingWidth, noOfGroupedBytes,
                                                 mSettings.valueCoding) );

    if( visibleByteArrayCodings & KHEUI::AbstractByteArrayView::CharCodingId )
    {
        if( visibleByteArrayCodings & KHEUI::AbstractByteArrayView::ValueCodingId )
            columnTextRendererList.append( new BorderColumnTextRenderer() );
        columnTextRendererList.append(
            new CharByteArrayColumnTextRenderer(byteArrayModel,section.start(),coordRange,
                                                noOfBytesPerLine, 0, 0,
                                                mSettings.codecName,mSettings.substituteChar,mSettings.undefinedChar) );
    }

    // encode
    QTextStream textStream( device );

    int l = coordRange.start().line();
    foreach( AbstractColumnTextRenderer* renderer, columnTextRendererList )
        renderer->renderFirstLine( &textStream, l );
    textStream << endl;
    for( ++l; l<=coordRange.end().line(); ++l )
    {
        foreach( AbstractColumnTextRenderer* renderer, columnTextRendererList )
            renderer->renderNextLine( &textStream );
        textStream << endl;
    }

    // clean up
    qDeleteAll( columnTextRendererList );

    return success;
}

ByteArrayViewTextStreamEncoder::~ByteArrayViewTextStreamEncoder() {}
