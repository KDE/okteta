/*
    This file is part of the Okteta Gui library, part of the KDE project.

    Copyright 2003,2007-2008 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "kbytearrayview.h"

// lib
#include "offsetcolumnrenderer.h"
#include "valuebytearraycolumnrenderer.h"
#include "charbytearraycolumnrenderer.h"
#include "bordercolumnrenderer.h"
#include "kdatacursor.h"
#include "bytearraytablelayout.h"
#include "kdataranges.h"
#include "controller/ktabcontroller.h"
#include "controller/knavigator.h"
#include "controller/kvalueeditor.h"
#include "controller/kchareditor.h"
#include "controller/dropper.h"
#include "controller/zoomwheelcontroller.h"
#include "kcursor.h"
// Okteta core
#include <kbytearraymodel.h> // TODO: used as dummy view, make own real dummyZzz
#include <kabstractbytearraymodel.h>
#include <kversionable.h>
#include <kbookmarkable.h>
#include <valuecodec.h>
#include <kcharcodec.h>
#include <kwordbufferservice.h>
#include <arraychangemetricslist.h>
// #include <arraychange.h>
// KDE
#include <KGlobalSettings>
// Qt
#include <QtCore/QListIterator>
#include <QtCore/QTimer>
#include <QtGui/QStyle>
#include <QtGui/QPainter>
#include <QtGui/QCursor>
#include <QtGui/QApplication>
#include <QtGui/QScrollBar>
#include <QtGui/QShowEvent>
#include <QtGui/QResizeEvent>
#include <QtGui/QKeyEvent>
#include <QtGui/QMouseEvent>
#include <QtGui/QDragEnterEvent>
#include <QtGui/QDragMoveEvent>
#include <QtGui/QDragLeaveEvent>
#include <QtGui/QDropEvent>
// C
#include <stdlib.h>

namespace KHEUI
{

// zooming is done in steps of font size points
static const int DefaultZoomStep = 1;
static const int MinFontPointSize = 4;
static const int MaxFontPointSize = 128;
static const int DefaultStartOffset = 0;//5;
static const int DefaultFirstLineOffset = 0;
static const int DefaultNoOfBytesPerLine =  16;
static const KByteArrayView::KResizeStyle DefaultResizeStyle = KByteArrayView::FullSizeUsage;
static const KByteArrayView::KEncoding DefaultEncoding = KByteArrayView::LocalEncoding;
static const int DefaultScrollTimerPeriod = 100;
static const int InsertCursorWidth = 2;

static const char OctetStreamFormatName[] = "application/octet-stream";

class KByteArrayView::Private
{
};

KByteArrayView::KByteArrayView( KHECore::KAbstractByteArrayModel *byteArrayModel, QWidget *parent )
 : ColumnsView( parent ),
   mByteArrayModel( byteArrayModel ),
   mDataLayout( new ByteArrayTableLayout(DefaultNoOfBytesPerLine,DefaultFirstLineOffset,DefaultStartOffset,0) ),
   mDataCursor( new KDataCursor(mDataLayout) ),
   mDataRanges( new KDataRanges(mDataLayout) ),
   mCursorBlinkTimer( new QTimer(this) ),
   mScrollTimer( new QTimer(this) ),
   mDragStartTimer( new QTimer(this) ),
   mTrippleClickTimer( new QTimer(this) ),
   mCursorPixmaps( new KCursor() ),
   mCharCodec( 0 ),
   mClipboardMode( QClipboard::Clipboard ),
   mZoomLevel( 1.0 ),
   mResizeStyle( DefaultResizeStyle ),
   mReadOnly( false ),
   mOverWriteOnly( false ),
   mOverWrite( true ),
   mMousePressed( false ),
   mInDoubleClick( false ),
   mInDnD( false ),
   mDragStartPossible( false ),
   mCursorPaused( false ),
   mBlinkCursorVisible( false ),
   mInZooming( false ),
   d( new Private() )
{
    // initialize layout
    if( !mByteArrayModel )
        mByteArrayModel = new KHECore::KByteArrayModel; // TODO: leaking, make it shared
    connect( mByteArrayModel, SIGNAL(readOnlyChanged( bool )), SLOT(onByteArrayReadOnlyChange( bool )) );
    connect( mByteArrayModel, SIGNAL(contentsChanged( const KHE::ArrayChangeMetricsList & )),
             SLOT(onContentsChanged( const KHE::ArrayChangeMetricsList & )) );

    KHECore::Bookmarkable *bookmarks = qobject_cast<KHECore::Bookmarkable*>( mByteArrayModel );
    if( bookmarks )
    {
        connect( mByteArrayModel, SIGNAL(bookmarksAdded( const QList<KHECore::KBookmark>& )),
                 SLOT(onBookmarksChange(const QList<KHECore::KBookmark>&)) );
        connect( mByteArrayModel, SIGNAL(bookmarksRemoved( const QList<KHECore::KBookmark>& )),
                 SLOT(onBookmarksChange(const QList<KHECore::KBookmark>&)) );
    }
    KHECore::Versionable *versionControl = qobject_cast<KHECore::Versionable*>( mByteArrayModel );
    if( versionControl )
    {
        connect( mByteArrayModel, SIGNAL(revertedToVersionIndex( int )),
                 SLOT(onRevertedToVersionIndex( int )) );
    }

    mDataLayout->setLength( mByteArrayModel->size() );
    mDataLayout->setNoOfLinesPerPage( noOfLinesPerPage() );

    // creating the columns in the needed order
    mOffsetColumn =       new OffsetColumnRenderer( this, DefaultFirstLineOffset, DefaultNoOfBytesPerLine, KOffsetFormat::Hexadecimal );
    mFirstBorderColumn =  new BorderColumnRenderer( this, false );
    mValueColumn =        new ValueByteArrayColumnRenderer( this, mByteArrayModel, mDataLayout, mDataRanges );
    mSecondBorderColumn = new BorderColumnRenderer( this, true );
    mCharColumn =         new CharByteArrayColumnRenderer( this, mByteArrayModel, mDataLayout, mDataRanges );

    // select the active column
    mActiveColumn = &charColumn();
    mInactiveColumn = &valueColumn();

    // set char encoding
    mCharCodec = KHECore::KCharCodec::createCodec( (KHECore::KEncoding)DefaultEncoding );
    valueColumn().setCharCodec( mCharCodec );
    charColumn().setCharCodec( mCharCodec );
    mCharEncoding = DefaultEncoding;

    mTabController = new KTabController( this, 0 );
    mNavigator = new KNavigator( this, mTabController );
    mValueEditor = new KValueEditor( mValueColumn, mDataCursor, this, mNavigator );
    mCharEditor = new KCharEditor( mCharColumn, mDataCursor, this, mNavigator );
    mZoomWheelController = new ZoomWheelController( this, 0 );
    mDropper = new Dropper( this );
    mWheelController = mZoomWheelController;

    adaptController();

    setFont( KGlobalSettings::fixedFont() );

    connect( mCursorBlinkTimer, SIGNAL(timeout()), this, SLOT(blinkCursor()) );
    connect( mScrollTimer,      SIGNAL(timeout()), this, SLOT(autoScrollTimerDone()) );
    connect( mDragStartTimer,   SIGNAL(timeout()), this, SLOT(startDrag()) );
    mDragStartTimer->setSingleShot( true );
    mTrippleClickTimer->setSingleShot( true );

    setAcceptDrops( true );
}

const ValueByteArrayColumnRenderer& KByteArrayView::valueColumn()     const { return *mValueColumn; }
const CharByteArrayColumnRenderer& KByteArrayView::charColumn()       const { return *mCharColumn; }
const AbstractByteArrayColumnRenderer& KByteArrayView::activeColumn()   const { return *mActiveColumn; }
const AbstractByteArrayColumnRenderer& KByteArrayView::inactiveColumn() const { return *mInactiveColumn; }

ValueByteArrayColumnRenderer& KByteArrayView::valueColumn()     { return *mValueColumn; }
CharByteArrayColumnRenderer& KByteArrayView::charColumn()       { return *mCharColumn; }
AbstractByteArrayColumnRenderer& KByteArrayView::activeColumn()     { return *mActiveColumn; }
AbstractByteArrayColumnRenderer& KByteArrayView::inactiveColumn()   { return *mInactiveColumn; }

KHECore::KAbstractByteArrayModel *KByteArrayView::byteArrayModel() const { return mByteArrayModel; }
int KByteArrayView::noOfBytesPerLine()               const { return mDataLayout->noOfBytesPerLine(); }
int KByteArrayView::firstLineOffset()                const { return mOffsetColumn->firstLineOffset(); }
int KByteArrayView::startOffset()                    const { return mDataLayout->startOffset(); }
KByteArrayView::KResizeStyle KByteArrayView::resizeStyle() const { return mResizeStyle; }
KByteArrayView::KCoding KByteArrayView::coding() const
{ return (KByteArrayView::KCoding)valueColumn().valueCoding(); }
KPixelX KByteArrayView::byteSpacingWidth()           const { return valueColumn().byteSpacingWidth(); }
int KByteArrayView::noOfGroupedBytes()               const { return valueColumn().noOfGroupedBytes(); }
KPixelX KByteArrayView::groupSpacingWidth()          const { return valueColumn().groupSpacingWidth(); }
KPixelX KByteArrayView::binaryGapWidth()             const { return valueColumn().binaryGapWidth(); }
bool KByteArrayView::isOverwriteMode()               const { return mOverWrite; }
bool KByteArrayView::isOverwriteOnly()               const { return mOverWriteOnly; }
bool KByteArrayView::isReadOnly()                    const { return mReadOnly || mByteArrayModel->isReadOnly(); }
bool KByteArrayView::isModified()                    const { return mByteArrayModel->isModified(); }
bool KByteArrayView::tabChangesFocus()               const { return mTabController->tabChangesFocus(); }
bool KByteArrayView::showsNonprinting()              const { return charColumn().isShowingNonprinting(); }
QChar KByteArrayView::substituteChar()               const { return charColumn().substituteChar(); }
QChar KByteArrayView::undefinedChar()                const { return charColumn().undefinedChar(); }
KByteArrayView::KEncoding KByteArrayView::encoding()       const { return (KByteArrayView::KEncoding)mCharEncoding; }
const QString &KByteArrayView::encodingName()        const { return mCharCodec->name(); }
bool KByteArrayView::isByteTypeColored()             const { return valueColumn().isByteTypeColored(); }

double KByteArrayView::zoomLevel()                   const { return mZoomLevel; }

int KByteArrayView::cursorPosition() const { return mDataCursor->realIndex(); }
bool KByteArrayView::isCursorBehind() const { return mDataCursor->isBehind(); }
KByteArrayView::ByteArrayColumnId KByteArrayView::cursorColumn() const
{ return static_cast<KHEUI::ValueByteArrayColumnRenderer *>( mActiveColumn ) == &valueColumn()? ValueColumnId : CharColumnId; }
KHE::KSection KByteArrayView::selection() const { return mDataRanges->selection(); }

void KByteArrayView::setOverwriteOnly( bool OO )    { mOverWriteOnly = OO; if( mOverWriteOnly ) setOverwriteMode( true ); }
void KByteArrayView::setModified( bool M )          { mByteArrayModel->setModified(M); }
void KByteArrayView::setTabChangesFocus( bool TCF ) { mTabController->setTabChangesFocus(TCF); }

bool KByteArrayView::offsetColumnVisible() const { return mOffsetColumn->isVisible(); }
int KByteArrayView::visibleBufferColumns() const
{ return (valueColumn().isVisible() ? ValueColumnId : 0) | (charColumn().isVisible() ? CharColumnId : 0); }


void KByteArrayView::setOverwriteMode( bool overwriteMode )
{
    if( (mOverWriteOnly && !overwriteMode) || (mOverWrite == overwriteMode) )
        return;

    mOverWrite = overwriteMode;

    // affected:
    // cursor shape
    const bool ChangeCursor = !( mCursorPaused || mValueEditor->isInEditMode() );
    if( ChangeCursor )
        pauseCursor();

    mDataCursor->setAppendPosEnabled( !mOverWrite );

    if( ChangeCursor )
        unpauseCursor();

    emit cutAvailable( !mOverWrite && mDataRanges->hasSelection() );
}


void KByteArrayView::setByteArrayModel( KHECore::KAbstractByteArrayModel *byteArrayModel )
{
    mByteArrayModel->disconnect( this );

    mValueEditor->reset();
    mCursorPaused = true;

    mByteArrayModel = byteArrayModel;
    valueColumn().set( mByteArrayModel );
    charColumn().set( mByteArrayModel);

    // affected:
    // length -> no of lines -> width
    mDataLayout->setLength( mByteArrayModel->size() );
    adjustLayoutToSize();

    // if the model is readonly make the view too, per default
    if( mByteArrayModel->isReadOnly() )
        setReadOnly( true );

    connect( mByteArrayModel, SIGNAL(readOnlyChanged( bool )), SLOT(adaptController()) );
    connect( mByteArrayModel, SIGNAL(contentsChanged( const KHE::ArrayChangeMetricsList & )),
             SLOT(onContentsChanged( const KHE::ArrayChangeMetricsList & )) );

    KHECore::Bookmarkable *bookmarks = qobject_cast<KHECore::Bookmarkable*>( mByteArrayModel );
    if( bookmarks )
    {
        connect( mByteArrayModel, SIGNAL(bookmarksAdded( const QList<KHECore::KBookmark>& )),
                SLOT(onBookmarksChange(const QList<KHECore::KBookmark>&)) );
        connect( mByteArrayModel, SIGNAL(bookmarksRemoved( const QList<KHECore::KBookmark>& )),
                SLOT(onBookmarksChange(const QList<KHECore::KBookmark>&)) );
    }
    KHECore::Versionable *versionControl = qobject_cast<KHECore::Versionable*>( mByteArrayModel );
    if( versionControl )
    {
        connect( mByteArrayModel, SIGNAL(revertedToVersionIndex( int )),
                 SLOT(onRevertedToVersionIndex( int )) );
    }

    viewport()->update();

    mDataCursor->gotoStart();
    ensureCursorVisible();
    unpauseCursor();

    emit cursorPositionChanged( mDataCursor->realIndex() );
}


void KByteArrayView::setStartOffset( int startOffset )
{
    if( !mDataLayout->setStartOffset(startOffset) )
        return;

    pauseCursor();
    // affects:
    // the no of lines -> width
    adjustLayoutToSize();

    viewport()->update();

    mDataCursor->updateCoord();
    ensureCursorVisible();
    unpauseCursor();
    emit cursorPositionChanged( mDataCursor->realIndex() );
}

void KByteArrayView::setFirstLineOffset( int firstLineOffset )
{
    if( !mDataLayout->setFirstLineOffset(firstLineOffset) )
        return;
    mOffsetColumn->setFirstLineOffset( firstLineOffset ); // TODO: offset renderer should use layout, too

    pauseCursor();
    // affects:
    // the no of lines -> width
    adjustLayoutToSize();

    viewport()->update();

    mDataCursor->updateCoord();
    ensureCursorVisible();
    unpauseCursor();
    emit cursorPositionChanged( mDataCursor->realIndex() );
}

void KByteArrayView::setReadOnly( bool readOnly )
{
    if( mReadOnly == readOnly )
        return;

    mReadOnly = readOnly;

    adaptController();

    if( !mByteArrayModel->isReadOnly() )
        emit readOnlyChanged( mReadOnly );
}

void KByteArrayView::adaptController()
{
    const bool isEffectiveReadOnly = mByteArrayModel->isReadOnly() || mReadOnly;

    mController = isEffectiveReadOnly ?            (KController*)mNavigator :
                  cursorColumn() == CharColumnId ? (KController*)mCharEditor :
                                                   (KController*)mValueEditor;
}

void KByteArrayView::onByteArrayReadOnlyChange( bool isByteArrayReadOnly )
{
    adaptController();

    if( !mReadOnly )
        emit readOnlyChanged( isByteArrayReadOnly );
}

void KByteArrayView::setBufferSpacing( KPixelX ByteSpacing, int noOfGroupedBytes, KPixelX GroupSpacing )
{
    if( !valueColumn().setSpacing(ByteSpacing,noOfGroupedBytes,GroupSpacing) )
        return;

    updateViewByWidth();
}


void KByteArrayView::setCoding( KCoding coding )
{
    const uint oldCodingWidth = valueColumn().valueCodec()->encodingWidth();

    if( !valueColumn().setValueCoding((KHECore::KCoding)coding) )
        return;

    mValueEditor->adaptToValueCodecChange();

    const uint newCodingWidth = valueColumn().valueCodec()->encodingWidth();

    // no change in the width?
    if( newCodingWidth == oldCodingWidth )
        updateColumn( valueColumn() );
    else
        updateViewByWidth();

    emit valueCodingChanged( coding );
}


void KByteArrayView::setResizeStyle( KResizeStyle resizeStyle )
{
    if( mResizeStyle == resizeStyle )
        return;

    mResizeStyle = resizeStyle;

    updateViewByWidth();
}


void KByteArrayView::setNoOfBytesPerLine( int NoBpL )
{
    // if the number is explicitly set we expect a wish for no automatic resize
    mResizeStyle = NoResize;

    if( !mDataLayout->setNoOfBytesPerLine(NoBpL) )
        return;
    updateViewByWidth();
}


void KByteArrayView::setByteSpacingWidth( int/*KPixelX*/ byteSpacingWidth )
{
    if( !valueColumn().setByteSpacingWidth(byteSpacingWidth) )
        return;
    updateViewByWidth();
}

