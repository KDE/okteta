/*
    This file is part of the Okteta Kasten module, part of the KDE project.

    Copyright 2003,2008-2009 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "bytearrayframerenderer.h"

// lib
#include "printcolumnstylist.h"
// Okteta gui
#include <offsetcolumnrenderer.h>
#include <bordercolumnrenderer.h>
#include <valuebytearraycolumnrenderer.h>
#include <charbytearraycolumnrenderer.h>
#include <bytearraytablelayout.h>
#include <bytearraytableranges.h>
// Okteta core
#include <abstractbytearraymodel.h>
#include <valuecodec.h>
#include <charcodec.h>
// KDE
#include <KGlobalSettings>
// Qt
#include <QtCore/QHash>
#include <QtCore/QDateTime>
#include <QtGui/QPainter>
#include <QtGui/QFontMetrics>
#include <QtGui/QApplication>
#include <QtCore/QListIterator>


static const int DefaultStartOffset = 0;
static const int DefaultFirstLineOffset = 0;
static const int DefaultNoOfBytesPerLine =  16;
static const ResizeStyle DefaultResizeStyle = FullSizeUsage;
static const KHECore::ValueCoding DefaultValueCoding =  KHECore::HexadecimalCoding;
static const KHECore::CharCoding DefaultCharCoding = KHECore::LocalEncoding;


static const int BAFInitialHeight = 50;
static const int BAFInitialWidth = 50;


ByteArrayFrameRenderer::ByteArrayFrameRenderer()
 : mHeight( BAFInitialHeight ),
   mWidth( BAFInitialWidth ),
   mByteArrayModel( 0 ),
   mCharCodec( 0 ),
   mResizeStyle( DefaultResizeStyle )
{
    mLayout = new KHEUI::ByteArrayTableLayout( DefaultNoOfBytesPerLine, DefaultFirstLineOffset, DefaultStartOffset, 0, 0 );
    mLayout->setNoOfLinesPerPage( noOfLinesPerFrame() );
    mTableRanges = new KHEUI::ByteArrayTableRanges( mLayout );

    // set codecs
    mValueCodec = KHECore::ValueCodec::createCodec( (KHECore::ValueCoding)DefaultValueCoding );
    mValueCoding = DefaultValueCoding;
    mCharCodec = KHECore::CharCodec::createCodec( (KHECore::CharCoding)DefaultCharCoding );
    mCharCoding = DefaultCharCoding;

    mStylist = new KHEUI::PrintColumnStylist();

    const KHE::Section emptySection;
    // creating the columns in the needed order
    mOffsetColumnRenderer =
        new KHEUI::OffsetColumnRenderer( mStylist, mLayout, KHEUI::KOffsetFormat::Hexadecimal );
    mFirstBorderColumnRenderer =
        new KHEUI::BorderColumnRenderer( mStylist, true, false );
    mValueColumnRenderer =
        new KHEUI::ValueByteArrayColumnRenderer( mStylist, mByteArrayModel, mLayout, mTableRanges );
    mSecondBorderColumnRenderer =
        new KHEUI::BorderColumnRenderer( mStylist, true, false );
    mCharColumnRenderer =
        new KHEUI::CharByteArrayColumnRenderer( mStylist, mByteArrayModel, mLayout, mTableRanges );

    addColumn( mOffsetColumnRenderer );
    addColumn( mFirstBorderColumnRenderer );
    addColumn( mValueColumnRenderer );
    addColumn( mSecondBorderColumnRenderer );
    addColumn( mCharColumnRenderer );

    mValueColumnRenderer->setValueCodec( (KHECore::ValueCoding)mValueCoding, mValueCodec );
    mValueColumnRenderer->setCharCodec( mCharCodec );
    mCharColumnRenderer->setCharCodec( mCharCodec );

    setFont( KGlobalSettings::fixedFont() );
}

KHECore::AbstractByteArrayModel* ByteArrayFrameRenderer::byteArrayModel() const { return mByteArrayModel; }
int ByteArrayFrameRenderer::offset()                                             const { return mLayout->startOffset(); }
int ByteArrayFrameRenderer::length()                                             const { return mLayout->length(); }

int ByteArrayFrameRenderer::noOfBytesPerLine()               const { return mLayout->noOfBytesPerLine(); }
int ByteArrayFrameRenderer::firstLineOffset()                const { return mLayout->firstLineOffset(); }
int ByteArrayFrameRenderer::startOffset()                    const { return mLayout->startOffset(); }
ResizeStyle ByteArrayFrameRenderer::resizeStyle()            const { return mResizeStyle; }
KHECore::ValueCoding ByteArrayFrameRenderer::valueCoding()   const { return mValueCoding; }
KHEUI::KPixelX ByteArrayFrameRenderer::byteSpacingWidth()           const { return mValueColumnRenderer->byteSpacingWidth(); }
int ByteArrayFrameRenderer::noOfGroupedBytes()               const { return mValueColumnRenderer->noOfGroupedBytes(); }
KHEUI::KPixelX ByteArrayFrameRenderer::groupSpacingWidth()          const { return mValueColumnRenderer->groupSpacingWidth(); }
KHEUI::KPixelX ByteArrayFrameRenderer::binaryGapWidth()             const { return mValueColumnRenderer->binaryGapWidth(); }
bool ByteArrayFrameRenderer::showsNonprinting()              const { return mCharColumnRenderer->isShowingNonprinting(); }
QChar ByteArrayFrameRenderer::substituteChar()               const { return mCharColumnRenderer->substituteChar(); }
QChar ByteArrayFrameRenderer::undefinedChar()                const { return mCharColumnRenderer->undefinedChar(); }
KHECore::CharCoding ByteArrayFrameRenderer::charCoding()     const { return mCharCoding; }
const QString& ByteArrayFrameRenderer::charCodingName()      const { return mCharCodec->name(); }

bool ByteArrayFrameRenderer::offsetColumnVisible() const { return mOffsetColumnRenderer->isVisible(); }
int ByteArrayFrameRenderer::visibleByteArrayCodings() const
{ return (mValueColumnRenderer->isVisible() ? ValueCodingId : 0) | (mCharColumnRenderer->isVisible() ? CharCodingId : 0); }

int ByteArrayFrameRenderer::height() const { return mHeight; }
int ByteArrayFrameRenderer::width() const { return mWidth; }

int ByteArrayFrameRenderer::framesCount() const
{
    const int charsPerFrame = mLayout->noOfBytesPerLine() * noOfLinesPerFrame();

    // clever calculation works: at least one page for the rest
    // hard to describe, think yourself
    // TODO: needs to include the offset in the first line
    const int frames = ( (mLayout->length()-1) / charsPerFrame ) + 1;

    return frames;
}

void ByteArrayFrameRenderer::setByteArrayModel( KHECore::AbstractByteArrayModel* byteArrayModel,
                                                int offset, int length )
{
    mByteArrayModel = byteArrayModel;
    length = ( byteArrayModel == 0 ) ?                      0 :
             ( length == -1 ) ?                             byteArrayModel->size()-offset :
             ( length <= byteArrayModel->size()-offset ) ?  length :
                                                            byteArrayModel->size()-offset;

    mValueColumnRenderer->set( byteArrayModel );
    mCharColumnRenderer->set( byteArrayModel );

    // affected:
    // length -> no of lines -> width
    mLayout->setByteArrayOffset( offset );
    mLayout->setLength( length );

    adjustLayoutToSize();
}

void ByteArrayFrameRenderer::setHeight( int height ) { mHeight = height; }
void ByteArrayFrameRenderer::setWidth( int width )
{
    if( mWidth == width )
        return;

    mWidth = width;

    adjustToWidth();
}

void ByteArrayFrameRenderer::setFirstLineOffset( int firstLineOffset )
{
    if( !mLayout->setFirstLineOffset(firstLineOffset) )
        return;

    // affects:
    // the no of lines -> width
    adjustLayoutToSize();
}

void ByteArrayFrameRenderer::setStartOffset( int startOffset )
{
    if( !mLayout->setStartOffset(startOffset) )
        return;

    // affects:
    // the no of lines -> width
    adjustLayoutToSize();
}


void ByteArrayFrameRenderer::setBufferSpacing( KHEUI::KPixelX byteSpacing, int noOfGroupedBytes, KHEUI::KPixelX groupSpacing )
{
    if( !mValueColumnRenderer->setSpacing(byteSpacing,noOfGroupedBytes,groupSpacing) )
        return;

    adjustToWidth();
}


void ByteArrayFrameRenderer::setResizeStyle( ResizeStyle style )
{
    if( mResizeStyle == style )
        return;

    mResizeStyle = style;

    adjustToWidth();
}


void ByteArrayFrameRenderer::setNoOfBytesPerLine( int noOfBytesPerLine )
{
    // if the number is explicitly set we expect a wish for no automatic resize
    mResizeStyle = NoResize;

    if( !mLayout->setNoOfBytesPerLine(noOfBytesPerLine) )
        return;
    adjustToWidth();
}


void ByteArrayFrameRenderer::setByteSpacingWidth( KHEUI::KPixelX byteSpacingWidth )
{
    if( !mValueColumnRenderer->setByteSpacingWidth(byteSpacingWidth) )
        return;
    adjustToWidth();
}

void ByteArrayFrameRenderer::setNoOfGroupedBytes( int noOfGroupedBytes )
{
    if( !mValueColumnRenderer->setNoOfGroupedBytes(noOfGroupedBytes) )
        return;
    adjustToWidth();
}


void ByteArrayFrameRenderer::setGroupSpacingWidth( KHEUI::KPixelX groupSpacingWidth )
{
    if( !mValueColumnRenderer->setGroupSpacingWidth(groupSpacingWidth) )
        return;
    adjustToWidth();
}


void ByteArrayFrameRenderer::setBinaryGapWidth( KHEUI::KPixelX binaryGapWidth )
{
    if( !mValueColumnRenderer->setBinaryGapWidth(binaryGapWidth) )
        return;
    adjustToWidth();
}


void ByteArrayFrameRenderer::setSubstituteChar( QChar substituteChar )
{
    mCharColumnRenderer->setSubstituteChar( substituteChar );
}

void ByteArrayFrameRenderer::setUndefinedChar( QChar undefinedChar )
{
    mCharColumnRenderer->setUndefinedChar( undefinedChar );
}

void ByteArrayFrameRenderer::setShowsNonprinting( bool showsNonprinting )
{
    mCharColumnRenderer->setShowingNonprinting( showsNonprinting );
}


void ByteArrayFrameRenderer::setValueCoding( KHECore::ValueCoding valueCoding )
{
    if( mValueCoding == valueCoding )
        return;

    const uint oldCodingWidth = mValueCodec->encodingWidth();

    KHECore::ValueCodec* newValueCodec =
        KHECore::ValueCodec::createCodec( valueCoding );
    if( newValueCodec == 0 )
        return;

    delete mValueCodec;
    mValueCodec = newValueCodec;
    mValueCoding = valueCoding;

    mValueColumnRenderer->setValueCodec( (KHECore::ValueCoding)mValueCoding, mValueCodec );

    const uint newCodingWidth = mValueCodec->encodingWidth();

    // change in the width?
    if( newCodingWidth != oldCodingWidth )
        adjustToWidth();
}


void ByteArrayFrameRenderer::setCharCoding( KHECore::CharCoding charCoding )
{
    if( mCharCoding == charCoding )
        return;

    KHECore::CharCodec* newCharCodec = KHECore::CharCodec::createCodec( charCoding );
    if( newCharCodec == 0 )
        return;

    delete mCharCodec;
    mCharCodec = newCharCodec;
    mCharCoding = charCoding;

    mValueColumnRenderer->setCharCodec( mCharCodec );
    mCharColumnRenderer->setCharCodec( mCharCodec );
}

// TODO: join with function above!
void ByteArrayFrameRenderer::setCharCoding( const QString& newCharCodingName )
{
    if( charCodingName() == newCharCodingName )
        return;

    KHECore::CharCodec* newCharCodec = KHECore::CharCodec::createCodec( newCharCodingName );
    if( newCharCodec == 0 )
        return;

    delete mCharCodec;
    mCharCodec = newCharCodec;
    mCharCoding = KHECore::LocalEncoding; // TODO: add encoding no to every known codec

    mValueColumnRenderer->setCharCodec( mCharCodec );
    mCharColumnRenderer->setCharCodec( mCharCodec );
}


void ByteArrayFrameRenderer::setFont( const QFont &font )
{
    mFont = font;

    // get new values
    QFontMetrics fontMetrics( font );
    KHEUI::KPixelX digitWidth = fontMetrics.maxWidth();
    KHEUI::KPixelY digitBaseLine = fontMetrics.ascent();

    setLineHeight( fontMetrics.height() );

    // update all dependant structures
    mLayout->setNoOfLinesPerPage( noOfLinesPerFrame() );

    mOffsetColumnRenderer->setMetrics( digitWidth, digitBaseLine );
    mValueColumnRenderer->setMetrics( digitWidth, digitBaseLine );
    mCharColumnRenderer->setMetrics( digitWidth, digitBaseLine );

    adjustToWidth();
}

void ByteArrayFrameRenderer::prepare()
{
}

void ByteArrayFrameRenderer::renderFrame( QPainter *painter, int frameIndex )
{
    painter->setFont( mFont );
    AbstractColumnFrameRenderer::renderFrame( painter, frameIndex );
}

void ByteArrayFrameRenderer::adjustToWidth()
{
    adjustToLayoutNoOfBytesPerLine();
    adjustLayoutToSize();
}

void ByteArrayFrameRenderer::adjustLayoutToSize()
{
    // check whether there is a change with the numbers of fitting bytes per line
    if( mResizeStyle != NoResize )
    {
        const int bytesPerLine = fittingBytesPerLine();

        // changes?
        if( mLayout->setNoOfBytesPerLine(bytesPerLine) )
            adjustToLayoutNoOfBytesPerLine();
    }

    setNoOfLines( mLayout->noOfLines() );
}


void ByteArrayFrameRenderer::adjustToLayoutNoOfBytesPerLine()
{
    mValueColumnRenderer->resetXBuffer();
    mCharColumnRenderer->resetXBuffer();

    updateWidths();
}

void ByteArrayFrameRenderer::showOffsetColumn( bool visible )
{
    bool OCVisible = mOffsetColumnRenderer->isVisible();
    // no change?
    if( OCVisible == visible )
        return;

    mOffsetColumnRenderer->setVisible( visible );
    mFirstBorderColumnRenderer->setVisible( visible );

    adjustToWidth();
}

#if 0
QSize ByteArrayFrameRenderer::sizeHint() const
{
    return QSize( columnsWidth(), columnsHeight() );
}


QSize ByteArrayFrameRenderer::minimumSizeHint() const
{
    // TODO: better minimal width (visibility!)

    const int width =
        mOffsetColumnRenderer->visibleWidth()
        + mFirstBorderColumnRenderer->visibleWidth()
        + mSecondBorderColumnRenderer->visibleWidth()
        + mValueColumnRenderer->byteWidth()
        + mCharColumnRenderer->byteWidth();
    const int height = lineHeight() * noOfLines();

    return QSize( width, height );
}
#endif

int ByteArrayFrameRenderer::fittingBytesPerLine() const
{
    const KHEUI::KPixelX nonDataWidth =
        mOffsetColumnRenderer->visibleWidth()
        + mFirstBorderColumnRenderer->visibleWidth()
        + mSecondBorderColumnRenderer->visibleWidth();

    // abstract offset and border columns width
    const KHEUI::KPixelX maxDataWidth = width() - nonDataWidth;

    // prepare needed values
    const KHEUI::KPixelX charByteWidth = mCharColumnRenderer->isVisible() ? mCharColumnRenderer->digitWidth() : 0;
    const KHEUI::KPixelX valueByteWidth = mValueColumnRenderer->isVisible() ? mValueColumnRenderer->byteWidth() : 0;
    const KHEUI::KPixelX valueByteSpacingWidth = mValueColumnRenderer->isVisible() ? mValueColumnRenderer->byteSpacingWidth() : 0;
    KHEUI::KPixelX valueByteGroupSpacingWidth;
    int noOfGroupedBytes = mValueColumnRenderer->noOfGroupedBytes();
    // no grouping?
    if( noOfGroupedBytes == 0 )
    {
        // faking grouping by 1
        noOfGroupedBytes = 1;
        valueByteGroupSpacingWidth = 0;
    }
    else
        valueByteGroupSpacingWidth = mValueColumnRenderer->isVisible() ? mValueColumnRenderer->groupSpacingWidth() : 0;

    KHEUI::KPixelX valueByteGroupWidth =  noOfGroupedBytes * valueByteWidth + (noOfGroupedBytes-1)*valueByteSpacingWidth;
    KHEUI::KPixelX charByteGroupWidth =   noOfGroupedBytes * charByteWidth;
    KHEUI::KPixelX charAndValueGroupWidth = (valueByteGroupWidth + valueByteGroupSpacingWidth) + charByteGroupWidth;

    // calculate fitting groups per line

    // the last value byte group does not need a group spacing behind, but it gets into the calculation.
    // so we simply add one to the max width to match that
    const int fittingGroupsPerLine = (maxDataWidth+valueByteGroupSpacingWidth)
                                     / charAndValueGroupWidth;

    // calculate the fitting bytes per line by groups
    int fittingBytesPerLine = noOfGroupedBytes * fittingGroupsPerLine;

    // groups can be splitted and not only full groups are requested?
    if( noOfGroupedBytes > 1 && mResizeStyle == FullSizeUsage )
    {
        const int leftDataWidth = maxDataWidth - fittingGroupsPerLine * charAndValueGroupWidth;

        if( leftDataWidth > 0 )
        {
            const int charAndValueByteWidth = valueByteWidth + valueByteSpacingWidth + charByteWidth;
            // the last value byte  does not need a spacing behind, but it gets into the calculation.
            // so we simply add one to the left width to match that
            const int ungroupedBytes = (leftDataWidth+valueByteSpacingWidth)
                                       / charAndValueByteWidth;
            fittingBytesPerLine += ungroupedBytes;
        }

        // is there not even the space for a single byte?
//         if( fittingBytesPerLine < 1 )
            // ensure at least one byte per line
//             fittingBytesPerLine = 1;
    }
    else
    {
        // is there not the space for a single group?
//         if( fittingBytesPerLine < 1 )
            // ensures at least one group
//             fittingBytesPerLine = noOfGroupedBytes;
    }

    return fittingBytesPerLine;
}



void ByteArrayFrameRenderer::showByteArrayColumns( int newColumns )
{
    int columns = visibleByteArrayCodings();

    // no changes or no column selected?
    if( newColumns == columns || !(newColumns&( ValueCodingId | CharCodingId )) )
        return;

    mValueColumnRenderer->setVisible( ValueCodingId & newColumns );
    mCharColumnRenderer->setVisible( CharCodingId & newColumns );
    mSecondBorderColumnRenderer->setVisible( newColumns == (ValueCodingId|CharCodingId) );

    adjustToWidth();
}

ByteArrayFrameRenderer::~ByteArrayFrameRenderer()
{
    delete mStylist;
    delete mTableRanges;
    delete mLayout;
    delete mValueCodec;
    delete mCharCodec;
}
