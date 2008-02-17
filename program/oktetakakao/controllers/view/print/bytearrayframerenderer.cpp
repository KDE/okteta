/***************************************************************************
                          bytearrayframerenderer.cpp  -  description
                             -------------------
    begin                : Tue Dec 11 2007
    copyright            : 2007 by Friedrich W. H. Kossebau
    email                : kossebau@kde.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU General Public License           *
 *   version 2 as published by the Free Software Foundation.               *
 *                                                                         *
 ***************************************************************************/

#include "bytearrayframerenderer.h"

// lib
#include "offsetcolumnrenderer.h"
#include "valuecolumnrenderer.h"
#include "charcolumnrenderer.h"
#include "bordercolumnrenderer.h"
// Okteta gui
#include <kdatalayout.h>
// Okteta core
#include <kabstractbytearraymodel.h>
#include <valuecodec.h>
#include <kcharcodec.h>
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
static const KResizeStyle DefaultResizeStyle = FullSizeUsage;
static const KHECore::KEncoding DefaultEncoding = KHECore::LocalEncoding;


static const int BAFInitialHeight = 50;
static const int BAFInitialWidth = 50;

ByteArrayFrameRenderer::ByteArrayFrameRenderer()
 : mHeight( BAFInitialHeight ),
   mWidth( BAFInitialWidth ),
   mByteArrayModel( 0 ),
   mOffset( 0 ),
   mLength( 0 ),
   mCodec( 0 ),
   mResizeStyle( DefaultResizeStyle )
{
    mDataLayout = new KHEUI::KDataLayout( DefaultNoOfBytesPerLine, DefaultStartOffset, 0 );
    mDataLayout->setLength( 0 );//mByteArrayModel->size() );
    mDataLayout->setNoOfLinesPerPage( noOfLinesPerFrame() );

    // creating the columns in the needed order
    mOffsetColumnRenderer =
        new OffsetColumnRenderer( this, DefaultFirstLineOffset, DefaultNoOfBytesPerLine, KOffsetFormat::Hexadecimal );
    mFirstBorderColumnRenderer =
        new BorderColumnRenderer( this, true );//false );
    mValueColumnRenderer =
        new ValueColumnRenderer( this, mByteArrayModel, mDataLayout );
    mSecondBorderColumnRenderer =
        new BorderColumnRenderer( this, true );
    mCharColumnRenderer =
        new CharColumnRenderer( this, mByteArrayModel, mDataLayout );

    // set encoding
    mCodec = KHECore::KCharCodec::createCodec( (KHECore::KEncoding)DefaultEncoding );
    mValueColumnRenderer->setCodec( mCodec );
    mCharColumnRenderer->setCodec( mCodec );
    mEncoding = DefaultEncoding;

    setFont( KGlobalSettings::fixedFont() );
}

const KHECore::KAbstractByteArrayModel *ByteArrayFrameRenderer::byteArrayModel() const { return mByteArrayModel; }
int ByteArrayFrameRenderer::offset()                                             const { return mOffset; }
int ByteArrayFrameRenderer::length()                                             const { return mLength; }

int ByteArrayFrameRenderer::noOfBytesPerLine()               const { return mDataLayout->noOfBytesPerLine(); }
int ByteArrayFrameRenderer::firstLineOffset()                const { return mOffsetColumnRenderer->firstLineOffset(); }
int ByteArrayFrameRenderer::startOffset()                    const { return mDataLayout->startOffset(); }
KResizeStyle ByteArrayFrameRenderer::resizeStyle()           const { return mResizeStyle; }
KHECore::KCoding ByteArrayFrameRenderer::coding()            const { return mValueColumnRenderer->coding(); }
KPixelX ByteArrayFrameRenderer::byteSpacingWidth()           const { return mValueColumnRenderer->byteSpacingWidth(); }
int ByteArrayFrameRenderer::noOfGroupedBytes()               const { return mValueColumnRenderer->noOfGroupedBytes(); }
KPixelX ByteArrayFrameRenderer::groupSpacingWidth()          const { return mValueColumnRenderer->groupSpacingWidth(); }
KPixelX ByteArrayFrameRenderer::binaryGapWidth()             const { return mValueColumnRenderer->binaryGapWidth(); }
bool ByteArrayFrameRenderer::showsNonprinting()              const { return mCharColumnRenderer->showsNonprinting(); }
QChar ByteArrayFrameRenderer::substituteChar()               const { return mCharColumnRenderer->substituteChar(); }
QChar ByteArrayFrameRenderer::undefinedChar()                const { return mCharColumnRenderer->undefinedChar(); }
KHECore::KEncoding ByteArrayFrameRenderer::encoding()        const { return mEncoding; }
const QString &ByteArrayFrameRenderer::encodingName()        const { return mCodec->name(); }