void KByteArrayView::setNoOfGroupedBytes( int noOfGroupedBytes )
{
    if( !valueColumn().setNoOfGroupedBytes(noOfGroupedBytes) )
        return;
    updateViewByWidth();
}


void KByteArrayView::setGroupSpacingWidth( int/*KPixelX*/ groupSpacingWidth )
{
    if( !valueColumn().setGroupSpacingWidth(groupSpacingWidth) )
        return;
    updateViewByWidth();
}


void KByteArrayView::setBinaryGapWidth( int/*KPixelX*/ binaryGapWidth )
{
    if( !valueColumn().setBinaryGapWidth(binaryGapWidth) )
        return;
    updateViewByWidth();
}


void KByteArrayView::setSubstituteChar( QChar substituteChar )
{
    if( !charColumn().setSubstituteChar(substituteChar) )
        return;
    pauseCursor();
    updateColumn( charColumn() );
    unpauseCursor();
}

void KByteArrayView::setUndefinedChar( QChar undefinedChar )
{
    if( !charColumn().setUndefinedChar(undefinedChar) )
        return;
    pauseCursor();
    updateColumn( charColumn() );
    unpauseCursor();
}

void KByteArrayView::setShowsNonprinting( bool showingNonprinting )
{
    if( !charColumn().setShowingNonprinting(showingNonprinting) )
        return;
    pauseCursor();
    updateColumn( charColumn() );
    unpauseCursor();
}


