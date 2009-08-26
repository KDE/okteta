/*
    This file is part of the Okteta Gui library, part of the KDE project.

    Copyright 2008-2009 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "abstractbytearrayview_p.h"

// lib
#include "controller/ktabcontroller.h"
#include "controller/knavigator.h"
#include "controller/kchareditor.h"
#include "controller/dropper.h"
#include "controller/zoomwheelcontroller.h"
// Okteta core
#include <valuecodec.h>
#include <bookmarkable.h>
#include <versionable.h>
#include <wordbytearrayservice.h>
#include <arraychangemetricslist.h>
#include <bookmark.h>
// KDE
#include <KGlobal>
// Qt
#include <QtGui/QKeyEvent>
#include <QtGui/QDragEnterEvent>
#include <QtGui/QDragMoveEvent>
#include <QtGui/QDragLeaveEvent>
#include <QtGui/QDropEvent>
#include <QtGui/QApplication>
#include <QtGui/QToolTip>
#include <QtCore/QMimeData>
#include <QtCore/QByteArray>


namespace Okteta
{

static const int DefaultStartOffset = 0;//5;
static const int DefaultFirstLineOffset = 0;
static const int DefaultNoOfBytesPerLine =  16;

// zooming is done in steps of font size points
static const int DefaultZoomStep = 1;
static const int MinFontPointSize = 4;
static const int MaxFontPointSize = 128;

static const AbstractByteArrayView::ValueCoding DefaultValueCoding =  AbstractByteArrayView::HexadecimalCoding;
static const AbstractByteArrayView::CharCoding DefaultCharCoding = AbstractByteArrayView::LocalEncoding;

static const AbstractByteArrayView::ResizeStyle DefaultResizeStyle = AbstractByteArrayView::FullSizeUsage;

static const char OctetStreamFormatName[] = "application/octet-stream";


class NullModel : public Okteta::AbstractByteArrayModel
{
  public:
    NullModel();
    virtual ~NullModel();

  public: // data access API
    virtual char datum( unsigned int offset ) const;
    virtual int size() const;

  public: // state read API
    virtual bool isModified() const;

  public: // modification API
    virtual unsigned int replace( const KDE::Section& removeSection, const char* insertData, unsigned int insertLength );
    virtual bool swap( int firstStart, const KDE::Section& secondSection );
    virtual int fill( const char fillChar, unsigned int offset = 0, int fillLength = -1 );
    virtual void setDatum( unsigned int offset, const char value );
    virtual void setModified( bool modified );
};

NullModel::NullModel() {}
NullModel::~NullModel() {}

char NullModel::datum( unsigned int offset ) const { Q_UNUSED(offset) return 0; }
int NullModel::size() const { return 0; }
bool NullModel::isModified() const { return false; }
unsigned int NullModel::replace( const KDE::Section& removeSection, const char* insertData, unsigned int insertLength )
{
Q_UNUSED(removeSection) Q_UNUSED(insertData) Q_UNUSED(insertLength)
    return 0;
}
bool NullModel::swap( int firstStart, const KDE::Section& secondSection )
{
Q_UNUSED(firstStart) Q_UNUSED(secondSection)
    return false;
}
int NullModel::fill( const char fillChar, unsigned int offset, int fillLength )
{
Q_UNUSED(fillChar) Q_UNUSED(offset) Q_UNUSED(fillLength)
    return 0;
}
void NullModel::setDatum( unsigned int offset, const char value )
{
Q_UNUSED(offset) Q_UNUSED(value)
}
void NullModel::setModified( bool modified )
{
Q_UNUSED(modified)
}

K_GLOBAL_STATIC( NullModel, nullModel )

AbstractByteArrayViewPrivate::AbstractByteArrayViewPrivate( AbstractByteArrayView* parent )
 : mByteArrayModel( nullModel ),
   mTableLayout( new ByteArrayTableLayout(DefaultNoOfBytesPerLine,DefaultFirstLineOffset,DefaultStartOffset,0,0) ),
   mTableCursor( new ByteArrayTableCursor(mTableLayout) ),
   mTableRanges( new ByteArrayTableRanges(mTableLayout) ),
   mReadOnly( false ),
   mOverWriteOnly( false ),
   mOverWrite( true ),
   mInZooming( false ),
   mCursorPaused( false ),
//    mDefaultFontSize( p->font().pointSize() ), crashes in font()
   mZoomLevel( 1.0 ),
   mClipboardMode( QClipboard::Clipboard ),
   mResizeStyle( DefaultResizeStyle ),
   q_ptr( parent )
{
}

void AbstractByteArrayViewPrivate::init()
{
    Q_Q( AbstractByteArrayView );

    // initialize layout
    mTableLayout->setLength( mByteArrayModel->size() );
    mTableLayout->setNoOfLinesPerPage( q->noOfLinesPerPage() );

    mValueCodec = Okteta::ValueCodec::createCodec( (Okteta::ValueCoding)DefaultValueCoding );
    mValueCoding = DefaultValueCoding;
    mCharCodec = Okteta::CharCodec::createCodec( (Okteta::CharCoding)DefaultCharCoding );
    mCharCoding = DefaultCharCoding;

    mTabController = new KTabController( q, 0 );
    mNavigator = new KNavigator( q, mTabController );
    mValueEditor = new KValueEditor( mTableCursor, q, mNavigator );
    mCharEditor = new KCharEditor( mTableCursor, q, mNavigator );
    mZoomWheelController = new ZoomWheelController( q, 0 );
    mDropper = new Dropper( q );

    setWheelController( mZoomWheelController );
}

void AbstractByteArrayViewPrivate::setByteArrayModel( Okteta::AbstractByteArrayModel* byteArrayModel )
{
    Q_Q( AbstractByteArrayView );

    mByteArrayModel->disconnect( q );
    mCursorPaused = true;

    mByteArrayModel = byteArrayModel ? byteArrayModel : nullModel;

    // affected:
    // length -> no of lines -> width
    mTableLayout->setLength( mByteArrayModel->size() );
    adjustLayoutToSize();

    // if the model is readonly make the view too, per default
    if( mByteArrayModel->isReadOnly() )
        setReadOnly( true );

    q->connect( mByteArrayModel, SIGNAL(readOnlyChanged( bool )),
                q, SLOT(onByteArrayReadOnlyChange( bool )) );
    q->connect( mByteArrayModel, SIGNAL(contentsChanged( const KDE::ArrayChangeMetricsList& )),
                q, SLOT(onContentsChanged( const KDE::ArrayChangeMetricsList& )) );

    Okteta::Bookmarkable *bookmarks = qobject_cast<Okteta::Bookmarkable*>( mByteArrayModel );
    if( bookmarks )
    {
        q->connect( mByteArrayModel, SIGNAL(bookmarksAdded( const QList<Okteta::Bookmark>& )),
                    q, SLOT(onBookmarksChange(const QList<Okteta::Bookmark>&)) );
        q->connect( mByteArrayModel, SIGNAL(bookmarksRemoved( const QList<Okteta::Bookmark>& )),
                    q, SLOT(onBookmarksChange(const QList<Okteta::Bookmark>&)) );
    }
    Okteta::Versionable* versionControl = qobject_cast<Okteta::Versionable*>( mByteArrayModel );
    if( versionControl )
    {
        q->connect( mByteArrayModel, SIGNAL(revertedToVersionIndex( int )),
                    q, SLOT(onRevertedToVersionIndex( int )) );
    }

    q->viewport()->update();

    mTableCursor->gotoStart();
    ensureCursorVisible();
    unpauseCursor();

    emit q->cursorPositionChanged( cursorPosition() );
}

void AbstractByteArrayViewPrivate::fontChange( const QFont& oldFont )
{
    Q_Q( AbstractByteArrayView );

    q->ColumnsView::fontChange( oldFont );

    if( !mInZooming )
    {
        mDefaultFontSize = q->font().pointSize();
        mZoomLevel = 1.0;
    }
}

void AbstractByteArrayViewPrivate::zoomIn()  { zoomIn( DefaultZoomStep ); }
void AbstractByteArrayViewPrivate::zoomOut() { zoomOut( DefaultZoomStep ); }

void AbstractByteArrayViewPrivate::zoomIn( int pointIncrement )
{
    Q_Q( AbstractByteArrayView );

    QFont newFont( q->font() );
    int newPointSize = QFontInfo(newFont).pointSize() + pointIncrement;
    if( newPointSize > MaxFontPointSize )
        newPointSize = MaxFontPointSize;

    mZoomLevel = (double)newPointSize/mDefaultFontSize;
    newFont.setPointSize( newPointSize );

    mInZooming = true;
    q->setFont( newFont );
    mInZooming = false;
}

void AbstractByteArrayViewPrivate::zoomOut( int pointDecrement )
{
    Q_Q( AbstractByteArrayView );

    QFont newFont( q->font() );
    int newPointSize = QFontInfo(newFont).pointSize() - pointDecrement;
    if( newPointSize < MinFontPointSize )
        newPointSize = MinFontPointSize;

    mZoomLevel = (double)newPointSize/mDefaultFontSize;
    newFont.setPointSize( newPointSize );

    mInZooming = true;
    q->setFont( newFont );
    mInZooming = false;
}


void AbstractByteArrayViewPrivate::zoomTo( int newPointSize )
{
    Q_Q( AbstractByteArrayView );

    if( newPointSize < MinFontPointSize )
        newPointSize = MinFontPointSize;
    else if( newPointSize > MaxFontPointSize )
        newPointSize = MaxFontPointSize;

    QFont newFont( q->font() );
    if( QFontInfo(newFont).pointSize() == newPointSize )
        return;

    newFont.setPointSize( newPointSize );
    mZoomLevel = (double)newPointSize/mDefaultFontSize;

    mInZooming = true;
    q->setFont( newFont );
    mInZooming = false;
}


void AbstractByteArrayViewPrivate::unZoom()
{
    zoomTo( mDefaultFontSize );
}

void AbstractByteArrayViewPrivate::setZoomLevel( double zoomLevel )
{
    Q_Q( AbstractByteArrayView );

    const int newPointSize = (int)(zoomLevel*mDefaultFontSize);
    const int currentPointSize = q->fontInfo().pointSize();

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


void AbstractByteArrayViewPrivate::setStartOffset( int startOffset )
{
    Q_Q( AbstractByteArrayView );

    if( !mTableLayout->setStartOffset(startOffset) )
        return;

    pauseCursor();
    // affects:
    // the no of lines -> width
    adjustLayoutToSize();

    q->viewport()->update();

    mTableCursor->updateCoord();
    ensureCursorVisible();
    unpauseCursor();
    emit q->cursorPositionChanged( cursorPosition() );
}

void AbstractByteArrayViewPrivate::setFirstLineOffset( int firstLineOffset )
{
    Q_Q( AbstractByteArrayView );

    if( !mTableLayout->setFirstLineOffset(firstLineOffset) )
        return;

    pauseCursor();
    // affects:
    // the no of lines -> width
    adjustLayoutToSize();

    q->viewport()->update();

    mTableCursor->updateCoord();
    ensureCursorVisible();
    unpauseCursor();
    emit q->cursorPositionChanged( cursorPosition() );
}

void AbstractByteArrayViewPrivate::setResizeStyle( AbstractByteArrayView::ResizeStyle resizeStyle )
{
    const bool isChanged = ( mResizeStyle != resizeStyle );

    if( !isChanged )
        return;

    mResizeStyle = resizeStyle;
    updateViewByWidth();
}

void AbstractByteArrayViewPrivate::setNoOfBytesPerLine( int noOfBytesPerLine )
{
    // if the number is explicitly set we expect a wish for no automatic resize
    setResizeStyle( AbstractByteArrayView::NoResize );

    if( !mTableLayout->setNoOfBytesPerLine(noOfBytesPerLine) )
        return;

    updateViewByWidth();
}


void AbstractByteArrayViewPrivate::setReadOnly( bool readOnly )
{
    Q_Q( AbstractByteArrayView );

    const bool isChanged = ( mReadOnly != readOnly );

    if( !isChanged )
        return;

    mReadOnly = readOnly;

    adaptController();

    if( mByteArrayModel->isReadOnly() )
        emit q->readOnlyChanged( mReadOnly );
}

void AbstractByteArrayViewPrivate::setOverwriteMode( bool overwriteMode )
{
    Q_Q( AbstractByteArrayView );

    const bool isChanged = ( (mOverWrite != overwriteMode)
                             && (!mOverWriteOnly || overwriteMode) );

    if( !isChanged )
        return;

    mOverWrite = overwriteMode;

    // affected:
    // cursor shape
    const bool changeCursor = !( mCursorPaused || isByteEditorActive() );
    if( changeCursor )
        pauseCursor();

    mTableCursor->setAppendPosEnabled( !mOverWrite );

    if( changeCursor )
        unpauseCursor();

    emit q->overwriteModeChanged( mOverWrite );
    emit q->cutAvailable( !mOverWrite && mTableRanges->hasSelection() );
}

void AbstractByteArrayViewPrivate::setOverwriteOnly( bool overwriteOnly )
{
    const bool isChanged = ( mOverWriteOnly != overwriteOnly );

    if( !isChanged )
        return;

    mOverWriteOnly = overwriteOnly;

    if( mOverWriteOnly )
        setOverwriteMode( true );
}

void AbstractByteArrayViewPrivate::setValueCoding( AbstractByteArrayView::ValueCoding valueCoding )
{
    if( mValueCoding == valueCoding )
        return;

    Okteta::ValueCodec* newValueCodec
        = Okteta::ValueCodec::createCodec( (Okteta::ValueCoding)valueCoding );
    if( newValueCodec == 0 )
        return;

    delete mValueCodec;
    mValueCodec = newValueCodec;
    mValueCoding = valueCoding;
}
void AbstractByteArrayViewPrivate::setCharCoding( AbstractByteArrayView::CharCoding charCoding )
{
    if( mCharCoding == charCoding )
        return;

    Okteta::CharCodec* newCharCodec
        = Okteta::CharCodec::createCodec( (Okteta::CharCoding)charCoding );
    if( newCharCodec == 0 )
        return;

    delete mCharCodec;
    mCharCodec = newCharCodec;
    mCharCoding = charCoding;
}
void AbstractByteArrayViewPrivate::setCharCoding( const QString& charCodingName )
{
    if( mCharCodec->name() == charCodingName )
        return;

    Okteta::CharCodec *newCharCodec =
        Okteta::CharCodec::createCodec( charCodingName );
    if( newCharCodec == 0 )
        return;

    delete mCharCodec;
    mCharCodec = newCharCodec;
    mCharCoding = AbstractByteArrayView::LocalEncoding; // TODO: add encoding no to every known codec
}

// TODO: make this use select( start, end )
bool AbstractByteArrayViewPrivate::selectWord( /*unsigned TODO:change all unneeded signed into unsigned!*/ int index )
{
    Q_Q( AbstractByteArrayView );

    bool result = false;
    if( 0 <= index && index < mTableLayout->length()  )
    {
        const Okteta::WordByteArrayService WBS( mByteArrayModel, mCharCodec );
        const KDE::Section wordSection = WBS.wordSection( index );
        if( wordSection.isValid() )
        {
            const bool oldHasSelection = mTableRanges->hasSelection();

            pauseCursor();
            finishByteEditor();

            mTableRanges->setFirstWordSelection( wordSection );
            mTableCursor->gotoIndex( wordSection.nextBehindEnd() );
            updateChanged();

            unpauseCursor();

            const bool newHasSelection = mTableRanges->hasSelection();
            emit q->selectionChanged( wordSection );
            if( oldHasSelection != newHasSelection )
            {
                if( !mOverWrite ) emit q->cutAvailable( newHasSelection );
                emit q->copyAvailable( newHasSelection );
                emit q->hasSelectedDataChanged( newHasSelection );
            }
            emit q->cursorPositionChanged( cursorPosition() );

            result = true;
        }
    }
    return result;
}


