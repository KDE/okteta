/*
    This file is part of the Okteta Gui library, made within the KDE community.

    SPDX-FileCopyrightText: 2008-2010, 2021 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "abstractbytearrayview_p.hpp"

// lib
#include "controller/touchonlytapandholdgesture.hpp"
#include "controller/touchonlytapandholdgesturerecognizer.hpp"
#include "widgetcolumnstylist.hpp"
#include "bordercolumnrenderer.hpp"
#include "oktetagui.hpp"
#include <logging.hpp>
// Okteta core
#include <Okteta/ValueCodec>
#include <Okteta/Bookmarkable>
#include <Okteta/Versionable>
#include <Okteta/TextByteArrayAnalyzer>
#include <Okteta/Bookmark>
// KF
#include <KLocalizedString>
// Qt
#include <QScroller>
#include <QPainter>
#include <QDragEnterEvent>
#include <QDragMoveEvent>
#include <QDragLeaveEvent>
#include <QDropEvent>
#include <QTimerEvent>
#include <QGestureEvent>
#include <QTapGesture>
#include <QPinchGesture>
#include <QStyleHints>
#include <QApplication>
#include <QToolTip>
#include <QMimeData>
#include <QMenu>
#include <QIcon>
#include <QScroller>
#include <QScrollerProperties>

namespace Okteta {

static constexpr Address DefaultStartOffset = 0;// 5;
static constexpr Address DefaultFirstLineOffset = 0;

// zooming is done in steps of font size points
static constexpr int DefaultZoomStep = 1;
static constexpr int MinFontPointSize = 4;
static constexpr int MaxFontPointSize = 128;

static constexpr AbstractByteArrayView::ValueCoding DefaultValueCoding =  AbstractByteArrayView::HexadecimalCoding;

static inline QString DefaultCharCoding() { return QString(); } // -> local 8-bit

static constexpr AbstractByteArrayView::LayoutStyle DefaultResizeStyle = AbstractByteArrayView::FixedLayoutStyle;

inline QString octetStreamFormatName() { return QStringLiteral("application/octet-stream"); }

class NullModel : public AbstractByteArrayModel
{
    Q_OBJECT

public:
    NullModel();
    ~NullModel() override;

public: // data access API
    Byte byte(Address offset) const override;
    Size size() const override;

public: // state read API
    bool isModified() const override;

public: // modification API
    Size replace(const AddressRange& removeSection, const Byte* insertData, int insertLength) override;
    bool swap(Address firstStart, const AddressRange& secondRange) override;
    Size fill(Byte fillByte, Address offset = 0, Size fillLength = -1) override;
    void setByte(Address offset, Byte byte) override;
    void setModified(bool modified) override;
};

NullModel::NullModel() = default;
NullModel::~NullModel() = default;

Byte NullModel::byte(Address offset) const { Q_UNUSED(offset) return 0; }
Size NullModel::size() const { return 0; }
bool NullModel::isModified() const { return false; }
Size NullModel::replace(const AddressRange& removeSection, const Byte* insertData, int insertLength)
{
    Q_UNUSED(removeSection) Q_UNUSED(insertData) Q_UNUSED(insertLength)
    return 0;
}
bool NullModel::swap(Address firstStart, const AddressRange& secondRange)
{
    Q_UNUSED(firstStart) Q_UNUSED(secondRange)
    return false;
}
Size NullModel::fill(Byte fillByte, Address offset, Size fillLength)
{
    Q_UNUSED(fillByte) Q_UNUSED(offset) Q_UNUSED(fillLength)
    return 0;
}
void NullModel::setByte(Address offset, Byte byte)
{
    Q_UNUSED(offset) Q_UNUSED(byte)
}
void NullModel::setModified(bool modified)
{
    Q_UNUSED(modified)
}

Q_GLOBAL_STATIC(NullModel, nullModel)

Qt::GestureType touchOnlyTapAndHoldGestureType()
{
    static Qt::GestureType type =
        QGestureRecognizer::registerRecognizer(new TouchOnlyTapAndHoldGestureRecognizer);
    return type;
}

AbstractByteArrayViewPrivate::AbstractByteArrayViewPrivate(AbstractByteArrayView* parent)
    : ColumnsViewScrollAreaEngine(parent)
    , mByteArrayModel(nullModel())
    , mTableLayout(DefaultNoOfBytesPerLine, DefaultFirstLineOffset, DefaultStartOffset, 0, 0)
    , mTableCursor(&mTableLayout)
    , mTableRanges(&mTableLayout)
    , mTapNavigator(this)
    , mZoomPinchController(this)
    , mTabController(this, nullptr)
    , mUndoRedoController(this, &mTabController)
    , mClipboardController(this, &mUndoRedoController)
    , mKeyNavigator(this, &mClipboardController)
    , mValueEditor(this, &mKeyNavigator)
    , mCharEditor(this, &mKeyNavigator)
    , mMousePaster(this, nullptr)
    , mMouseNavigator(this, &mMousePaster)
    , mZoomWheelController(this, nullptr)
    , mDropper(this)
    , mMouseController(&mMouseNavigator)
    , mReadOnly(false)
    , mOverWriteOnly(false)
    , mOverWrite(true)
    , mInZooming(false)
    , mCursorPaused(false)
    , mBlinkCursorVisible(false)
    , mCursorVisible(false)
    // , mDefaultFontSize( p->font().pointSize() ) crashes in font()
    , mResizeStyle(DefaultResizeStyle)
{
}

AbstractByteArrayViewPrivate::~AbstractByteArrayViewPrivate() = default;

void AbstractByteArrayViewPrivate::init()
{
    Q_Q(AbstractByteArrayView);

    ColumnsViewScrollAreaEngine::init();

    // initialize layout
    mTableLayout.setLength(mByteArrayModel->size());
    mTableLayout.setNoOfLinesPerPage(noOfLinesPerPage());

    mStylist = std::make_unique<WidgetColumnStylist>(q);

    // TODO: on destruction, columns are destructed after mStylist instance in ~ColumnsViewScrollAreaEngine
    // small window of dangling pointer
    // TODO: here and in subclasses look for better pattern to refer to intsances owned by engine
    auto offsetColumn =
        std::make_unique<OffsetColumnRenderer>(mStylist.get(), &mTableLayout, OffsetFormat::Hexadecimal);
    mOffsetColumn = offsetColumn.get();
    auto offsetBorderColumn =
        std::make_unique<BorderColumnRenderer>(mStylist.get(), false);
    mOffsetBorderColumn = offsetBorderColumn.get();

    addColumn(std::move(offsetColumn));
    addColumn(std::move(offsetBorderColumn));

    mValueCodec = ValueCodec::createCodec((ValueCoding)DefaultValueCoding);
    mValueCoding = DefaultValueCoding;
    mCharCodec = CharCodec::createCodec(DefaultCharCoding());

    mWheelController = &mZoomWheelController;

    QObject::connect(QGuiApplication::styleHints(), &QStyleHints::cursorFlashTimeChanged,
                     q, [&](int flashTime) { onCursorFlashTimeChanged(flashTime); });

    q->setAcceptDrops(true);

    q->grabGesture(Qt::TapGesture);
    q->grabGesture(touchOnlyTapAndHoldGestureType());
    q->grabGesture(Qt::PinchGesture);

    // there seems no way to generate a QScroller explicitly (using QScroller::scroller(widget))
    // while also setting it to use the touch gesture
    // So we implicitly generate one with QScroller::grabGesture(widget) and the pick it as current active
    // by QScroller::scroller(widget)
    QScroller::grabGesture(q->viewport(), QScroller::TouchGesture);
    QScroller* scroller = QScroller::scroller(q->viewport());
    QScrollerProperties scrollerProperties = scroller->scrollerProperties();
    scrollerProperties.setScrollMetric(QScrollerProperties::HorizontalOvershootPolicy, QScrollerProperties::OvershootAlwaysOff);
    scrollerProperties.setScrollMetric(QScrollerProperties::VerticalOvershootPolicy, QScrollerProperties::OvershootAlwaysOff);
    // values used in other KDE apps
    scrollerProperties.setScrollMetric(QScrollerProperties::DecelerationFactor, 0.3);
    scrollerProperties.setScrollMetric(QScrollerProperties::MaximumVelocity, 1);
    scrollerProperties.setScrollMetric(QScrollerProperties::AcceleratingFlickMaximumTime, 0.2); // Workaround for QTBUG-88249 (non-flick gestures recognized as accelerating flick)
    scrollerProperties.setScrollMetric(QScrollerProperties::DragStartDistance, 0.0);
    scroller->setScrollerProperties(scrollerProperties);
}

void AbstractByteArrayViewPrivate::setByteArrayModel(AbstractByteArrayModel* byteArrayModel)
{
    Q_Q(AbstractByteArrayView);

    mByteArrayModel->disconnect(q);
    mCursorPaused = true;

    mByteArrayModel = byteArrayModel ? byteArrayModel : nullModel();

    // affected:
    // length -> no of lines -> width
    mTableLayout.setLength(mByteArrayModel->size());
    adjustLayoutToSize();

    // if the model is readonly make the view too, per default
    if (mByteArrayModel->isReadOnly()) {
        setReadOnly(true);
    }

    QObject::connect(mByteArrayModel, &AbstractByteArrayModel::readOnlyChanged,
                     q, [&](bool isReadOnly) { onByteArrayReadOnlyChange(isReadOnly); });
    QObject::connect(mByteArrayModel, &AbstractByteArrayModel::contentsChanged,
                     q, [&](const Okteta::ArrayChangeMetricsList& changeList) { onContentsChanged(changeList); });

    Bookmarkable* bookmarks = qobject_cast<Bookmarkable*>(mByteArrayModel);
    if (bookmarks) {
        QObject::connect(mByteArrayModel, SIGNAL(bookmarksAdded(QVector<Okteta::Bookmark>)),
                         q, SLOT(onBookmarksChange(QVector<Okteta::Bookmark>)));
        QObject::connect(mByteArrayModel, SIGNAL(bookmarksRemoved(QVector<Okteta::Bookmark>)),
                         q, SLOT(onBookmarksChange(QVector<Okteta::Bookmark>)));
    }
    Versionable* versionControl = qobject_cast<Versionable*>(mByteArrayModel);
    if (versionControl) {
        QObject::connect(mByteArrayModel, SIGNAL(revertedToVersionIndex(int)),
                         q, SLOT(onRevertedToVersionIndex(int)));
    }

    q->viewport()->update();

    mTableCursor.gotoStart();
    ensureCursorVisible();

    unpauseCursor();

    Q_EMIT q->cursorPositionChanged(cursorPosition());
}

void AbstractByteArrayViewPrivate::toggleOffsetColumn(bool offsetColumnVisible)
{
    Q_Q(AbstractByteArrayView);

    const bool isVisible = mOffsetColumn->isVisible();
    // no change?
    if (isVisible == offsetColumnVisible) {
        return;
    }

    mOffsetColumn->setVisible(offsetColumnVisible);

    updateViewByWidth();

    Q_EMIT q->offsetColumnVisibleChanged(offsetColumnVisible);
}

void AbstractByteArrayViewPrivate::setOffsetCoding(AbstractByteArrayView::OffsetCoding offsetCoding)
{
    Q_Q(AbstractByteArrayView);

    const auto format = static_cast<OffsetFormat::Format>(offsetCoding);
    const OffsetFormat::Format currentFormat = mOffsetColumn->format();
    // no change?
    if (currentFormat == format) {
        return;
    }

    mOffsetColumn->setFormat(format, q->fontMetrics());

    updateViewByWidth();

    Q_EMIT q->offsetCodingChanged(offsetCoding);
}

void AbstractByteArrayViewPrivate::setNoOfLines(int newNoOfLines)
{
    ColumnsViewScrollAreaEngine::setNoOfLines(newNoOfLines > 1 ? newNoOfLines : 1);
}


void AbstractByteArrayViewPrivate::changeEvent(QEvent* event)
{
    Q_Q(AbstractByteArrayView);

    q->QAbstractScrollArea::changeEvent(event);

    if (event->type() == QEvent::FontChange
        && !mInZooming) {
        mDefaultFontSize = q->font().pointSize();
        // TODO: why reset zoomlevel here? should this not rather recalculate the new applied font size?
        mZoomLevel = 1.0;
    }
}

void AbstractByteArrayViewPrivate::zoomIn()  { zoomIn(DefaultZoomStep); }
void AbstractByteArrayViewPrivate::zoomOut() { zoomOut(DefaultZoomStep); }

void AbstractByteArrayViewPrivate::zoomIn(int pointIncrement)
{
    Q_Q(AbstractByteArrayView);

    QFont newFont(q->font());
    int newPointSize = QFontInfo(newFont).pointSize() + pointIncrement;
    if (newPointSize > MaxFontPointSize) {
        newPointSize = MaxFontPointSize;
    }

    mZoomLevel = (double)newPointSize / mDefaultFontSize;
    newFont.setPointSize(newPointSize);

    mInZooming = true;
    q->setFont(newFont);
    mInZooming = false;

    Q_EMIT q->zoomLevelChanged(mZoomLevel);
}

void AbstractByteArrayViewPrivate::zoomOut(int pointDecrement)
{
    Q_Q(AbstractByteArrayView);

    QFont newFont(q->font());
    int newPointSize = QFontInfo(newFont).pointSize() - pointDecrement;
    if (newPointSize < MinFontPointSize) {
        newPointSize = MinFontPointSize;
    }

    mZoomLevel = (double)newPointSize / mDefaultFontSize;
    newFont.setPointSize(newPointSize);

    mInZooming = true;
    q->setFont(newFont);
    mInZooming = false;

    Q_EMIT q->zoomLevelChanged(mZoomLevel);
}

void AbstractByteArrayViewPrivate::zoomTo(int newPointSize)
{
    Q_Q(AbstractByteArrayView);

    if (newPointSize < MinFontPointSize) {
        newPointSize = MinFontPointSize;
    } else if (newPointSize > MaxFontPointSize) {
        newPointSize = MaxFontPointSize;
    }

    QFont newFont(q->font());
    if (QFontInfo(newFont).pointSize() == newPointSize) {
        return;
    }

    newFont.setPointSize(newPointSize);
    mZoomLevel = (double)newPointSize / mDefaultFontSize;

    mInZooming = true;
    q->setFont(newFont);
    mInZooming = false;

    Q_EMIT q->zoomLevelChanged(mZoomLevel);
}

void AbstractByteArrayViewPrivate::unZoom()
{
    zoomTo(mDefaultFontSize);
}

void AbstractByteArrayViewPrivate::setZoomLevel(double zoomLevel)
{
    Q_Q(AbstractByteArrayView);

    const int currentPointSize = q->fontInfo().pointSize();

    // TODO: here we catch any new zoomlevels which are out of bounds and the zoom already at that bound
    if ((currentPointSize <= MinFontPointSize && zoomLevel < (double)MinFontPointSize / mDefaultFontSize)
        || (MaxFontPointSize <= currentPointSize && (double)MaxFontPointSize / mDefaultFontSize < zoomLevel)) {
        return;
    }

    int newPointSize = (int)(zoomLevel * mDefaultFontSize);
    if (newPointSize < MinFontPointSize) {
        newPointSize = MinFontPointSize;
    } else if (newPointSize > MaxFontPointSize) {
        newPointSize = MaxFontPointSize;
    }

    QFont newFont(q->font());

    // other than in zoomTo(), where the new zoomlevel is calculated from the integers, here
    // use the passed zoomlevel value, to avoid getting trapped inside a small integer value,
    // if the zoom tool operates relatively
    // think about, if this is the right approach
    mZoomLevel = zoomLevel;
    newFont.setPointSize(newPointSize);

    mInZooming = true;
    q->setFont(newFont);
    mInZooming = false;

    Q_EMIT q->zoomLevelChanged(mZoomLevel);
}

void AbstractByteArrayViewPrivate::setStartOffset(Address startOffset)
{
    Q_Q(AbstractByteArrayView);

    if (!mTableLayout.setStartOffset(startOffset)) {
        return;
    }

    pauseCursor();

    // affects:
    // the no of lines -> width
    adjustLayoutToSize();

    q->viewport()->update();

    mTableCursor.updateCoord();
    ensureCursorVisible();

    unpauseCursor();
    Q_EMIT q->cursorPositionChanged(cursorPosition());
}

void AbstractByteArrayViewPrivate::setFirstLineOffset(Address firstLineOffset)
{
    Q_Q(AbstractByteArrayView);

    if (!mTableLayout.setFirstLineOffset(firstLineOffset)) {
        return;
    }

    pauseCursor();

    // affects:
    // the no of lines -> width
    adjustLayoutToSize();

    q->viewport()->update();

    mTableCursor.updateCoord();
    ensureCursorVisible();

    unpauseCursor();
    Q_EMIT q->cursorPositionChanged(cursorPosition());
}

void AbstractByteArrayViewPrivate::setLayoutStyle(AbstractByteArrayView::LayoutStyle layoutStyle)
{
    Q_Q(AbstractByteArrayView);

    const bool isChanged = (mResizeStyle != layoutStyle);

    if (!isChanged) {
        return;
    }

    mResizeStyle = layoutStyle;
    updateViewByWidth();

    Q_EMIT q->layoutStyleChanged(mResizeStyle);
}

void AbstractByteArrayViewPrivate::setNoOfBytesPerLine(int noOfBytesPerLine)
{
    Q_Q(AbstractByteArrayView);

    // if the number is explicitly set we expect a wish for no automatic resize
    setLayoutStyle(AbstractByteArrayView::FixedLayoutStyle);

    if (!mTableLayout.setNoOfBytesPerLine(noOfBytesPerLine)) {
        return;
    }

    updateViewByWidth();

    Q_EMIT q->noOfBytesPerLineChanged(mTableLayout.noOfBytesPerLine());
}

void AbstractByteArrayViewPrivate::setReadOnly(bool readOnly)
{
    Q_Q(AbstractByteArrayView);

    const bool isChanged = (mReadOnly != readOnly);

    if (!isChanged) {
        return;
    }

    mReadOnly = readOnly;

    adaptController();

    if (mByteArrayModel->isReadOnly()) {
        Q_EMIT q->readOnlyChanged(mReadOnly);
    }
}

void AbstractByteArrayViewPrivate::setOverwriteMode(bool overwriteMode)
{
    Q_Q(AbstractByteArrayView);

    const bool isChanged = ((mOverWrite != overwriteMode)
                            && (!mOverWriteOnly || overwriteMode));

    if (!isChanged) {
        return;
    }

    mOverWrite = overwriteMode;

    // affected:
    // cursor shape
    const bool changeCursor = !(mCursorPaused || isByteEditorActive());
    if (changeCursor) {
        pauseCursor();
    }

    mTableCursor.setAppendPosEnabled(!mOverWrite);

    if (changeCursor) {
        unpauseCursor();
    }

    Q_EMIT q->overwriteModeChanged(mOverWrite);
    Q_EMIT q->cutAvailable(!mOverWrite && mTableRanges.hasSelection());
}

void AbstractByteArrayViewPrivate::setOverwriteOnly(bool overwriteOnly)
{
    const bool isChanged = (mOverWriteOnly != overwriteOnly);

    if (!isChanged) {
        return;
    }

    mOverWriteOnly = overwriteOnly;

    if (mOverWriteOnly) {
        setOverwriteMode(true);
    }
}

void AbstractByteArrayViewPrivate::setValueCoding(AbstractByteArrayView::ValueCoding valueCoding)
{
    if (mValueCoding == valueCoding) {
        return;
    }

    auto newValueCodec = ValueCodec::createCodec((ValueCoding)valueCoding);
    if (!newValueCodec) {
        return;
    }

    mValueCodec = std::move(newValueCodec);
    mValueCoding = valueCoding;
}
void AbstractByteArrayViewPrivate::setCharCoding(const QString& charCodingName)
{
    if (mCharCodec->name() == charCodingName) {
        return;
    }

    auto newCharCodec = CharCodec::createCodec(charCodingName);
    if (!newCharCodec) {
        return;
    }

    mCharCodec = std::move(newCharCodec);
}

void AbstractByteArrayViewPrivate::setMarking(const AddressRange& _marking)
{
    AddressRange marking(_marking);
    marking.restrictEndTo(mTableLayout.length() - 1);

    const AddressRange oldMarking = mTableRanges.marking();

    if (marking == oldMarking) {
        return;
    }

    mTableRanges.setMarking(marking);

    updateChanged();
}

// TODO: make this use select( start, end )
bool AbstractByteArrayViewPrivate::selectWord(Address index)
{
    bool result = false;

    if (0 <= index && index < mTableLayout.length()) {
        const TextByteArrayAnalyzer textAnalyzer(mByteArrayModel, mCharCodec.get());
        const AddressRange wordSection = textAnalyzer.wordSection(index);
        if (wordSection.isValid()) {
            pauseCursor();
            finishByteEditor();

            mTableRanges.setFirstWordSelection(wordSection);
            mTableCursor.gotoIndex(wordSection.nextBehindEnd());

            endViewUpdate();

            result = true;
        }
    }
    return result;
}

// TODO: make this use select( start, end )
void AbstractByteArrayViewPrivate::selectAll(bool select)
{
    pauseCursor();
    finishByteEditor();

    if (select) {
        mTableRanges.setSelection(AddressRange(0, mTableLayout.length() - 1));
        mTableCursor.gotoEnd();
    } else {
        mTableRanges.removeSelection();
    }

    endViewUpdate();
}

void AbstractByteArrayViewPrivate::setCursorPosition(Address index, bool behind)
{
    pauseCursor();
    finishByteEditor();

    if (behind) {
        --index;
    }
    mTableCursor.gotoCIndex(index);
    if (behind) {
        mTableCursor.stepBehind();
    }

    mTableRanges.removeSelection();
    ensureCursorVisible();

    endViewUpdate();
}

void AbstractByteArrayViewPrivate::setSelectionCursorPosition(Address index)
{
    pauseCursor();
    finishByteEditor();

    if (!mTableRanges.selectionStarted()) {
        mTableRanges.setSelectionStart(mTableCursor.realIndex());
    }

    mTableCursor.gotoCIndex(index);

    mTableRanges.setSelectionEnd(mTableCursor.realIndex());
    ensureCursorVisible();

    endViewUpdate();
}

void AbstractByteArrayViewPrivate::setSelection(const AddressRange& _selection)
{
    AddressRange selection(_selection);
    selection.restrictEndTo(mTableLayout.length() - 1);

    const AddressRange oldSelection = mTableRanges.selection();

    if (!selection.isValid()
        || selection == oldSelection) {
        return;
    }

    pauseCursor();
    finishByteEditor();

    mTableRanges.setSelection(selection);
    mTableCursor.gotoCIndex(selection.nextBehindEnd());

// TODO:            ensureVisible( *mActiveColumn, mTableLayout.coordOfIndex(selection.start()) );
    ensureCursorVisible();

    endViewUpdate();
}

QByteArray AbstractByteArrayViewPrivate::selectedData() const
{
    if (!mTableRanges.hasSelection()) {
        return {};
    }

    const AddressRange selection = mTableRanges.selection();
    QByteArray data;
    data.resize(selection.width());
    byteArrayModel()->copyTo(reinterpret_cast<Byte*>(data.data()), selection.start(), selection.width());
    return data;
}

std::unique_ptr<QMimeData> AbstractByteArrayViewPrivate::selectionAsMimeData() const
{
    if (!mTableRanges.hasSelection()) {
        return {};
    }

    auto mimeData = std::make_unique<QMimeData>();
    mimeData->setData(octetStreamFormatName(), selectedData());
    return mimeData;
}

void AbstractByteArrayViewPrivate::cutToClipboard(QClipboard::Mode mode)
{
    if (isEffectiveReadOnly() || mOverWrite) {
        return;
    }

    auto cutData = selectionAsMimeData();
    if (!cutData) {
        return;
    }

    QApplication::clipboard()->setMimeData(cutData.release(), mode);

    removeSelectedData();
}

void AbstractByteArrayViewPrivate::copyToClipboard(QClipboard::Mode mode) const
{
    auto cutData = selectionAsMimeData();
    if (!cutData) {
        return;
    }

//     if( mode == QClipboard::Selection )
//         q->disconnect( QApplication::clipboard(), SIGNAL(selectionChanged()) );

    QApplication::clipboard()->setMimeData(cutData.release(), mode);

    // TODO: why did we do this? And why does the disconnect above not work?
    // got connected multiple times after a few selections by mouse
//         connect( QApplication::clipboard(), SIGNAL(selectionChanged()), SLOT(clipboardChanged()) );
}

void AbstractByteArrayViewPrivate::pasteFromClipboard(QClipboard::Mode mode)
{
    if (isEffectiveReadOnly()) {
        return;
    }

    const QMimeData* data = QApplication::clipboard()->mimeData(mode);
    pasteData(data);
}

void AbstractByteArrayViewPrivate::pasteData(const QMimeData* data)
{
    if (!data || data->formats().isEmpty()) {
        return;
    }

    // SYNC: with bytearraydocumentfactory.cpp
    // if there is a octet stream, use it, otherwise take the dump of the format
    // with the highest priority
    // TODO: this may not be, what is expected, think about it, if we just
    // take byte array descriptions, like encodings in chars or values
    // would need the movement of the encoders into the core library
    QString dataFormatName = octetStreamFormatName();
    if (!data->hasFormat(dataFormatName)) {
        dataFormatName = data->formats()[0];
    }

    const QByteArray byteArray = data->data(dataFormatName);

    if (!byteArray.isEmpty()) {
        insert(byteArray);
    }
}

bool AbstractByteArrayViewPrivate::canReadData(const QMimeData* data) const
{
    Q_UNUSED(data)
    // taking all for now, see comment in pasteData above
    return true;// data->hasFormat( OctetStreamFormatName );
}

void AbstractByteArrayViewPrivate::insert(const QByteArray& data)
{
    Q_Q(AbstractByteArrayView);

    Size lengthOfInserted;
    Address insertionOffset = -1;
    if (mOverWrite) {
        if (mTableRanges.hasSelection()) {
            // replacing the selection:
            // we restrict the replacement to the minimum length of selection and input
            AddressRange selection = mTableRanges.removeSelection();
            selection.restrictEndByWidth(data.size());
            insertionOffset = selection.start();
            lengthOfInserted = mByteArrayModel->replace(selection, reinterpret_cast<const Byte*>(data.constData()), selection.width());
        } else {
            const Size length = mTableLayout.length();
            if (!isCursorBehind() && length > 0) {
                // replacing the normal data, at least until the end
                AddressRange insertRange = AddressRange::fromWidth(cursorPosition(), data.size());
                insertRange.restrictEndTo(length - 1);
                insertionOffset = insertRange.start();
                lengthOfInserted = mByteArrayModel->replace(insertRange, reinterpret_cast<const Byte*>(data.constData()), insertRange.width());
            } else {
                lengthOfInserted = 0;
            }
        }
    } else {
        if (mTableRanges.hasSelection()) {
            // replacing the selection
            const AddressRange selection = mTableRanges.removeSelection();
            insertionOffset = selection.start();
            lengthOfInserted = mByteArrayModel->replace(selection, data);
        } else {
            insertionOffset = cursorPosition();
            lengthOfInserted = mByteArrayModel->insert(insertionOffset, data);
        }
    }
    // if inserting ourself we want to place the cursor at the end of the inserted data
    if (lengthOfInserted > 0) {
        const Address postInsertionOffset = insertionOffset + lengthOfInserted;
        if (postInsertionOffset != cursorPosition()) {
            pauseCursor();
            mTableCursor.gotoCIndex(postInsertionOffset);
            unpauseCursor();
            Q_EMIT q->cursorPositionChanged(cursorPosition());
        }
    }
}

void AbstractByteArrayViewPrivate::removeSelectedData()
{
    // Can't we do this?
    if (isEffectiveReadOnly() || mOverWrite) { // TODO: || mValueEditor->isInEditMode() )
        return;
    }

    const AddressRange selection = mTableRanges.removeSelection();

    mByteArrayModel->remove(selection);

//     clearUndoRedo();
}

bool AbstractByteArrayViewPrivate::getNextChangedRange(CoordRange* changedRange, const CoordRange& visibleRange) const
{
    const bool result = mTableRanges.overlapsChanges(visibleRange, changedRange);

    if (result) {
        changedRange->restrictTo(visibleRange);
    }

    return result;
}

void AbstractByteArrayViewPrivate::adaptController()
{
    AbstractController* controller =
        isEffectiveReadOnly() ?                                 static_cast<AbstractController*>(&mKeyNavigator) :
        activeCoding() == AbstractByteArrayView::CharCodingId ? static_cast<AbstractController*>(&mCharEditor) :
                                                                static_cast<AbstractController*>(&mValueEditor);

    mController = controller;
}

void AbstractByteArrayViewPrivate::updateViewByWidth()
{
    Q_Q(AbstractByteArrayView);

    pauseCursor();

    adjustToLayoutNoOfBytesPerLine();
    adjustLayoutToSize();

    q->viewport()->update();

    mTableCursor.updateCoord();
    // TODO: see for what actions if would be usable to have ensureCursorVisible()
    // TODO: think about adding action "showCursor"/"show FocusItem" to menu
    // disabled as this prevents splitting views with aligned areas from working
    // ensureCursorVisible();

    unpauseCursor();
    Q_EMIT q->cursorPositionChanged(cursorPosition());
}

void AbstractByteArrayViewPrivate::adjustLayoutToSize()
{
    // check whether there is a change with the numbers of fitting bytes per line
    if (mResizeStyle != AbstractByteArrayView::FixedLayoutStyle) {
        // changes?
        if (mTableLayout.setNoOfBytesPerLine(fittingBytesPerLine())) {
            adjustToLayoutNoOfBytesPerLine();
        }
    }

    setNoOfLines(mTableLayout.noOfLines());
}

void AbstractByteArrayViewPrivate::onCursorFlashTimeChanged(int flashTime)
{
    Q_Q(AbstractByteArrayView);

    if (!mCursorVisible) {
        return;
    }

    if (mCursorBlinkTimerId != 0) {
        q->killTimer(mCursorBlinkTimerId);
    }

    if (flashTime >= 2) {
        mCursorBlinkTimerId = q->startTimer(flashTime / 2);
    } else {
        mCursorBlinkTimerId = 0;
    }

    // ensure cursor is drawn if set to not-blinking and currently in off-blink state
    if (!mBlinkCursorVisible && (mCursorBlinkTimerId == 0)) {
        blinkCursor();
    }
}

void AbstractByteArrayViewPrivate::startCursor()
{
    Q_Q(AbstractByteArrayView);

    mCursorPaused = false;
    mCursorVisible = true;

    updateCursors();

    const int flashTime = QGuiApplication::styleHints()->cursorFlashTime();
    if (flashTime >= 2) {
        mCursorBlinkTimerId = q->startTimer(flashTime / 2);
    }
}

void AbstractByteArrayViewPrivate::stopCursor()
{
    Q_Q(AbstractByteArrayView);

    mCursorVisible = false;

    if (mCursorBlinkTimerId != 0) {
        q->killTimer(mCursorBlinkTimerId);
        mCursorBlinkTimerId = 0;
    }

    pauseCursor();
}

void AbstractByteArrayViewPrivate::unpauseCursor()
{
    mCursorPaused = false;

    if (mCursorVisible) {
        updateCursors();
    }
}

void AbstractByteArrayViewPrivate::initPainterFromWidget(QPainter* painter) const
{
    Q_Q(const AbstractByteArrayView);

    const QPalette& palette = q->palette();
    painter->setPen(QPen(palette.brush(q->foregroundRole()), 1));
    painter->setBrush(palette.brush(q->backgroundRole()));
    painter->setFont(q->font());
}

QMenu* AbstractByteArrayViewPrivate::createStandardContextMenu(QPoint position)
{
    Q_UNUSED(position)

    Q_Q(AbstractByteArrayView);

    auto* menu = new QMenu(q);

    if (mUndoRedoController.addContextMenuActions(menu) > 0) {
        menu->addSeparator();
    }

    if (mClipboardController.addContextMenuActions(menu) > 0) {
        menu->addSeparator();
    }

    mKeyNavigator.addContextMenuActions(menu);

    return menu;
}

void AbstractByteArrayViewPrivate::mousePressEvent(QMouseEvent* mousePressEvent)
{
    Q_Q(AbstractByteArrayView);

    if (mMouseController->handleMousePressEvent(mousePressEvent)) {
        mousePressEvent->accept();
    } else {
        q->QAbstractScrollArea::mousePressEvent(mousePressEvent);
    }
}

void AbstractByteArrayViewPrivate::mouseMoveEvent(QMouseEvent* mouseMoveEvent)
{
    Q_Q(AbstractByteArrayView);

    if (mMouseController->handleMouseMoveEvent(mouseMoveEvent)) {
        mouseMoveEvent->accept();
    } else {
        q->QAbstractScrollArea::mouseMoveEvent(mouseMoveEvent);
    }
}

void AbstractByteArrayViewPrivate::mouseReleaseEvent(QMouseEvent* mouseReleaseEvent)
{
    Q_Q(AbstractByteArrayView);

    if (mMouseController->handleMouseReleaseEvent(mouseReleaseEvent)) {
        mouseReleaseEvent->accept();
    } else {
        q->QAbstractScrollArea::mouseReleaseEvent(mouseReleaseEvent);
    }
}

// gets called after press and release instead of a plain press event (?)
void AbstractByteArrayViewPrivate::mouseDoubleClickEvent(QMouseEvent* mouseDoubleClickEvent)
{
    Q_Q(AbstractByteArrayView);

    if (mMouseController->handleMouseDoubleClickEvent(mouseDoubleClickEvent)) {
        mouseDoubleClickEvent->accept();
    } else {
        q->QAbstractScrollArea::mouseDoubleClickEvent(mouseDoubleClickEvent);
    }
}

bool AbstractByteArrayViewPrivate::event(QEvent* event)
{
    Q_Q(AbstractByteArrayView);

    //
    if (event->type() == QEvent::KeyPress) {
        auto* keyEvent = static_cast<QKeyEvent*>(event);
        if (keyEvent->key() == Qt::Key_Tab || keyEvent->key() == Qt::Key_Backtab) {
            q->keyPressEvent(keyEvent);
            if (keyEvent->isAccepted()) {
                return true;
            }
        }
    } else if ((event->type() == QEvent::MouseMove) ||
               (event->type() == QEvent::MouseButtonPress) ||
               (event->type() == QEvent::MouseButtonRelease)) {
        // discard any events synthesized from touch input
        auto* mouseEvent = static_cast<QMouseEvent*>(event);
        if (mouseEvent->source() == Qt::MouseEventSynthesizedByQt) {
            event->accept();
            return true;
        }
    } else if ((event->type() == QEvent::PaletteChange)) {
        if (mCursorVisible) {
            updateCursors();
        }
    } else if ((event->type() == QEvent::ContextMenu) &&
               (static_cast<QContextMenuEvent*>(event)->reason() == QContextMenuEvent::Keyboard)) {
        ensureCursorVisible();

        const QPoint cursorPos = cursorRect().center();
        QContextMenuEvent adaptedContextMenuEvent(QContextMenuEvent::Keyboard, cursorPos,
                                                  q->viewport()->mapToGlobal(cursorPos));
        adaptedContextMenuEvent.setAccepted(event->isAccepted());

        const bool result = q->QAbstractScrollArea::event(&adaptedContextMenuEvent);
        event->setAccepted(adaptedContextMenuEvent.isAccepted());

        return result;
    } else if (event->type() == QEvent::Gesture) {
        auto* gestureEvent = static_cast<QGestureEvent*>(event);
        if (auto* tapGesture = static_cast<QTapGesture*>(gestureEvent->gesture(Qt::TapGesture))) {
            return mTapNavigator.handleTapGesture(tapGesture);
        } else if (auto* tapAndHoldGesture = static_cast<TouchOnlyTapAndHoldGesture*>(gestureEvent->gesture(touchOnlyTapAndHoldGestureType()))) {
            if (tapAndHoldGesture->state() == Qt::GestureFinished) {
                const QPoint viewPortPos = tapAndHoldGesture->position().toPoint();
                const QPoint pos = viewPortPos + q->viewport()->pos();
                // TODO: QScrollArea for some reason only deals with QContextMenuEvent::Keyboard, ignores others?
                // case QEvent::ContextMenu:
                //     if (static_cast<QContextMenuEvent *>(e)->reason() == QContextMenuEvent::Keyboard)
                //         return QFrame::event(e);
                //     e->ignore();
                //     break;
                // why that? QFrame delegates to QWidget, which does the normal policy dance
                // context menu
                QContextMenuEvent simulatedContextMenuEvent(QContextMenuEvent::Keyboard, pos,
                                                          q->viewport()->mapToGlobal(viewPortPos));
                simulatedContextMenuEvent.setAccepted(event->isAccepted());

                const bool result = q->QAbstractScrollArea::event(&simulatedContextMenuEvent);
                event->setAccepted(simulatedContextMenuEvent.isAccepted());

                return result;
            }
        } else if (auto* pinchGesture = static_cast<QPinchGesture*>(gestureEvent->gesture(Qt::PinchGesture))) {
            return mZoomPinchController.handlePinchGesture(pinchGesture);
        };
    }

    ColumnsViewScrollAreaEngine::event(event);

    return q->QAbstractScrollArea::event(event);
}

bool AbstractByteArrayViewPrivate::viewportEvent(QEvent* event)
{
    Q_Q(AbstractByteArrayView);

    if (event->type() == QEvent::ToolTip) {
        auto* helpEvent = static_cast<QHelpEvent*>(event);

        QString toolTip;

        Bookmarkable* bookmarks = qobject_cast<Bookmarkable*>(mByteArrayModel);
        if (bookmarks) {
            const Address index = indexByPoint(viewportToColumns(helpEvent->pos()));
            if (index != -1) {
                if (bookmarks->containsBookmarkFor(index)) {
                    toolTip = bookmarks->bookmarkFor(index).name();
                }
            }
        }

        if (!toolTip.isNull()) {
            QToolTip::showText(helpEvent->globalPos(), toolTip);
        } else {
            QToolTip::hideText();
            event->ignore();
        }

        return true;
    } else if ((event->type() == QEvent::MouseMove) ||
               (event->type() == QEvent::MouseButtonPress) ||
               (event->type() == QEvent::MouseButtonRelease)) {
        // discard any events synthesized from touch input
        auto* mouseEvent = static_cast<QMouseEvent*>(event);
        if (mouseEvent->source() == Qt::MouseEventSynthesizedByQt) {
            event->accept();
            return true;
        }
    }

    return q->QAbstractScrollArea::viewportEvent(event);
}

void AbstractByteArrayViewPrivate::resizeEvent(QResizeEvent* resizeEvent)
{
    Q_Q(AbstractByteArrayView);

    if (mResizeStyle != AbstractByteArrayView::FixedLayoutStyle) {
        // changes?
        if (mTableLayout.setNoOfBytesPerLine(fittingBytesPerLine())) {
            setNoOfLines(mTableLayout.noOfLines());
            updateViewByWidth();
        }
    }

    ColumnsViewScrollAreaEngine::resizeEvent(resizeEvent);

    q->QAbstractScrollArea::resizeEvent(resizeEvent);

    mTableLayout.setNoOfLinesPerPage(noOfLinesPerPage());   // TODO: doesn't work with the new size!!!
}

void AbstractByteArrayViewPrivate::focusInEvent(QFocusEvent* focusEvent)
{
    Q_Q(AbstractByteArrayView);

    q->QAbstractScrollArea::focusInEvent(focusEvent);
    startCursor();

    const Qt::FocusReason focusReason = focusEvent->reason();
    if (focusReason != Qt::ActiveWindowFocusReason
        && focusReason != Qt::PopupFocusReason) {
        Q_EMIT q->focusChanged(true);
    }
}

void AbstractByteArrayViewPrivate::focusOutEvent(QFocusEvent* focusEvent)
{
    Q_Q(AbstractByteArrayView);

    stopCursor();
    q->QAbstractScrollArea::focusOutEvent(focusEvent);

    const Qt::FocusReason focusReason = focusEvent->reason();
    if (focusReason != Qt::ActiveWindowFocusReason
        && focusReason != Qt::PopupFocusReason) {
        Q_EMIT q->focusChanged(false);
    }
}

void AbstractByteArrayViewPrivate::dragEnterEvent(QDragEnterEvent* dragEnterEvent)
{
    if (mDropper.handleDragEnterEvent(dragEnterEvent)) {
        dragEnterEvent->accept();
    } else {
        dragEnterEvent->ignore();
    }
}

void AbstractByteArrayViewPrivate::dragMoveEvent(QDragMoveEvent* dragMoveEvent)
{
    if (mDropper.handleDragMoveEvent(dragMoveEvent)) {
        dragMoveEvent->accept();
    } else {
        dragMoveEvent->ignore();
    }
}

void AbstractByteArrayViewPrivate::dragLeaveEvent(QDragLeaveEvent* dragLeaveEvent)
{
    if (mDropper.handleDragLeaveEvent(dragLeaveEvent)) {
        dragLeaveEvent->accept();
    } else {
        dragLeaveEvent->ignore();
    }
}

void AbstractByteArrayViewPrivate::dropEvent(QDropEvent* dropEvent)
{
    if (mDropper.handleDropEvent(dropEvent)) {
        dropEvent->accept();
    } else {
        dropEvent->ignore();
    }
}

void AbstractByteArrayViewPrivate::contextMenuEvent(QContextMenuEvent* contextMenuEvent)
{
    QMenu* menu = createStandardContextMenu(contextMenuEvent->pos());
    menu->setAttribute(Qt::WA_DeleteOnClose);

    menu->popup(contextMenuEvent->globalPos());
}

void AbstractByteArrayViewPrivate::timerEvent(QTimerEvent* timerEvent)
{
    Q_Q(AbstractByteArrayView);

    if (timerEvent->timerId() == mCursorBlinkTimerId) {
        blinkCursor();
    }

    q->QAbstractScrollArea::timerEvent(timerEvent);
}

void AbstractByteArrayViewPrivate::onBookmarksChange(const QVector<Bookmark>& bookmarks)
{
    for (const Bookmark& bookmark : bookmarks) {
        const Address position = bookmark.offset();
        mTableRanges.addChangedRange(position, position);
    }

    updateChanged();
    unpauseCursor();
}

void AbstractByteArrayViewPrivate::onRevertedToVersionIndex(int versionIndex)
{
    Q_UNUSED(versionIndex)
    // TODO: only call this if needed
    cancelByteEditor();
}

void AbstractByteArrayViewPrivate::onByteArrayReadOnlyChange(bool isByteArrayReadOnly)
{
    Q_Q(AbstractByteArrayView);

    adaptController();

    if (!mReadOnly) {
        Q_EMIT q->readOnlyChanged(isByteArrayReadOnly);
    }
}

void AbstractByteArrayViewPrivate::onContentsChanged(const ArrayChangeMetricsList& changeList)
{
    pauseCursor();

    const bool atEnd = mTableCursor.atEnd();
    const Size oldLength = mTableLayout.length(); // TODO: hack for mDataCursor->adaptSelectionToChange
    // update lengths
    int oldNoOfLines = noOfLines();
    mTableLayout.setLength(mByteArrayModel->size());
    const int newNoOfLines = mTableLayout.noOfLines();
    if (oldNoOfLines != newNoOfLines) {
        setNoOfLines(newNoOfLines);
        const LineRange changedLines = (oldNoOfLines < newNoOfLines) ?
                                       LineRange(oldNoOfLines, newNoOfLines - 1) :
                                       LineRange(newNoOfLines, oldNoOfLines - 1);
        mTableRanges.addChangedOffsetLines(changedLines);
    }

    // adapt cursor(s)
    if (atEnd) {
        mTableCursor.gotoEnd();
    } else {
        mTableCursor.adaptToChanges(changeList, oldLength);
    }

    mTableRanges.adaptToChanges(changeList, oldLength);
    // qCDebug(LOG_OKTETA_GUI) << "Cursor:"<<mDataCursor->index()<<", selection:"<<mTableRanges.selectionStart()<<"-"<<mTableRanges.selectionEnd()
    //          <<", BytesPerLine: "<<mTableLayout.noOfBytesPerLine()<<endl;
    ensureCursorVisible();

    endViewUpdate();
}

void AbstractByteArrayViewPrivate::endViewUpdate()
{
    updateChanged();

    unpauseCursor();

    emitSelectionUpdates();
}

void AbstractByteArrayViewPrivate::emitSelectionUpdates()
{
    Q_Q(AbstractByteArrayView);

    bool selectionChanged = false;
    bool hasSelectionChanged = false;
    mTableRanges.takeHasSelectionChanged(&hasSelectionChanged, &selectionChanged);

    if (selectionChanged) {
        Q_EMIT q->selectionChanged(mTableRanges.selection());
    }
    if (hasSelectionChanged) {
        const bool hasSelection = mTableRanges.hasSelection();
        if (!mOverWrite) {
            Q_EMIT q->cutAvailable(hasSelection);
        }
        Q_EMIT q->copyAvailable(hasSelection);
        Q_EMIT q->hasSelectedDataChanged(hasSelection);
    }
    Q_EMIT q->cursorPositionChanged(cursorPosition());
}

void AbstractByteArrayViewPrivate::emitCursorPositionChanged()
{
    Q_Q(AbstractByteArrayView);

    Q_EMIT q->cursorPositionChanged(cursorPosition());
}

void AbstractByteArrayViewPrivate::emitDoubleClicked(Address index)
{
    Q_Q(AbstractByteArrayView);

    Q_EMIT q->doubleClicked(index);
}

void AbstractByteArrayViewPrivate::setMouseCursor(Qt::CursorShape cursorShape)
{
    Q_Q(AbstractByteArrayView);

    q->viewport()->setCursor(cursorShape);
}

QPoint AbstractByteArrayViewPrivate::mapViewportFromGlobal(QPoint pos) const
{
    Q_Q(const AbstractByteArrayView);

    return q->viewport()->mapFromGlobal(pos);
}

#if 0
void AbstractByteArrayViewPrivate::onClipboardChanged()
{
    Q_Q(AbstractByteArrayView);

    // don't listen to selection changes
    q->disconnect(QApplication::clipboard(), SIGNAL(selectionChanged()));
    selectAll(false);
}
#endif

}

#include "abstractbytearrayview_p.moc"