void KByteArrayView::setEncoding( KEncoding charEncoding )
{
    if( mCharEncoding == charEncoding )
        return;

    KHECore::KCharCodec *newCharCodec
        = KHECore::KCharCodec::createCodec( (KHECore::KEncoding)charEncoding );
    if( newCharCodec == 0 )
        return;

    valueColumn().setCharCodec( newCharCodec );
    charColumn().setCharCodec( newCharCodec );

    delete mCharCodec;
    mCharCodec = newCharCodec;
    mCharEncoding = charEncoding;

    pauseCursor();
    updateColumn( valueColumn() );
    updateColumn( charColumn() );
    unpauseCursor();

    emit charCodecChanged( newCharCodec->name() );
}

// TODO: join with function above!
void KByteArrayView::setEncoding( const QString &encodingName )
{
    if( encodingName == mCharCodec->name() )
        return;

    KHECore::KCharCodec *newCharCodec =
        KHECore::KCharCodec::createCodec( encodingName );
    if( newCharCodec == 0 )
        return;

    valueColumn().setCharCodec( newCharCodec );
    charColumn().setCharCodec( newCharCodec );

    delete mCharCodec;
    mCharCodec = newCharCodec;
    mCharEncoding = LocalEncoding; // TODO: add encoding no to every known codec

    pauseCursor();
    updateColumn( valueColumn() );
    updateColumn( charColumn() );
    unpauseCursor();

    emit charCodecChanged( newCharCodec->name() );
}

void KByteArrayView::setByteTypeColored( bool isColored )
{
    if( isColored == valueColumn().isByteTypeColored() )
        return;

    valueColumn().setByteTypeColored( isColored );
    charColumn().setByteTypeColored( isColored );

    pauseCursor();
    updateColumn( valueColumn() );
    updateColumn( charColumn() );
    unpauseCursor();
}


void KByteArrayView::fontChange( const QFont &oldFont )
{
    ColumnsView::fontChange( oldFont );

    if( !mInZooming )
    {
        mDefaultFontSize = font().pointSize();
        mZoomLevel = 1.0;
    }

    // get new values
    const QFontMetrics newFontMetrics = fontMetrics();
    KPixelX digitWidth = newFontMetrics.maxWidth();
    KPixelY digitBaseLine = newFontMetrics.ascent();

    setLineHeight( newFontMetrics.height() );

    // update all dependant structures
    mDataLayout->setNoOfLinesPerPage( noOfLinesPerPage() );

    mOffsetColumn->setMetrics( digitWidth, digitBaseLine );
    valueColumn().setMetrics( digitWidth, digitBaseLine );
    charColumn().setMetrics( digitWidth, digitBaseLine );

    updateViewByWidth();
}


void KByteArrayView::updateViewByWidth()
{
    pauseCursor();

    adjustToLayoutNoOfBytesPerLine();
    adjustLayoutToSize();

    viewport()->update();

    mDataCursor->updateCoord();
    ensureCursorVisible();

    unpauseCursor();
    emit cursorPositionChanged( mDataCursor->realIndex() );
}


void KByteArrayView::zoomIn()         { zoomIn( DefaultZoomStep ); }
void KByteArrayView::zoomOut()        { zoomOut( DefaultZoomStep ); }

void KByteArrayView::zoomIn( int pointIncrement )
{
    QFont newFont( font() );
    int newPointSize = QFontInfo(newFont).pointSize() + pointIncrement;
    if( newPointSize > MaxFontPointSize )
        newPointSize = MaxFontPointSize;

    mZoomLevel = (double)newPointSize/mDefaultFontSize;
    newFont.setPointSize( newPointSize );

    mInZooming = true;
    setFont( newFont );
    mInZooming = false;
}

void KByteArrayView::zoomOut( int pointDecrement )
{
    QFont newFont( font() );
    int newPointSize = QFontInfo(newFont).pointSize() - pointDecrement;
    if( newPointSize < MinFontPointSize )
        newPointSize = MinFontPointSize;

    mZoomLevel = (double)newPointSize/mDefaultFontSize;
    newFont.setPointSize( newPointSize );

    mInZooming = true;
    setFont( newFont );
    mInZooming = false;
}


void KByteArrayView::zoomTo( int newPointSize )
{
    if( newPointSize < MinFontPointSize )
        newPointSize = MinFontPointSize;
    else if( newPointSize > MaxFontPointSize )
        newPointSize = MaxFontPointSize;

    QFont newFont( font() );
    if( QFontInfo(newFont).pointSize() == newPointSize )
        return;

    newFont.setPointSize( newPointSize );
    mZoomLevel = (double)newPointSize/mDefaultFontSize;

    mInZooming = true;
    setFont( newFont );
    mInZooming = false;
}


void KByteArrayView::unZoom()
{
    zoomTo( mDefaultFontSize );
}

void KByteArrayView::setZoomLevel( double zoomLevel )
{
    const int newPointSize = (int)(zoomLevel*mDefaultFontSize);
    const int currentPointSize = QFontInfo(font()).pointSize();

    // TODO: here we catch any new zoomlevels which are out of bounds and the zoom already at that bound
    if( (currentPointSize <= MinFontPointSize && zoomLevel < (double)MinFontPointSize/mDefaultFontSize)
        || (MaxFontPointSize <= currentPointSize && (double)MaxFontPointSize/mDefaultFontSize < zoomLevel) )
        return;

    zoomTo( newPointSize );
    // TODO: this hack overwrites the new zoomlevel calculated from the integers in zoomTo,
    // to avoid getting trapped inside a small integer value, if the zoom tool operates relatively
    // think about, if this is the right approach
    mZoomLevel = zoomLevel;
}


void KByteArrayView::adjustLayoutToSize()
{
    // check whether there is a change with the numbers of fitting bytes per line
    if( mResizeStyle != NoResize )
    {
        // changes?
        if( mDataLayout->setNoOfBytesPerLine(fittingBytesPerLine()) )
            adjustToLayoutNoOfBytesPerLine();
    }

    setNoOfLines( mDataLayout->noOfLines() );
}


void KByteArrayView::adjustToLayoutNoOfBytesPerLine()
{
    mOffsetColumn->setDelta( mDataLayout->noOfBytesPerLine() );
    valueColumn().resetXBuffer();
    charColumn().resetXBuffer();

    updateWidths();
}