bool ByteArrayFrameRenderer::offsetColumnVisible() const { return mOffsetColumnRenderer->isVisible(); }
int ByteArrayFrameRenderer::visibleByteArrayColumns() const
{ return (mValueColumnRenderer->isVisible() ? ValueColumnId : 0) | (mCharColumnRenderer->isVisible() ? CharColumnId : 0); }

int ByteArrayFrameRenderer::height() const { return mHeight; }
int ByteArrayFrameRenderer::width() const { return mWidth; }

int ByteArrayFrameRenderer::framesCount() const
{
    const int charsPerFrame = mDataLayout->noOfBytesPerLine() * noOfLinesPerFrame();

    // clever calculation works: at least one page for the rest
    // hard to describe, think yourself
    const int frames = ( (mLength-1) / charsPerFrame ) + 1;

    return frames;
}

void ByteArrayFrameRenderer::setByteArrayModel( const KHECore::KAbstractByteArrayModel *byteArrayModel,
                                                int offset, int length )
{
    mByteArrayModel = byteArrayModel;
    mOffset = offset;
    mLength = ( byteArrayModel == 0 ) ?              0 :
              ( length == -1 ) ?                     byteArrayModel->size() :
              ( length <= byteArrayModel->size() ) ? length :
              byteArrayModel->size();

    mValueColumnRenderer->setByteArrayModel( byteArrayModel );
    mCharColumnRenderer->setByteArrayModel( byteArrayModel );

    // affected:
    // length -> no of lines -> width
    mDataLayout->setLength( mLength );
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
    mOffsetColumnRenderer->setFirstLineOffset( firstLineOffset );
}

void ByteArrayFrameRenderer::setStartOffset( int startOffset )
{
    if( !mDataLayout->setStartOffset(startOffset) )
        return;

    // affects:
    // the no of lines -> width
    adjustLayoutToSize();
}


void ByteArrayFrameRenderer::setBufferSpacing( KPixelX byteSpacing, int noOfGroupedBytes, KPixelX groupSpacing )
{
    if( !mValueColumnRenderer->setSpacing(byteSpacing,noOfGroupedBytes,groupSpacing) )
        return;

    adjustToWidth();
}


void ByteArrayFrameRenderer::setCoding( KHECore::KCoding coding )
{
    const uint oldCodingWidth = mValueColumnRenderer->byteCodec()->encodingWidth();

    if( !mValueColumnRenderer->setCoding(coding) )
        return;

    const uint newCodingWidth = mValueColumnRenderer->byteCodec()->encodingWidth();

    // change in the width?
    if( newCodingWidth != oldCodingWidth )
        adjustToWidth();
}


void ByteArrayFrameRenderer::setResizeStyle( KResizeStyle style )
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

    if( !mDataLayout->setNoOfBytesPerLine(noOfBytesPerLine) )
        return;
    adjustToWidth();
}


void ByteArrayFrameRenderer::setByteSpacingWidth( KPixelX byteSpacingWidth )
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


void ByteArrayFrameRenderer::setGroupSpacingWidth( KPixelX groupSpacingWidth )
{
    if( !mValueColumnRenderer->setGroupSpacingWidth(groupSpacingWidth) )
        return;
    adjustToWidth();
}