// TODO: make this use select( start, end )
void AbstractByteArrayViewPrivate::selectAll( bool select )
{
    Q_Q( AbstractByteArrayView );

    const bool oldHasSelection = mTableRanges->hasSelection();

    pauseCursor();
    finishByteEditor();

    if( select )
    {
        mTableRanges->setSelection( KDE::Section(0,mTableLayout->length()-1) );
        mTableCursor->gotoEnd();
    }
    else
        mTableRanges->removeSelection();

    updateChanged();

    unpauseCursor();

    const bool newHasSelection = mTableRanges->hasSelection();
    emit q->selectionChanged( mTableRanges->selection() );
    if( oldHasSelection != newHasSelection )
    {
        if( !mOverWrite ) emit q->cutAvailable( newHasSelection );
        emit q->copyAvailable( newHasSelection );
        emit q->hasSelectedDataChanged( newHasSelection );
    }
    emit q->cursorPositionChanged( cursorPosition() );
}


void AbstractByteArrayViewPrivate::setCursorPosition( int index, bool behind )
{
    Q_Q( AbstractByteArrayView );

    const bool oldHasSelection = mTableRanges->hasSelection();

    pauseCursor();
    finishByteEditor();

    if( behind ) --index;
    mTableCursor->gotoCIndex( index );
    if( behind )
        mTableCursor->stepBehind();

    mTableRanges->removeSelection();
    if( mTableRanges->isModified() )
    {
        updateChanged();

        const bool newHasSelection = mTableRanges->hasSelection();
        emit q->selectionChanged( mTableRanges->selection() );
        if( oldHasSelection != newHasSelection )
        {
            if( !mOverWrite ) emit q->cutAvailable( newHasSelection );
            emit q->copyAvailable( newHasSelection );
            emit q->hasSelectedDataChanged( newHasSelection );
        }
    }
    ensureCursorVisible();

    unpauseCursor();
    emit q->cursorPositionChanged( cursorPosition() );
}