void KByteArrayView::setNoOfLines( int newNoOfLines )
{
    ColumnsView::setNoOfLines( newNoOfLines>1?newNoOfLines:1 );
}


void KByteArrayView::toggleOffsetColumn( bool showOffsetColumn )
{
    const bool isVisible = mOffsetColumn->isVisible();
    // no change?
    if( isVisible == showOffsetColumn )
        return;

    mOffsetColumn->setVisible( showOffsetColumn );
    mFirstBorderColumn->setVisible( showOffsetColumn );

    updateViewByWidth();
}


QSize KByteArrayView::sizeHint() const
{
    return QSize( columnsWidth(), columnsHeight() );
}


QSize KByteArrayView::minimumSizeHint() const
{
    // TODO: better minimal width (visibility!)
    return QSize(
        mOffsetColumn->visibleWidth()
        + mFirstBorderColumn->visibleWidth()
        + mSecondBorderColumn->visibleWidth()
        + valueColumn().byteWidth()
        + charColumn().byteWidth(),
        lineHeight()
        + noOfLines()>1? style()->pixelMetric(QStyle::PM_ScrollBarExtent):0 );
}


void KByteArrayView::resizeEvent( QResizeEvent *resizeEvent )
{
    if( mResizeStyle != NoResize )
    {
        // changes?
        if( mDataLayout->setNoOfBytesPerLine(fittingBytesPerLine()) )
        {
            setNoOfLines( mDataLayout->noOfLines() );
            updateViewByWidth();
        }
    }

    ColumnsView::resizeEvent( resizeEvent );

    mDataLayout->setNoOfLinesPerPage( noOfLinesPerPage() ); // TODO: doesn't work with the new size!!!
}


int KByteArrayView::fittingBytesPerLine() const
{
    const QSize newSize = maximumViewportSize();
    const KPixelX reservedWidth =
        mOffsetColumn->visibleWidth()
        + mFirstBorderColumn->visibleWidth()
        + mSecondBorderColumn->visibleWidth();

    // abstract offset and border columns width
    const KPixelX fullWidth = newSize.width() - reservedWidth;

    //  // no width left for resizeable columns? TODO: put this in resizeEvent
    //  if( fullWidth < 0 )
    //    return;

    const KPixelY fullHeight = newSize.height();

    // check influence of dis-/appearing of the vertical scrollbar
    const bool verticalScrollbarIsVisible = verticalScrollBar()->isVisible();
    const KPixelX scrollbarExtent = style()->pixelMetric( QStyle::PM_ScrollBarExtent );

    KPixelX availableWidth = fullWidth;
    if( verticalScrollbarIsVisible )
        availableWidth -= scrollbarExtent;

    enum KMatchTrial { FirstRun, RerunWithScrollbarOn, TestWithoutScrollbar };
    KMatchTrial matchRun = FirstRun;

    // prepare needed values
    const KPixelX digitWidth = valueColumn().digitWidth();
    const KPixelX charByteWidth = charColumn().isVisible() ? digitWidth : 0;
    const KPixelX valueByteWidth = valueColumn().isVisible() ? valueColumn().byteWidth() : 0;
    const KPixelX byteSpacingWidth = valueColumn().isVisible() ? valueColumn().byteSpacingWidth() : 0;
    KPixelX groupSpacingWidth;
    int noOfGroupedBytes = valueColumn().noOfGroupedBytes();
    // no grouping?
    if( noOfGroupedBytes == 0 )
    {
        // faking grouping by 1
        noOfGroupedBytes = 1;
        groupSpacingWidth = 0;
    }
    else
        groupSpacingWidth = valueColumn().isVisible() ? valueColumn().groupSpacingWidth() : 0;

    const KPixelX valueByteGroupWidth =  noOfGroupedBytes * valueByteWidth + (noOfGroupedBytes-1)*byteSpacingWidth;
    const KPixelX charByteGroupWidth = noOfGroupedBytes * charByteWidth;
    const KPixelX totalGroupWidth = valueByteGroupWidth + groupSpacingWidth + charByteGroupWidth;

    int fittingBytesPerLine;
    int fittingBytesPerLineWithScrollbar = 0;
    for(;;)
    {
    //    kDebug() << "matchWidth: " << fullWidth
    //              << " (v:" << visibleWidth()
    //              << ", f:" << frameWidth()
    //              << ", A:" << availableWidth
    //              << ", S:" << scrollbarExtent
    //              << ", R:" << reservedWidth << ")" << endl;

        // calculate fitting groups per line
        const int fittingGroupsPerLine = (availableWidth+groupSpacingWidth) // fake spacing after last group
                                          / totalGroupWidth;

        // calculate the fitting bytes per line by groups
        fittingBytesPerLine = noOfGroupedBytes * fittingGroupsPerLine;

        // not only full groups?
        if( mResizeStyle == FullSizeUsage && noOfGroupedBytes > 1 )
        {
            if( fittingGroupsPerLine > 0 )
                availableWidth -= fittingGroupsPerLine*totalGroupWidth; // includes additional spacing after last group

//         kDebug() << "Left: " << availableWidth << "("<<valueByteWidth<<", "<<charByteWidth<<")" ;

            if( availableWidth > 0 )
                fittingBytesPerLine += (availableWidth+byteSpacingWidth) / (valueByteWidth+byteSpacingWidth+charByteWidth);

            // is there not even the space for a single byte?
            if( fittingBytesPerLine == 0 )
            {
                // ensure at least one byte per line
                fittingBytesPerLine = 1;
                // and
                break;
            }
        }
        // is there not the space for a single group?
        else if( fittingBytesPerLine == 0 )
        {
            // ensures at least one group
            fittingBytesPerLine = noOfGroupedBytes;
            break;
        }

//    kDebug() << "meantime: " << fittingGroupsPerLine << " (T:" << totalGroupWidth
//              << ", h:" << valueByteGroupWidth
//              << ", t:" << charByteGroupWidth
//              << ", s:" << groupSpacingWidth << ") " <<fittingBytesPerLine<< endl;

        const int newNoOfLines = (mDataLayout->length()+mDataLayout->startOffset()+fittingBytesPerLine-1)
                                 / fittingBytesPerLine;
        const KPixelY newHeight =  newNoOfLines * lineHeight();

        if( verticalScrollbarIsVisible )
        {
            if( matchRun == TestWithoutScrollbar )
            {
                // did the test without the scrollbar fail, don't the data fit into the view?
                if( newHeight>fullHeight )
                    // reset to old calculated value
                    fittingBytesPerLine =  fittingBytesPerLineWithScrollbar;
                break;
            }

            // a chance for to perhaps fit in height?
            if( fittingBytesPerLine <= mDataLayout->noOfBytesPerLine() )
            {
                // remember this trial's result and calc number of bytes with vertical scrollbar on
                fittingBytesPerLineWithScrollbar = fittingBytesPerLine;
                availableWidth = fullWidth;
                matchRun = TestWithoutScrollbar;
        //          kDebug() << "tested without scrollbar..." ;
                continue;
            }
        }
        else
        {
            // doesn't it fit into the height anymore?
            if( newHeight>fullHeight && matchRun==FirstRun )
            {
                // need for a scrollbar has risen... ->less width, new calculation
                availableWidth = fullWidth - scrollbarExtent;
                matchRun = RerunWithScrollbarOn;
        //          kDebug() << "rerun with scrollbar on..." ;
                continue;
            }
        }

        break;
    }

    return fittingBytesPerLine;
}


bool KByteArrayView::selectWord( /*unsigned TODO:change all unneeded signed into unsigned!*/ int index )
{
    bool result = false;
    if( index >= 0 && index < mDataLayout->length()  )
    {
        const KHECore::KWordBufferService WBS( mByteArrayModel, mCharCodec );
        const KHE::KSection wordSection = WBS.wordSection( index );
        if( wordSection.isValid() )
        {
            pauseCursor();
            mValueEditor->finishEdit();

            mDataRanges->setFirstWordSelection( wordSection );
            mDataCursor->gotoIndex( wordSection.nextBehindEnd() );
            updateChanged();

            unpauseCursor();
            emit cursorPositionChanged( mDataCursor->realIndex() );
            result = true;
        }
    }
    return result;
}