void ByteArrayFrameRenderer::setBinaryGapWidth( KPixelX binaryGapWidth )
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
    mCharColumnRenderer->setShowsNonprinting( showsNonprinting );
}


void ByteArrayFrameRenderer::setEncoding( KHECore::KEncoding encoding )
{
    if( mEncoding == encoding )
        return;

    KHECore::KCharCodec *newCharCodec = KHECore::KCharCodec::createCodec( encoding );
    if( newCharCodec == 0 )
        return;

    mValueColumnRenderer->setCodec( newCharCodec );
    mCharColumnRenderer->setCodec( newCharCodec );

    delete mCodec;
    mCodec = newCharCodec;
    mEncoding = encoding;
}

// TODO: join with function above!
void ByteArrayFrameRenderer::setEncoding( const QString &encodingName )
{
    if( encodingName == mCodec->name() )
        return;

    KHECore::KCharCodec *newCharCodec = KHECore::KCharCodec::createCodec( encodingName );
    if( newCharCodec == 0 )
        return;

    mValueColumnRenderer->setCodec( newCharCodec );
    mCharColumnRenderer->setCodec( newCharCodec );

    delete mCodec;
    mCodec = newCharCodec;
    mEncoding = KHECore::LocalEncoding; // TODO: add encoding no to every known codec
}


void ByteArrayFrameRenderer::setFont( const QFont &font )
{
    mFont = font;

    // get new values
    QFontMetrics fontMetrics( font );
    KPixelX digitWidth = fontMetrics.maxWidth();
    KPixelY digitBaseLine = fontMetrics.ascent();

    setLineHeight( fontMetrics.height() );

    // update all dependant structures
    mDataLayout->setNoOfLinesPerPage( noOfLinesPerFrame() );

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
        if( mDataLayout->setNoOfBytesPerLine(bytesPerLine) )
            adjustToLayoutNoOfBytesPerLine();
    }

    setNoOfLines( mDataLayout->noOfLines() );
}


void ByteArrayFrameRenderer::adjustToLayoutNoOfBytesPerLine()
{
    mOffsetColumnRenderer->setDelta( mDataLayout->noOfBytesPerLine() );
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
    const KPixelX nonDataWidth =
        mOffsetColumnRenderer->visibleWidth()
        + mFirstBorderColumnRenderer->visibleWidth()
        + mSecondBorderColumnRenderer->visibleWidth();

    // abstract offset and border columns width
    const KPixelX maxDataWidth = width() - nonDataWidth;

    // prepare needed values
    const KPixelX charByteWidth = mCharColumnRenderer->isVisible() ? mCharColumnRenderer->digitWidth() : 0;
    const KPixelX valueByteWidth = mValueColumnRenderer->isVisible() ? mValueColumnRenderer->byteWidth() : 0;
    const KPixelX valueByteSpacingWidth = mValueColumnRenderer->isVisible() ? mValueColumnRenderer->byteSpacingWidth() : 0;
    KPixelX valueByteGroupSpacingWidth;
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

    KPixelX valueByteGroupWidth =  noOfGroupedBytes * valueByteWidth + (noOfGroupedBytes-1)*valueByteSpacingWidth;
    KPixelX charByteGroupWidth =   noOfGroupedBytes * charByteWidth;
    KPixelX charAndValueGroupWidth = (valueByteGroupWidth + valueByteGroupSpacingWidth) + charByteGroupWidth;

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
    int columns = visibleByteArrayColumns();

    // no changes or no column selected?
    if( newColumns == columns || !(newColumns&( ValueColumnId | CharColumnId )) )
        return;

    mValueColumnRenderer->setVisible( ValueColumnId & newColumns );
    mCharColumnRenderer->setVisible( CharColumnId & newColumns );
    mSecondBorderColumnRenderer->setVisible( newColumns == (ValueColumnId|CharColumnId) );

    adjustToWidth();
}

ByteArrayFrameRenderer::~ByteArrayFrameRenderer()
{
    delete mDataLayout;
    delete mCodec;
}