void AbstractByteArrayViewPrivate::setSelectionCursorPosition( int index )
{
    Q_Q( AbstractByteArrayView );

    pauseCursor();
    finishByteEditor();

    if( !mTableRanges->selectionStarted() )
        mTableRanges->setSelectionStart( mTableCursor->realIndex() );

    mTableCursor->gotoCIndex( index );

    mTableRanges->setSelectionEnd( mTableCursor->realIndex() );

    ensureCursorVisible();
    updateChanged();

    unpauseCursor();

    if( mTableRanges->isModified() )
        q->emitSelectionSignals(); // TODO: can this be moved somewhere
    emit q->cursorPositionChanged( mTableCursor->realIndex() );
}



void AbstractByteArrayViewPrivate::setSelection( const KDE::Section& _selection )
{
    Q_Q( AbstractByteArrayView );

    KDE::Section selection( _selection );
    selection.restrictEndTo( mTableLayout->length()-1 );

    const KDE::Section oldSelection = mTableRanges->selection();

    if( ! selection.isValid()
        || selection == oldSelection )
        return;

    pauseCursor();
    finishByteEditor();

    mTableRanges->setSelection( selection );
    mTableCursor->gotoCIndex( selection.nextBehindEnd() );
// TODO:            ensureVisible( *mActiveColumn, mTableLayout->coordOfIndex(selection.start()) );
    ensureCursorVisible();
    updateChanged();

    unpauseCursor();

    emit q->selectionChanged( selection );
    if( oldSelection.isEmpty() )
    {
        if( !mOverWrite ) emit q->cutAvailable( true );
        emit q->copyAvailable( true );
        emit q->hasSelectedDataChanged( true );
    }
    emit q->cursorPositionChanged( cursorPosition() );
}