void KByteArrayView::selectAll( bool select )
{
    pauseCursor();
    mValueEditor->finishEdit();

    if( select )
    {
        mDataRanges->setSelection( KHE::KSection(0,mDataLayout->length()-1) );
        mDataCursor->gotoEnd();
    }
    else
        mDataRanges->removeSelection();

    updateChanged();

    unpauseCursor();

    if( !mOverWrite ) emit cutAvailable( mDataRanges->hasSelection() );
    emit copyAvailable( mDataRanges->hasSelection() );
    emit selectionChanged( mDataRanges->hasSelection() );
    emit cursorPositionChanged( mDataCursor->realIndex() );
    viewport()->setCursor( isReadOnly() ? Qt::ArrowCursor : Qt::IBeamCursor );
}


bool KByteArrayView::hasSelectedData() const
{
    return mDataRanges->hasSelection();
}


QByteArray KByteArrayView::selectedData() const
{
    if( !mDataRanges->hasSelection() )
        return QByteArray();

    const KHE::KSection selection = mDataRanges->selection();
    QByteArray data;
    data.resize( selection.width() );
    mByteArrayModel->copyTo( data.data(), selection.start(), selection.width() );
    return data;
}


QMimeData *KByteArrayView::selectionAsMimeData() const
{
    if( !mDataRanges->hasSelection() )
        return 0;

    QMimeData *mimeData = new QMimeData;
    mimeData->setData( OctetStreamFormatName, selectedData() );
    return mimeData;
}


void KByteArrayView::cut()
{
    if( isReadOnly() || mOverWrite )
        return;

    QMimeData *cutData = selectionAsMimeData();
    if( !cutData )
        return;

    QApplication::clipboard()->setMimeData( cutData, mClipboardMode );

    removeSelectedData();
}


void KByteArrayView::copy()
{
    QMimeData *cutData = selectionAsMimeData();
    if( !cutData )
        return;

    QApplication::clipboard()->setMimeData( cutData, mClipboardMode );
}


void KByteArrayView::paste()
{
    if( isReadOnly() )
        return;

    const QMimeData *data = QApplication::clipboard()->mimeData( mClipboardMode );
    pasteData( data );
}


void KByteArrayView::pasteData( const QMimeData *data )
{
    if( !data )
        return;

    // if there is a octet stream, use it, otherwise take the dump of the format
    // with the highest priority
    // TODO: this may not be, what is expected, think about it, if we just
    // take byte array descriptions, like encodings in chars or values
    // would need the movement of the encoders into the core library
    const QLatin1String octetStreamFormatName( OctetStreamFormatName );
    const QString dataFormatName = ( data->hasFormat(octetStreamFormatName) ) ?
        QString( octetStreamFormatName ) :
        data->formats()[0];

    const QByteArray byteArray = data->data( dataFormatName );

    if( !byteArray.isEmpty() )
        insert( byteArray );
}

bool KByteArrayView::canReadData( const QMimeData *data ) const
{
Q_UNUSED( data )
    // taking all for now, see comment in pasteData above
    return true;//data->hasFormat( OctetStreamFormatName );
}


void KByteArrayView::insert( const QByteArray &data )
{
    if( mOverWrite )
    {
        int lengthOfInserted;
        if( mDataRanges->hasSelection() )
        {
            // replacing the selection:
            // we restrict the replacement to the minimum length of selection and input
            KHE::KSection selection = mDataRanges->removeSelection();
            selection.restrictEndByWidth( data.size() );
            lengthOfInserted = mByteArrayModel->replace( selection, data.data(), selection.width() );
        }
        else
        {
            const int length = mDataLayout->length();
            if( !mDataCursor->isBehind() && length > 0 )
            {
                // replacing the normal data, at least until the end
                KHE::KSection insertRange = KHE::KSection::fromWidth( mDataCursor->realIndex(), data.size() );
                insertRange.restrictEndTo( length-1 );
                lengthOfInserted = mByteArrayModel->replace( insertRange, data.data(), insertRange.width() );
            }
            else
                lengthOfInserted = 0;
        }
        // if inserting ourself we want to place the cursor at the end of the inserted data
        if( lengthOfInserted > 0 )
        {
            pauseCursor();
            mDataCursor->gotoNextByte(lengthOfInserted);
            unpauseCursor();
            emit cursorPositionChanged( mDataCursor->realIndex() );
        }
    }
    else
    {
        if( mDataRanges->hasSelection() )
        {
            // replacing the selection
            const KHE::KSection selection = mDataRanges->removeSelection();
            mByteArrayModel->replace( selection, data );
        }
        else
        mByteArrayModel->insert( mDataCursor->realIndex(), data );
    }

    emit selectionChanged( mDataRanges->hasSelection() );
}


void KByteArrayView::removeSelectedData()
{
    // Can't we do this?
    if( isReadOnly() || mOverWrite || mValueEditor->isInEditMode() )
        return;

    const KHE::KSection selection = mDataRanges->removeSelection();

    mByteArrayModel->remove( selection );

//     clearUndoRedo();

  //emit selectionChanged( -1, -1 );
}


void KByteArrayView::onContentsChanged( const KHE::ArrayChangeMetricsList &changeList )
{
    pauseCursor();

    const bool appending = mDataCursor->atAppendPos();
    const int oldLength = mDataLayout->length(); // TODO: hack for mDataCursor->adaptSelectionToChange
    // update lengths
    int oldNoOfLines = noOfLines();
    mDataLayout->setLength( mByteArrayModel->size() );
    int newNoOfLines = mDataLayout->noOfLines();
    if( oldNoOfLines != newNoOfLines )
    {
        setNoOfLines( newNoOfLines );
        updateColumn( *mOffsetColumn );
    }

    // adapt cursor(s)
    if( appending )
        mDataCursor->gotoEnd();
    else
        mDataCursor->adaptToChanges( changeList, oldLength );

    mDataRanges->adaptToChanges( changeList );
    // kDebug() << "Cursor:"<<mDataCursor->index()<<", selection:"<<mDataRanges->selectionStart()<<"-"<<mDataRanges->selectionEnd()
    //          <<", BytesPerLine: "<<mDataLayout->noOfBytesPerLine()<<endl;

    ensureCursorVisible();
    updateChanged();
    unpauseCursor();

    emit cursorPositionChanged( mDataCursor->realIndex() );
}


void KByteArrayView::onBookmarksChange( const QList<KHECore::KBookmark> &bookmarks )
{

    foreach( const KHECore::KBookmark &bookmark, bookmarks )
    {
        const int position = bookmark.offset();
        mDataRanges->addChangedRange( position, position );
    }

    unpauseCursor();
    updateChanged();
}


void KByteArrayView::onRevertedToVersionIndex( int versionIndex )
{
Q_UNUSED( versionIndex )
    mValueEditor->finishEdit();
}


void KByteArrayView::clipboardChanged()
{
    // don't listen to selection changes
    disconnect( QApplication::clipboard(), SIGNAL(selectionChanged()) );
    selectAll( false );
}


void KByteArrayView::setCursorPosition( int index, bool behind )
{
    pauseCursor();
    mValueEditor->finishEdit();

    if( behind ) --index;
    mDataCursor->gotoCIndex( index );
    if( behind )
        mDataCursor->stepBehind();

    mDataRanges->removeSelection();
    if( mDataRanges->isModified() )
    {
        updateChanged();

        viewport()->setCursor( isReadOnly() ? Qt::ArrowCursor : Qt::IBeamCursor );

        if( !mOverWrite ) emit cutAvailable( mDataRanges->hasSelection() );
        emit copyAvailable( mDataRanges->hasSelection() );
        emit selectionChanged( mDataRanges->hasSelection() );
    }
    ensureCursorVisible();

    unpauseCursor();
    emit cursorPositionChanged( mDataCursor->realIndex() );
}

void KByteArrayView::setSelection( int start, int end )
{
    if( start >= 0 && end < mDataLayout->length()  )
    {
        const KHE::KSection selection( start, end );
        if( selection.isValid() )
        {
            pauseCursor();
            mValueEditor->finishEdit();

            mDataRanges->setSelection( selection );
            mDataCursor->gotoCIndex( selection.nextBehindEnd() );
            ensureVisible( activeColumn(), mDataLayout->coordOfIndex(selection.start()) );
            ensureCursorVisible();
            updateChanged();

            unpauseCursor();
            emit cursorPositionChanged( mDataCursor->realIndex() );
        }
    }
}

void KByteArrayView::setVisibleByteArrayColumns( int newColumns )
{
    const int oldColumns = visibleBufferColumns();

    // no changes or no column selected?
    if( newColumns == oldColumns || !(newColumns&( ValueColumnId | CharColumnId )) )
        return;

    valueColumn().setVisible( ValueColumnId & newColumns );
    charColumn().setVisible( CharColumnId & newColumns );
    mSecondBorderColumn->setVisible( newColumns == (ValueColumnId|CharColumnId) );

    // active column not visible anymore?
    if( !activeColumn().isVisible() )
    {
        AbstractByteArrayColumnRenderer *h = mActiveColumn;
        mActiveColumn = mInactiveColumn;
        mInactiveColumn = h;
        adaptController();
    }

    updateViewByWidth();
}


void KByteArrayView::setCursorColumn( ByteArrayColumnId columnId )
{
    // no changes or not visible?
    if( columnId == cursorColumn()
        || (columnId == ValueColumnId && !valueColumn().isVisible())
        || (columnId == CharColumnId && !charColumn().isVisible()) )
        return;

    pauseCursor();
    mValueEditor->finishEdit();

    if( columnId == ValueColumnId )
    {
        mActiveColumn = &valueColumn();
        mInactiveColumn = &charColumn();
    }
    else
    {
        mActiveColumn = &charColumn();
        mInactiveColumn = &valueColumn();
    }
    adaptController();

    ensureCursorVisible();
    unpauseCursor();
}


void KByteArrayView::placeCursor( const QPoint &point )
{
    // switch active column if needed
    if( charColumn().isVisible() && point.x() >= charColumn().x() )
    {
        mActiveColumn = &charColumn();
        mInactiveColumn = &valueColumn();
    }
    else
    {
        mActiveColumn = &valueColumn();
        mInactiveColumn = &charColumn();
    }
    adaptController();

    // get coord of click and whether this click was closer to the end of the pos
    const Coord coord( activeColumn().magneticLinePositionOfX(point.x()), lineAt(point.y()) );

    mDataCursor->gotoCCoord( coord );
    emit cursorPositionChanged( mDataCursor->realIndex() );
}


int KByteArrayView::indexByPoint( const QPoint &point ) const
{
    const AbstractByteArrayColumnRenderer *column =
         ( charColumn().isVisible() && point.x() >= charColumn().x() ) ?
         (AbstractByteArrayColumnRenderer *)&charColumn() : (AbstractByteArrayColumnRenderer *)&valueColumn();

    const Coord coord( column->linePositionOfX(point.x()), lineAt(point.y()) );

    return mDataLayout->indexAtCCoord( coord );
}


void KByteArrayView::showEvent( QShowEvent *Event )
{
    ColumnsView::showEvent( Event );
    mDataLayout->setNoOfLinesPerPage( noOfLinesPerPage() );
}


void KByteArrayView::focusInEvent( QFocusEvent *focusEvent )
{
    ColumnsView::focusInEvent( focusEvent );
    startCursor();
}

void KByteArrayView::focusOutEvent( QFocusEvent *focusEvent )
{
    stopCursor();
    ColumnsView::focusOutEvent( focusEvent );
}


void KByteArrayView::blinkCursor()
{
    // skip the cursor drawing?
    if( mCursorPaused || mValueEditor->isInEditMode() )
        return;

    // switch the cursor state
    mBlinkCursorVisible = !mBlinkCursorVisible;
    updateCursor( activeColumn() );
}


void KByteArrayView::startCursor()
{
    mCursorPaused = false;

    updateCursors();

    mCursorBlinkTimer->start( QApplication::cursorFlashTime()/2 );
}


void KByteArrayView::unpauseCursor()
{
    mCursorPaused = false;

    if( mCursorBlinkTimer->isActive() )
        updateCursors();
}


void KByteArrayView::updateCursors()
{
    createCursorPixmaps();

    mBlinkCursorVisible = true;
    updateCursor( activeColumn() );
    updateCursor( inactiveColumn() );
}


void KByteArrayView::stopCursor()
{
    mCursorBlinkTimer->stop();

    pauseCursor();
}


void KByteArrayView::pauseCursor()
{
    mCursorPaused = true;

    mBlinkCursorVisible = false;
    updateCursor( activeColumn() );
    updateCursor( inactiveColumn() );
}


void KByteArrayView::updateCursor( const AbstractByteArrayColumnRenderer &column )
{
    const int x = column.xOfLinePosition( mDataCursor->pos() ) - xOffset();
    const int y = lineHeight() * mDataCursor->line() - yOffset();
    const int w = column.byteWidth();

    viewport()->update( x,y, w,lineHeight() );
}

void KByteArrayView::createCursorPixmaps()
{
    // create mCursorPixmaps
    mCursorPixmaps->setSize( activeColumn().byteWidth(), lineHeight() );

    const int index = mDataCursor->validIndex();

    QPainter painter;
    painter.begin( &mCursorPixmaps->offPixmap() );
    painter.initFrom( this );
    activeColumn().renderByte( &painter, index );
    painter.end();

    painter.begin( &mCursorPixmaps->onPixmap() );
    painter.initFrom( this );
    activeColumn().renderCursor( &painter, index );
    painter.end();

    // calculat the shape
    KPixelX cursorX;
    KPixelX cursorW;
    if( mDataCursor->isBehind() )
    {
        cursorX = qMax( 0, mCursorPixmaps->onPixmap().width()-InsertCursorWidth );
        cursorW = InsertCursorWidth;
    }
    else
    {
        cursorX = 0;
        cursorW = mOverWrite ? -1 : InsertCursorWidth;
    }
    mCursorPixmaps->setShape( cursorX, cursorW );
}


void KByteArrayView::drawActiveCursor( QPainter *painter )
{
    // any reason to skip the cursor drawing?
    if( mBlinkCursorVisible && !hasFocus() && !viewport()->hasFocus() && !mDropper->isActive() )
        return;

    const int x = activeColumn().xOfLinePosition( mDataCursor->pos() );
    const int y = lineHeight() * mDataCursor->line();

    painter->translate( x, y );

    // paint edited byte?
    if( mValueEditor->isInEditMode() )
    {
        const int index = mDataCursor->index();

        if( mBlinkCursorVisible )
            valueColumn().renderEditedByte( painter, mValueEditor->value(), mValueEditor->valueAsString() );
        else
            valueColumn().renderByte( painter, index );
    }
    else
        painter->drawPixmap( mCursorPixmaps->cursorX(), 0,
                            mBlinkCursorVisible?mCursorPixmaps->onPixmap():mCursorPixmaps->offPixmap(),
                            mCursorPixmaps->cursorX(),0,mCursorPixmaps->cursorW(),-1 );

    painter->translate( -x, -y );
}


void KByteArrayView::drawInactiveCursor( QPainter *painter )
{
    // any reason to skip the cursor drawing?
    if( !inactiveColumn().isVisible()
        || mCursorPaused
        || (!mCursorPaused && !hasFocus() && !viewport()->hasFocus() && !mDropper->isActive())  )
        return;

    const int index = mDataCursor->validIndex();

    const int x = inactiveColumn().xOfLinePosition( mDataCursor->pos() );
    const int y = lineHeight() * mDataCursor->line();

    painter->translate( x, y );

    const AbstractByteArrayColumnRenderer::FrameStyle frameStyle =
        mDataCursor->isBehind() ?                    AbstractByteArrayColumnRenderer::Right :
        (mOverWrite||mValueEditor->isInEditMode()) ? AbstractByteArrayColumnRenderer::Frame :
                                                     AbstractByteArrayColumnRenderer::Left;
    inactiveColumn().renderFramedByte( painter, index, frameStyle );

    painter->translate( -x, -y );
}


void KByteArrayView::renderColumns( QPainter *painter, int cx, int cy, int cw, int ch )
{
    ColumnsView::renderColumns( painter, cx, cy, cw, ch );
    // TODO: update non blinking cursors. Should this perhaps be done in the buffercolumn?
    // Then it needs to know about inactive, insideByte and the like... well...
    // perhaps subclassing the buffer columns even more, to CharByteArrayColumnRenderer and ValueByteArrayColumnRenderer?

    if( visibleLines(KPixelYs::fromWidth(cy,ch)).includes(mDataCursor->line()) )
    {
        drawActiveCursor( painter );
        drawInactiveCursor( painter );
    }
}

void KByteArrayView::updateColumn( ColumnRenderer &column )
{
    if( column.isVisible() )
        viewport()->update( column.x()-xOffset(), 0, column.width(), visibleHeight() );
}