QByteArray AbstractByteArrayViewPrivate::selectedData() const
{
    if( !mTableRanges->hasSelection() )
        return QByteArray();

    const KDE::Section selection = mTableRanges->selection();
    QByteArray data;
    data.resize( selection.width() );
    byteArrayModel()->copyTo( data.data(), selection.start(), selection.width() );
    return data;
}


QMimeData* AbstractByteArrayViewPrivate::selectionAsMimeData() const
{
    if( !mTableRanges->hasSelection() )
        return 0;

    QMimeData* mimeData = new QMimeData;
    mimeData->setData( OctetStreamFormatName, selectedData() );
    return mimeData;
}

void AbstractByteArrayViewPrivate::cut()
{
    if( isEffectiveReadOnly() || mOverWrite )
        return;

    QMimeData* cutData = selectionAsMimeData();
    if( !cutData )
        return;

    QApplication::clipboard()->setMimeData( cutData, mClipboardMode );

    removeSelectedData();
}


void AbstractByteArrayViewPrivate::copy()
{
    QMimeData* cutData = selectionAsMimeData();
    if( !cutData )
        return;

    QApplication::clipboard()->setMimeData( cutData, mClipboardMode );
}


void AbstractByteArrayViewPrivate::paste()
{
    if( isEffectiveReadOnly() )
        return;

    const QMimeData* data = QApplication::clipboard()->mimeData( mClipboardMode );
    pasteData( data );
}