void KByteArrayView::emitSelectionSignals()
{
    const bool hasSelection = mDataRanges->hasSelection();
    if( !mOverWrite ) emit cutAvailable( hasSelection );
    emit copyAvailable( hasSelection );
    emit selectionChanged( hasSelection );
}


void KByteArrayView::keyPressEvent( QKeyEvent *keyEvent )
{
    if( !mController->handleKeyPress( keyEvent ) )
        ColumnsView::keyPressEvent( keyEvent );
}

bool KByteArrayView::event( QEvent *event )
{
    if( event->type() == QEvent::KeyPress )
    {
        QKeyEvent *keyEvent = (QKeyEvent *)event;
        if( keyEvent->key() == Qt::Key_Tab || keyEvent->key() == Qt::Key_Backtab )
        {
            keyPressEvent( keyEvent );
            if( keyEvent->isAccepted() )
                return true;
        }
    }

    return ColumnsView::event( event );
}


void KByteArrayView::updateChanged()
{
    const KPixelXs Xs = KPixelXs::fromWidth( xOffset(), visibleWidth() );

    // collect affected buffer columns
    QList<AbstractByteArrayColumnRenderer*> dirtyColumns;

    AbstractByteArrayColumnRenderer *column = mValueColumn;
    while( true )
    {
        if( column->isVisible() && column->overlaps(Xs) )
        {
            dirtyColumns.append( column );
            column->prepareRendering( Xs );
        }

        if( column == mCharColumn )
            break;
        column = mCharColumn;
    }

    // any colums to paint?
    if( dirtyColumns.size() > 0 )
    {
        KPixelYs Ys = KPixelYs::fromWidth( yOffset(), visibleHeight() );

        // calculate affected lines/indizes
        const KHE::KSection fullPositions( 0, mDataLayout->noOfBytesPerLine()-1 );
        CoordRange visibleRange( fullPositions, visibleLines(Ys) );

        CoordRange changedRange;
        // as there might be multiple selections on this line redo until no more is changed
        while( hasChanged(visibleRange,&changedRange) )
        {
            KPixelY cy = changedRange.start().line() * lineHeight() - yOffset();

            QListIterator<AbstractByteArrayColumnRenderer*> columnIt( dirtyColumns );
            // only one line?
            if( changedRange.start().line() == changedRange.end().line() )
            {
                const KHE::KSection changedPositions( changedRange.start().pos(), changedRange.end().pos() );
                while( columnIt.hasNext() )
                {
                    const KPixelXs xPixels = columnIt.next()->xsOfLinePositionsInclSpaces( changedPositions );

                    viewport()->update( xPixels.start()-xOffset(), cy, xPixels.width(), lineHeight() );
                }
            }
            //
            else
            {
                // first line
                const KHE::KSection firstChangedPositions( changedRange.start().pos(), fullPositions.end() );
                while( columnIt.hasNext() )
                {
                    const KPixelXs XPixels = columnIt.next()->xsOfLinePositionsInclSpaces( firstChangedPositions );

                    viewport()->update( XPixels.start()-xOffset(), cy, XPixels.width(), lineHeight() );
                }

                // at least one full line?
                for( int l = changedRange.start().line()+1; l < changedRange.end().line(); ++l )
                {
                    cy += lineHeight();
                    columnIt.toFront();
                    while( columnIt.hasNext() )
                    {
                        const KPixelXs XPixels = columnIt.next()->xsOfLinePositionsInclSpaces( fullPositions );

                        viewport()->update( XPixels.start()-xOffset(), cy, XPixels.width(), lineHeight() );
                    }
                }
                // last line
                cy += lineHeight();
                columnIt.toFront();
                const KHE::KSection lastChangedPositions( fullPositions.start(), changedRange.end().pos() );
                while( columnIt.hasNext() )
                {
                    const KPixelXs XPixels = columnIt.next()->xsOfLinePositionsInclSpaces( lastChangedPositions );

                    viewport()->update( XPixels.start()-xOffset(), cy, XPixels.width(), lineHeight() );
                }
            }

            // continue the search at the overnext index
            visibleRange.setStart( changedRange.end()+1 ); //+2 ); TODO: currently bounding ranges are not merged
            if( !visibleRange.isValid() )
                break;
        }
    }

    mDataRanges->resetChangedRanges();
}


bool KByteArrayView::hasChanged( const CoordRange &visibleRange, CoordRange *changedRange ) const
{
    if( !mDataRanges->overlapsChanges(visibleRange,changedRange) )
        return false;

    changedRange->restrictTo( visibleRange );
    return true;
}


void KByteArrayView::ensureCursorVisible()
{
//   // Not visible or the user is dragging the window, so don't position to caret yet
//   if ( !isVisible() || isHorizontalSliderPressed() || isVerticalSliderPressed() )
//   {
//     d->ensureCursorVisibleInShowEvent = true;
//     return;
//   }
  //static const int Margin = 10;
    ensureVisible( activeColumn(), mDataCursor->coord() );
}

void KByteArrayView::ensureVisible( const AbstractByteArrayColumnRenderer &column, const Coord &coord )
{

    // TODO: add getCursorRect to BufferColumn
    const KPixelXs cursorXs = KPixelXs::fromWidth( column.xOfLinePosition(coord.pos()),
                                                   column.byteWidth() );

    const KPixelYs cursorYs = KPixelYs::fromWidth( lineHeight()*coord.line(), lineHeight() );

    const KPixelXs visibleXs = KPixelXs::fromWidth( xOffset(), visibleWidth() );
    const KPixelYs visibleYs = KPixelXs::fromWidth( yOffset(), visibleHeight() );

    horizontalScrollBar()->setValue( visibleXs.startForInclude(cursorXs) );
    verticalScrollBar()->setValue( visibleYs.startForInclude(cursorYs) );
}


void KByteArrayView::mousePressEvent( QMouseEvent *mouseEvent )
{
    pauseCursor();
    mValueEditor->finishEdit();

    // care about a left button press?
    if( mouseEvent->button() == Qt::LeftButton )
    {
        mMousePressed = true;

        // select whole line?
        if( mTrippleClickTimer->isActive()
            && (mouseEvent->globalPos()-mDoubleClickPoint).manhattanLength() < QApplication::startDragDistance() )
        {
            mTrippleClickTimer->stop();
            const int indexAtFirstDoubleClickLinePosition = mDataLayout->indexAtFirstLinePosition( mDoubleClickLine );
            mDataRanges->setSelectionStart( indexAtFirstDoubleClickLinePosition );
            mDataCursor->gotoIndex( indexAtFirstDoubleClickLinePosition );
            mDataCursor->gotoLineEnd();
            mDataRanges->setSelectionEnd( mDataCursor->realIndex() );
            updateChanged();

            unpauseCursor();
            emit cursorPositionChanged( mDataCursor->realIndex() );
            return;
        }

        const QPoint mousePoint = viewportToColumns( mouseEvent->pos() );

        // start of a drag perhaps?
        if( mDataRanges->hasSelection() && mDataRanges->selectionIncludes(indexByPoint( mousePoint )) )
        {
            mDragStartPossible = true;
            mDragStartTimer->start( QApplication::startDragTime() );
            mDragStartPoint = mousePoint;
        }
        else
        {
            placeCursor( mousePoint );
            ensureCursorVisible();

            const int realIndex = mDataCursor->realIndex();
            if( mDataRanges->selectionStarted() )
            {
                if( mouseEvent->modifiers() & Qt::SHIFT )
                    mDataRanges->setSelectionEnd( realIndex );
                else
                {
                    mDataRanges->removeSelection();
                    mDataRanges->setSelectionStart( realIndex );
                }
            }
            else // start of a new selection possible
            {
                mDataRanges->setSelectionStart( realIndex );

                if( !isReadOnly() && (mouseEvent->modifiers()&Qt::SHIFT) ) // TODO: why only for readwrite?
                    mDataRanges->setSelectionEnd( realIndex );
            }

            mDataRanges->removeFurtherSelections();
        }
    }
    else if( mouseEvent->button() == Qt::MidButton )
        mDataRanges->removeSelection();

    if( mDataRanges->isModified() )
    {
        updateChanged();
        viewport()->setCursor( isReadOnly() ? Qt::ArrowCursor : Qt::IBeamCursor );
    }

    unpauseCursor();
}


void KByteArrayView::mouseMoveEvent( QMouseEvent *mouseEvent )
{
    const QPoint movePoint = viewportToColumns( mouseEvent->pos() );

    if( mMousePressed )
    {
        if( mDragStartPossible )
        {
            mDragStartTimer->stop();
            // moved enough for a drag?
            if( (movePoint-mDragStartPoint).manhattanLength() > QApplication::startDragDistance() )
                startDrag();
            if( !isReadOnly() )
                viewport()->setCursor( Qt::IBeamCursor );
            return;
        }
        // selecting
        handleMouseMove( movePoint );
    }
    else if( !isReadOnly() )
    {
        // visual feedback for possible dragging
        const bool InSelection =
            mDataRanges->hasSelection() && mDataRanges->selectionIncludes( indexByPoint(movePoint) );
        viewport()->setCursor( InSelection?Qt::ArrowCursor:Qt::IBeamCursor );
    }
}


void KByteArrayView::mouseReleaseEvent( QMouseEvent *mouseEvent )
{
    const QPoint releasePoint = viewportToColumns( mouseEvent->pos() );

    // this is not the release of a doubleclick so we need to process it?
    if( !mInDoubleClick )
    {
        const int line = lineAt( releasePoint.y() );
        const int pos = activeColumn().linePositionOfX( releasePoint.x() ); // TODO: can we be sure here about the active column?
        const int index = mDataLayout->indexAtCCoord( Coord(pos,line) ); // TODO: can this be another index than the one of the cursor???
        emit clicked( index );
    }

    if( mMousePressed )
    {
        mMousePressed = false;

        if( mScrollTimer->isActive() )
            mScrollTimer->stop();

        // was only click inside selection, nothing dragged?
        if( mDragStartPossible )
        {
            selectAll( false );
            mDragStartTimer->stop();
            mDragStartPossible = false;

            placeCursor( mDragStartPoint );
            ensureCursorVisible();

            unpauseCursor();
        }
        // was end of selection operation?
        else if( mDataRanges->hasSelection() )
        {
            if( QApplication::clipboard()->supportsSelection() )
            {
                mClipboardMode = QClipboard::Selection;
                disconnect( QApplication::clipboard(), SIGNAL(selectionChanged()) );

                copy();

                //TODO: why did we do this? And why does the disconnect above not work?
                // got connected multiple times after a few selections by mouse
        //         connect( QApplication::clipboard(), SIGNAL(selectionChanged()), SLOT(clipboardChanged()) );
                mClipboardMode = QClipboard::Clipboard;
            }
        }
    }
    // middle mouse button paste?
    else if( mouseEvent->button() == Qt::MidButton && !isReadOnly() )
    {
        pauseCursor();
        mValueEditor->finishEdit();

        placeCursor( releasePoint );

        // replace no selection?
        if( mDataRanges->hasSelection() && !mDataRanges->selectionIncludes(mDataCursor->index()) )
            mDataRanges->removeSelection();

        mClipboardMode = QClipboard::Selection;
        paste();
        mClipboardMode = QClipboard::Clipboard;

        // ensure selection changes to be drawn TODO: create a insert/pasteAtCursor that leaves out drawing
        updateChanged();

        ensureCursorVisible();
        unpauseCursor();
    }

    emit cursorPositionChanged( mDataCursor->realIndex() );

    mInDoubleClick = false;

    if( mDataRanges->selectionJustStarted() )
        mDataRanges->removeSelection();

    if( !mOverWrite ) emit cutAvailable( mDataRanges->hasSelection() );
    emit copyAvailable( mDataRanges->hasSelection() );
    emit selectionChanged( mDataRanges->hasSelection() );
}


// gets called after press and release instead of a plain press event (?)
void KByteArrayView::mouseDoubleClickEvent( QMouseEvent *mouseEvent )
{
    // we are only interested in LMB doubleclicks
    if( mouseEvent->button() != Qt::LeftButton )
    {
        mouseEvent->ignore();
        return;
    }

    mDoubleClickLine = mDataCursor->line();

    const int index = mDataCursor->validIndex();

    if( mActiveColumn == &charColumn() )
    {
        selectWord( index );

        // as we already have a doubleclick maybe it is a tripple click
        mTrippleClickTimer->start( qApp->doubleClickInterval() );
        mDoubleClickPoint = mouseEvent->globalPos();
    }
    //  else
    //    mValueEditor->goInsideByte(); TODO: make this possible again

    mInDoubleClick = true; //
    mMousePressed = true;

    emit doubleClicked( index );
}


void KByteArrayView::autoScrollTimerDone()
{
    if( mMousePressed )
        handleMouseMove( viewportToColumns(viewport()->mapFromGlobal( QCursor::pos() )) );
}


void KByteArrayView::handleMouseMove( const QPoint &point ) // handles the move of the mouse with pressed buttons
{
    // no scrolltimer and outside of viewport?
    if( !mScrollTimer->isActive() && (point.y()<yOffset() || yOffset()+visibleHeight()<=point.y()) )
        mScrollTimer->start( DefaultScrollTimerPeriod );
    // scrolltimer but inside of viewport?
    else if( mScrollTimer->isActive() && (yOffset() <= point.y() && point.y() < yOffset()+visibleHeight()) )
        mScrollTimer->stop();

    pauseCursor();

    placeCursor( point );
    ensureCursorVisible();

    // do wordwise selection?
    if( mInDoubleClick && mDataRanges->hasFirstWordSelection() )
    {
        int newIndex = mDataCursor->realIndex();
        const KHE::KSection firstWordSelection = mDataRanges->firstWordSelection();
        const KHECore::KWordBufferService WBS( mByteArrayModel, mCharCodec );
        // are we before the selection?
        if( firstWordSelection.startsBehind(newIndex) )
        {
            mDataRanges->ensureWordSelectionForward( false );
            newIndex = WBS.indexOfLeftWordSelect( newIndex );
        }
        // or behind?
        else if( firstWordSelection.endsBefore(newIndex) )
        {
            mDataRanges->ensureWordSelectionForward( true );
            newIndex = WBS.indexOfRightWordSelect( newIndex );
        }
        // or inside?
        else
        {
            mDataRanges->ensureWordSelectionForward( true );
            newIndex = firstWordSelection.nextBehindEnd();
        }

        mDataCursor->gotoIndex( newIndex );
    }

    if( mDataRanges->selectionStarted() )
        mDataRanges->setSelectionEnd( mDataCursor->realIndex() );

    updateChanged();

    unpauseCursor();
    emit cursorPositionChanged( mDataCursor->realIndex() );
}


void KByteArrayView::startDrag()
{
    // reset states
    mMousePressed = false;
    mInDoubleClick = false;
    mDragStartPossible = false;

    // create data
    QMimeData *dragData = selectionAsMimeData();
    if( !dragData )
        return;

    QDrag *drag = new QDrag( this );
    drag->setMimeData( dragData );

    Qt::DropActions request = (isReadOnly()||mOverWrite) ? Qt::CopyAction : Qt::CopyAction|Qt::MoveAction;
    Qt::DropAction dropAction = drag->exec( request );

    if( dropAction == Qt::MoveAction )
        // Not inside this widget itself?
        if( drag->target() != this )
            removeSelectedData();
}


void KByteArrayView::dragEnterEvent( QDragEnterEvent* dragEnterEvent )
{
    if( !mDropper->handleDragEnterEvent(dragEnterEvent) )
        dragEnterEvent->ignore();
}


void KByteArrayView::dragMoveEvent( QDragMoveEvent* dragMoveEvent )
{
    if( !mDropper->handleDragMoveEvent(dragMoveEvent) )
        dragMoveEvent->ignore();
}


void KByteArrayView::dragLeaveEvent( QDragLeaveEvent* dragLeaveEvent )
{
    if( !mDropper->handleDragLeaveEvent(dragLeaveEvent) )
        dragLeaveEvent->ignore();
}


void KByteArrayView::dropEvent( QDropEvent* dropEvent )
{
    if( !mDropper->handleDropEvent(dropEvent) )
        dropEvent->ignore();
}


void KByteArrayView::wheelEvent( QWheelEvent* wheelEvent )
{
    if( !mWheelController->handleWheelEvent(wheelEvent) )
        ColumnsView::wheelEvent( wheelEvent );
}


KByteArrayView::~KByteArrayView()
{
    delete mTabController;
    delete mNavigator;
    delete mValueEditor;
    delete mCharEditor;

    delete mDropper;

    delete mZoomWheelController;

    delete mDataCursor;
    delete mDataRanges;
    delete mDataLayout;

    delete mCharCodec;
    delete mCursorPixmaps;

    delete d;
}

}

#include "kbytearrayview.moc"