void AbstractByteArrayViewPrivate::pasteData( const QMimeData* data )
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

bool AbstractByteArrayViewPrivate::canReadData( const QMimeData* data ) const
{
Q_UNUSED( data )
    // taking all for now, see comment in pasteData above
    return true;//data->hasFormat( OctetStreamFormatName );
}


void AbstractByteArrayViewPrivate::insert( const QByteArray& data )
{
    Q_Q( AbstractByteArrayView );

    const bool oldHasSelection = mTableRanges->hasSelection();

    if( mOverWrite )
    {
        int lengthOfInserted;
        if( mTableRanges->hasSelection() )
        {
            // replacing the selection:
            // we restrict the replacement to the minimum length of selection and input
            KDE::Section selection = mTableRanges->removeSelection();
            selection.restrictEndByWidth( data.size() );
            lengthOfInserted = mByteArrayModel->replace( selection, data.data(), selection.width() );
        }
        else
        {
            const int length = mTableLayout->length();
            if( !isCursorBehind() && length > 0 )
            {
                // replacing the normal data, at least until the end
                KDE::Section insertRange = KDE::Section::fromWidth( cursorPosition(), data.size() );
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
            mTableCursor->gotoNextByte(lengthOfInserted);
            unpauseCursor();
            emit q->cursorPositionChanged( cursorPosition() );
        }
    }
    else
    {
        if( mTableRanges->hasSelection() )
        {
            // replacing the selection
            const KDE::Section selection = mTableRanges->removeSelection();
            mByteArrayModel->replace( selection, data );
        }
        else
            mByteArrayModel->insert( cursorPosition(), data );
    }

    const bool newHasSelection = mTableRanges->hasSelection();
    emit q->selectionChanged( mTableRanges->selection() );
    if( oldHasSelection != newHasSelection )
        emit q->hasSelectedDataChanged( newHasSelection );
}


void AbstractByteArrayViewPrivate::removeSelectedData()
{
    // Can't we do this?
    if( isEffectiveReadOnly() || mOverWrite ) //TODO: || mValueEditor->isInEditMode() )
        return;

    const KDE::Section selection = mTableRanges->removeSelection();

    mByteArrayModel->remove( selection );

//     clearUndoRedo();

  //emit q->selectionChanged( -1, -1 );
}

bool AbstractByteArrayViewPrivate::hasChanged( const CoordRange& visibleRange, CoordRange* changedRange ) const
{
    const bool result = mTableRanges->overlapsChanges( visibleRange, changedRange );

    if( result )
        changedRange->restrictTo( visibleRange );

    return result;
}


void AbstractByteArrayViewPrivate::adaptController()
{
    KController* controller =
        isEffectiveReadOnly() ?                                 (KController*)mNavigator :
        activeCoding() == AbstractByteArrayView::CharCodingId ? (KController*)mCharEditor :
                                                                (KController*)mValueEditor;
    setController( controller );
}

void AbstractByteArrayViewPrivate::updateViewByWidth()
{
    Q_Q( AbstractByteArrayView );

    pauseCursor();

    adjustToLayoutNoOfBytesPerLine();
    adjustLayoutToSize();

    q->viewport()->update();

    mTableCursor->updateCoord();
    ensureCursorVisible();

    unpauseCursor();
    emit q->cursorPositionChanged( cursorPosition() );
}


void AbstractByteArrayViewPrivate::adjustLayoutToSize()
{
    Q_Q( AbstractByteArrayView );

    // check whether there is a change with the numbers of fitting bytes per line
    if( mResizeStyle != AbstractByteArrayView::NoResize )
    {
        // changes?
        if( mTableLayout->setNoOfBytesPerLine(fittingBytesPerLine()) )
            adjustToLayoutNoOfBytesPerLine();
    }

    q->setNoOfLines( mTableLayout->noOfLines() );
}

bool AbstractByteArrayViewPrivate::event( QEvent* event )
{
    Q_Q( AbstractByteArrayView );

    // 
    if( event->type() == QEvent::KeyPress )
    {
        QKeyEvent* keyEvent = static_cast<QKeyEvent*>( event );
        if( keyEvent->key() == Qt::Key_Tab || keyEvent->key() == Qt::Key_Backtab )
        {
            q->keyPressEvent( keyEvent );
            if( keyEvent->isAccepted() )
                return true;
        }
    }
    else if( event->type() == QEvent::ToolTip )
    {
        QHelpEvent* helpEvent = static_cast<QHelpEvent*>( event );

        QString toolTip;

        Okteta::Bookmarkable* bookmarks = qobject_cast<Okteta::Bookmarkable*>( mByteArrayModel );
        if( bookmarks )
        {
            const int index = indexByPoint( helpEvent->pos() );
            if( index != -1 )
            {
                if( bookmarks->containsBookmarkFor(index) )
                    toolTip = bookmarks->bookmarkFor( index ).name();
            }
        }

        if( ! toolTip.isNull() )
            QToolTip::showText( helpEvent->globalPos(), toolTip );
        else
        {
            QToolTip::hideText();
            event->ignore();
        }

        return true;
     }

    return q->ColumnsView::event( event );
}

void AbstractByteArrayViewPrivate::resizeEvent( QResizeEvent* resizeEvent )
{
    Q_Q( AbstractByteArrayView );

    if( mResizeStyle != AbstractByteArrayView::NoResize )
    {
        // changes?
        if( mTableLayout->setNoOfBytesPerLine(fittingBytesPerLine()) )
        {
            q->setNoOfLines( mTableLayout->noOfLines() );
            updateViewByWidth();
        }
    }

    q->ColumnsView::resizeEvent( resizeEvent );

    mTableLayout->setNoOfLinesPerPage( q->noOfLinesPerPage() ); // TODO: doesn't work with the new size!!!
}

void AbstractByteArrayViewPrivate::focusInEvent( QFocusEvent* focusEvent )
{
    Q_Q( AbstractByteArrayView );

    q->ColumnsView::focusInEvent( focusEvent );
    startCursor();
    emit q->focusChanged( true );
}

void AbstractByteArrayViewPrivate::focusOutEvent( QFocusEvent* focusEvent )
{
    Q_Q( AbstractByteArrayView );

    stopCursor();
    q->ColumnsView::focusOutEvent( focusEvent );
    emit q->focusChanged( false );
}


void AbstractByteArrayViewPrivate::dragEnterEvent( QDragEnterEvent* dragEnterEvent )
{
    if( mDropper->handleDragEnterEvent(dragEnterEvent) )
        dragEnterEvent->accept();
    else
        dragEnterEvent->ignore();
}

void AbstractByteArrayViewPrivate::dragMoveEvent( QDragMoveEvent* dragMoveEvent )
{
    if( mDropper->handleDragMoveEvent(dragMoveEvent) )
        dragMoveEvent->accept();
    else
        dragMoveEvent->ignore();
}

void AbstractByteArrayViewPrivate::dragLeaveEvent( QDragLeaveEvent* dragLeaveEvent )
{
    if( mDropper->handleDragLeaveEvent(dragLeaveEvent) )
        dragLeaveEvent->accept();
    else
        dragLeaveEvent->ignore();
}

void AbstractByteArrayViewPrivate::dropEvent( QDropEvent* dropEvent )
{
    if( mDropper->handleDropEvent(dropEvent) )
        dropEvent->accept();
    else
        dropEvent->ignore();
}


void AbstractByteArrayViewPrivate::onBookmarksChange( const QList<Okteta::Bookmark>& bookmarks )
{
    foreach( const Okteta::Bookmark& bookmark, bookmarks )
    {
        const int position = bookmark.offset();
        mTableRanges->addChangedRange( position, position );
    }

    unpauseCursor();
    updateChanged();
}

void AbstractByteArrayViewPrivate::onRevertedToVersionIndex( int versionIndex )
{
Q_UNUSED( versionIndex )
    finishByteEditor();
}

void AbstractByteArrayViewPrivate::onByteArrayReadOnlyChange( bool isByteArrayReadOnly )
{
    Q_Q( AbstractByteArrayView );

    adaptController();

    if( !mReadOnly )
        emit q->readOnlyChanged( isByteArrayReadOnly );
}


void AbstractByteArrayViewPrivate::onContentsChanged( const KDE::ArrayChangeMetricsList& changeList )
{
    Q_Q( AbstractByteArrayView );

    pauseCursor();

    const bool atEnd = mTableCursor->atEnd();
    const int oldLength = mTableLayout->length(); // TODO: hack for mDataCursor->adaptSelectionToChange
    // update lengths
    int oldNoOfLines = q->noOfLines();
    mTableLayout->setLength( mByteArrayModel->size() );
    const int newNoOfLines = mTableLayout->noOfLines();
    if( oldNoOfLines != newNoOfLines )
    {
        q->setNoOfLines( newNoOfLines );
        const KDE::Section changedLines = (oldNoOfLines < newNoOfLines) ?
            KDE::Section( oldNoOfLines, newNoOfLines-1 ) :
            KDE::Section( newNoOfLines, oldNoOfLines-1 );
        mTableRanges->addChangedOffsetLines( changedLines );
    }

    // adapt cursor(s)
    if( atEnd )
        mTableCursor->gotoEnd();
    else
        mTableCursor->adaptToChanges( changeList, oldLength );

    mTableRanges->adaptToChanges( changeList, oldLength );
    // kDebug() << "Cursor:"<<mDataCursor->index()<<", selection:"<<mTableRanges->selectionStart()<<"-"<<mTableRanges->selectionEnd()
    //          <<", BytesPerLine: "<<mTableLayout->noOfBytesPerLine()<<endl;

    ensureCursorVisible();
    updateChanged();
    unpauseCursor();

    emit q->cursorPositionChanged( cursorPosition() );
}

AbstractByteArrayViewPrivate::~AbstractByteArrayViewPrivate()
{
    delete mDropper;

    delete mZoomWheelController;

    delete mCharEditor;
    delete mValueEditor;
    delete mNavigator;
    delete mTabController;

    delete mTableRanges;
    delete mTableCursor;
    delete mTableLayout;
    delete mValueCodec;
    delete mCharCodec;
}

}
