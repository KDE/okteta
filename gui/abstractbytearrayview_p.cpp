/*
    This file is part of the Okteta Gui library, made within the KDE community.

    SPDX-FileCopyrightText: 2008-2010 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "abstractbytearrayview_p.hpp"

// lib
#include "controller/undoredocontroller.hpp"
#include "controller/clipboardcontroller.hpp"
#include "controller/keynavigator.hpp"
#include "controller/chareditor.hpp"
#include "controller/dropper.hpp"
#include "controller/mousenavigator.hpp"
#include "controller/mousepaster.hpp"
#include "controller/zoomwheelcontroller.hpp"
#include "widgetcolumnstylist.hpp"
#include "cursor.hpp"
#include "bordercolumnrenderer.hpp"
#include "oktetagui.hpp"
// Okteta core
#include <Okteta/ValueCodec>
#include <Okteta/Bookmarkable>
#include <Okteta/Versionable>
#include <Okteta/TextByteArrayAnalyzer>
#include <Okteta/Bookmark>
// KF
#include <KLocalizedString>
// Qt
#include <QPainter>
#include <QDragEnterEvent>
#include <QDragMoveEvent>
#include <QDragLeaveEvent>
#include <QDropEvent>
#include <QApplication>
#include <QToolTip>
#include <QMimeData>
#include <QMenu>
#include <QIcon>
#include <QTimer>

namespace Okteta {

static constexpr Address DefaultStartOffset = 0;// 5;
static constexpr Address DefaultFirstLineOffset = 0;

// zooming is done in steps of font size points
static constexpr int DefaultZoomStep = 1;
static constexpr int MinFontPointSize = 4;
static constexpr int MaxFontPointSize = 128;

static constexpr AbstractByteArrayView::ValueCoding DefaultValueCoding =  AbstractByteArrayView::HexadecimalCoding;
static constexpr AbstractByteArrayView::CharCoding DefaultCharCoding = AbstractByteArrayView::LocalEncoding;

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

AbstractByteArrayViewPrivate::AbstractByteArrayViewPrivate(AbstractByteArrayView* parent)
    : ColumnsViewPrivate(parent)
    , mByteArrayModel(nullModel())
    , mTableLayout(new ByteArrayTableLayout(DefaultNoOfBytesPerLine, DefaultFirstLineOffset, DefaultStartOffset, 0, 0))
    , mTableCursor(new ByteArrayTableCursor(mTableLayout))
    , mTableRanges(new ByteArrayTableRanges(mTableLayout))
    , mCursorPixmaps(new Cursor())
    , mReadOnly(false)
    , mOverWriteOnly(false)
    , mOverWrite(true)
    , mInZooming(false)
    , mCursorPaused(false)
    , mBlinkCursorVisible(false)
    // , mDefaultFontSize( p->font().pointSize() ) crashes in font()
    , mResizeStyle(DefaultResizeStyle)
{
}

AbstractByteArrayViewPrivate::~AbstractByteArrayViewPrivate()
{
    delete mDropper;

    delete mZoomWheelController;

    delete mMousePaster;
    delete mMouseNavigator;

    delete mCharEditor;
    delete mValueEditor;
    delete mKeyNavigator;
    delete mClipboardController;
    delete mUndoRedoController;
    delete mTabController;

    delete mStylist;

    delete mTableRanges;
    delete mTableCursor;
    delete mTableLayout;
    delete mValueCodec;
    delete mCharCodec;

    delete mCursorPixmaps;
}

void AbstractByteArrayViewPrivate::init()
{
    Q_Q(AbstractByteArrayView);

    // initialize layout
    mTableLayout->setLength(mByteArrayModel->size());
    mTableLayout->setNoOfLinesPerPage(q->noOfLinesPerPage());

    mStylist = new WidgetColumnStylist(q);

    mOffsetColumn =
        new OffsetColumnRenderer(mStylist, mTableLayout, OffsetFormat::Hexadecimal);
    mOffsetBorderColumn =
        new BorderColumnRenderer(mStylist, false);

    mValueCodec = ValueCodec::createCodec((ValueCoding)DefaultValueCoding);
    mValueCoding = DefaultValueCoding;
    mCharCodec = CharCodec::createCodec((CharCoding)DefaultCharCoding);
    mCharCoding = DefaultCharCoding;

    mTabController = new TabController(q, nullptr);
    mUndoRedoController = new UndoRedoController(q, mTabController);
    mClipboardController = new ClipboardController(q, mUndoRedoController);
    mKeyNavigator = new KeyNavigator(q, mClipboardController);
    mValueEditor = new ValueEditor(mTableCursor, q, mKeyNavigator);
    mCharEditor = new CharEditor(mTableCursor, q, mKeyNavigator);

    mMousePaster = new MousePaster(q, nullptr);
    mMouseNavigator = new MouseNavigator(q, mMousePaster);
    mMouseController = mMouseNavigator;

    mZoomWheelController = new ZoomWheelController(q, nullptr);
    mDropper = new Dropper(q);

    setWheelController(mZoomWheelController);

    mCursorBlinkTimer = new QTimer(q);

    QObject::connect(mCursorBlinkTimer, &QTimer::timeout,
                     q, [&]() { blinkCursor(); });

    q->setAcceptDrops(true);
}

void AbstractByteArrayViewPrivate::setByteArrayModel(AbstractByteArrayModel* byteArrayModel)
{
    Q_Q(AbstractByteArrayView);

    mByteArrayModel->disconnect(q);
    mCursorPaused = true;

    mByteArrayModel = byteArrayModel ? byteArrayModel : nullModel();

    // affected:
    // length -> no of lines -> width
    mTableLayout->setLength(mByteArrayModel->size());
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

    mTableCursor->gotoStart();
    ensureCursorVisible();
    unpauseCursor();

    emit q->cursorPositionChanged(cursorPosition());
}

void AbstractByteArrayViewPrivate::toggleOffsetColumn(bool showOffsetColumn)
{
    Q_Q(AbstractByteArrayView);

    const bool isVisible = mOffsetColumn->isVisible();
    // no change?
    if (isVisible == showOffsetColumn) {
        return;
    }

    mOffsetColumn->setVisible(showOffsetColumn);

    updateViewByWidth();

    emit q->offsetColumnVisibleChanged(showOffsetColumn);
}

void AbstractByteArrayViewPrivate::setOffsetCoding(AbstractByteArrayView::OffsetCoding offsetCoding)
{
    Q_Q(AbstractByteArrayView);

    const OffsetFormat::Format format = static_cast<OffsetFormat::Format>(offsetCoding);
    const OffsetFormat::Format currentFormat = mOffsetColumn->format();
    // no change?
    if (currentFormat == format) {
        return;
    }

    mOffsetColumn->setFormat(format, q->fontMetrics());

    updateViewByWidth();

    emit q->offsetCodingChanged(offsetCoding);
}

void AbstractByteArrayViewPrivate::changeEvent(QEvent* event)
{
    Q_Q(AbstractByteArrayView);

    q->ColumnsView::changeEvent(event);

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

    emit q->zoomLevelChanged(mZoomLevel);
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

    emit q->zoomLevelChanged(mZoomLevel);
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

    emit q->zoomLevelChanged(mZoomLevel);
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

    emit q->zoomLevelChanged(mZoomLevel);
}

void AbstractByteArrayViewPrivate::setStartOffset(Address startOffset)
{
    Q_Q(AbstractByteArrayView);

    if (!mTableLayout->setStartOffset(startOffset)) {
        return;
    }

    pauseCursor();
    // affects:
    // the no of lines -> width
    adjustLayoutToSize();

    q->viewport()->update();

    mTableCursor->updateCoord();
    ensureCursorVisible();
    unpauseCursor();
    emit q->cursorPositionChanged(cursorPosition());
}

void AbstractByteArrayViewPrivate::setFirstLineOffset(Address firstLineOffset)
{
    Q_Q(AbstractByteArrayView);

    if (!mTableLayout->setFirstLineOffset(firstLineOffset)) {
        return;
    }

    pauseCursor();
    // affects:
    // the no of lines -> width
    adjustLayoutToSize();

    q->viewport()->update();

    mTableCursor->updateCoord();
    ensureCursorVisible();
    unpauseCursor();
    emit q->cursorPositionChanged(cursorPosition());
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

    emit q->layoutStyleChanged(mResizeStyle);
}

void AbstractByteArrayViewPrivate::setNoOfBytesPerLine(int noOfBytesPerLine)
{
    Q_Q(AbstractByteArrayView);

    // if the number is explicitly set we expect a wish for no automatic resize
    setLayoutStyle(AbstractByteArrayView::FixedLayoutStyle);

    if (!mTableLayout->setNoOfBytesPerLine(noOfBytesPerLine)) {
        return;
    }

    updateViewByWidth();

    emit q->noOfBytesPerLineChanged(mTableLayout->noOfBytesPerLine());
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
        emit q->readOnlyChanged(mReadOnly);
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

    mTableCursor->setAppendPosEnabled(!mOverWrite);

    if (changeCursor) {
        unpauseCursor();
    }

    emit q->overwriteModeChanged(mOverWrite);
    emit q->cutAvailable(!mOverWrite && mTableRanges->hasSelection());
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

    ValueCodec* newValueCodec
        = ValueCodec::createCodec((ValueCoding)valueCoding);
    if (!newValueCodec) {
        return;
    }

    delete mValueCodec;
    mValueCodec = newValueCodec;
    mValueCoding = valueCoding;
}
void AbstractByteArrayViewPrivate::setCharCoding(AbstractByteArrayView::CharCoding charCoding)
{
    if (mCharCoding == charCoding) {
        return;
    }

    CharCodec* newCharCodec
        = CharCodec::createCodec((CharCoding)charCoding);
    if (!newCharCodec) {
        return;
    }

    delete mCharCodec;
    mCharCodec = newCharCodec;
    mCharCoding = charCoding;
}
void AbstractByteArrayViewPrivate::setCharCoding(const QString& charCodingName)
{
    if (mCharCodec->name() == charCodingName) {
        return;
    }

    CharCodec* newCharCodec =
        CharCodec::createCodec(charCodingName);
    if (!newCharCodec) {
        return;
    }

    delete mCharCodec;
    mCharCodec = newCharCodec;
    mCharCoding = AbstractByteArrayView::LocalEncoding; // TODO: add encoding no to every known codec
}

void AbstractByteArrayViewPrivate::setMarking(const AddressRange& _marking)
{
    AddressRange marking(_marking);
    marking.restrictEndTo(mTableLayout->length() - 1);

    const AddressRange oldMarking = mTableRanges->marking();

    if (marking == oldMarking) {
        return;
    }

    mTableRanges->setMarking(marking);

    updateChanged();
}

// TODO: make this use select( start, end )
bool AbstractByteArrayViewPrivate::selectWord(Address index)
{
    Q_Q(AbstractByteArrayView);

    bool result = false;

    if (0 <= index && index < mTableLayout->length()) {
        const TextByteArrayAnalyzer textAnalyzer(mByteArrayModel, mCharCodec);
        const AddressRange wordSection = textAnalyzer.wordSection(index);
        if (wordSection.isValid()) {
            const bool oldHasSelection = mTableRanges->hasSelection();

            pauseCursor();
            finishByteEditor();

            mTableRanges->setFirstWordSelection(wordSection);
            mTableCursor->gotoIndex(wordSection.nextBehindEnd());
            updateChanged();

            unpauseCursor();

            const bool newHasSelection = mTableRanges->hasSelection();
            emit q->selectionChanged(wordSection);
            if (oldHasSelection != newHasSelection) {
                if (!mOverWrite) {
                    emit q->cutAvailable(newHasSelection);
                }
                emit q->copyAvailable(newHasSelection);
                emit q->hasSelectedDataChanged(newHasSelection);
            }
            emit q->cursorPositionChanged(cursorPosition());

            result = true;
        }
    }
    return result;
}

// TODO: make this use select( start, end )
void AbstractByteArrayViewPrivate::selectAll(bool select)
{
    Q_Q(AbstractByteArrayView);

    const bool oldHasSelection = mTableRanges->hasSelection();

    pauseCursor();
    finishByteEditor();

    if (select) {
        mTableRanges->setSelection(AddressRange(0, mTableLayout->length() - 1));
        mTableCursor->gotoEnd();
    } else {
        mTableRanges->removeSelection();
    }

    updateChanged();

    unpauseCursor();

    const bool newHasSelection = mTableRanges->hasSelection();
    emit q->selectionChanged(mTableRanges->selection());
    if (oldHasSelection != newHasSelection) {
        if (!mOverWrite) {
            emit q->cutAvailable(newHasSelection);
        }
        emit q->copyAvailable(newHasSelection);
        emit q->hasSelectedDataChanged(newHasSelection);
    }
    emit q->cursorPositionChanged(cursorPosition());
}

void AbstractByteArrayViewPrivate::setCursorPosition(Address index, bool behind)
{
    Q_Q(AbstractByteArrayView);

    const bool oldHasSelection = mTableRanges->hasSelection();

    pauseCursor();
    finishByteEditor();

    if (behind) {
        --index;
    }
    mTableCursor->gotoCIndex(index);
    if (behind) {
        mTableCursor->stepBehind();
    }

    mTableRanges->removeSelection();
    if (mTableRanges->isModified()) {
        updateChanged();

        const bool newHasSelection = mTableRanges->hasSelection();
        emit q->selectionChanged(mTableRanges->selection());
        if (oldHasSelection != newHasSelection) {
            if (!mOverWrite) {
                emit q->cutAvailable(newHasSelection);
            }
            emit q->copyAvailable(newHasSelection);
            emit q->hasSelectedDataChanged(newHasSelection);
        }
    }
    ensureCursorVisible();

    unpauseCursor();
    emit q->cursorPositionChanged(cursorPosition());
}

void AbstractByteArrayViewPrivate::setSelectionCursorPosition(Address index)
{
    Q_Q(AbstractByteArrayView);

    pauseCursor();
    finishByteEditor();

    if (!mTableRanges->selectionStarted()) {
        mTableRanges->setSelectionStart(mTableCursor->realIndex());
    }

    mTableCursor->gotoCIndex(index);

    mTableRanges->setSelectionEnd(mTableCursor->realIndex());

    ensureCursorVisible();
    updateChanged();

    unpauseCursor();

    if (mTableRanges->isModified()) {
        q->emitSelectionSignals(); // TODO: can this be moved somewhere
    }
    emit q->cursorPositionChanged(mTableCursor->realIndex());
}

void AbstractByteArrayViewPrivate::setSelection(const AddressRange& _selection)
{
    Q_Q(AbstractByteArrayView);

    AddressRange selection(_selection);
    selection.restrictEndTo(mTableLayout->length() - 1);

    const AddressRange oldSelection = mTableRanges->selection();

    if (!selection.isValid()
        || selection == oldSelection) {
        return;
    }

    pauseCursor();
    finishByteEditor();

    mTableRanges->setSelection(selection);
    mTableCursor->gotoCIndex(selection.nextBehindEnd());
// TODO:            ensureVisible( *mActiveColumn, mTableLayout->coordOfIndex(selection.start()) );
    ensureCursorVisible();
    updateChanged();

    unpauseCursor();

    emit q->selectionChanged(selection);
    if (oldSelection.isEmpty()) {
        if (!mOverWrite) {
            emit q->cutAvailable(true);
        }
        emit q->copyAvailable(true);
        emit q->hasSelectedDataChanged(true);
    }
    emit q->cursorPositionChanged(cursorPosition());
}

QByteArray AbstractByteArrayViewPrivate::selectedData() const
{
    if (!mTableRanges->hasSelection()) {
        return {};
    }

    const AddressRange selection = mTableRanges->selection();
    QByteArray data;
    data.resize(selection.width());
    byteArrayModel()->copyTo(reinterpret_cast<Byte*>(data.data()), selection.start(), selection.width());
    return data;
}

QMimeData* AbstractByteArrayViewPrivate::selectionAsMimeData() const
{
    if (!mTableRanges->hasSelection()) {
        return nullptr;
    }

    auto* mimeData = new QMimeData;
    mimeData->setData(octetStreamFormatName(), selectedData());
    return mimeData;
}

void AbstractByteArrayViewPrivate::cutToClipboard(QClipboard::Mode mode)
{
    if (isEffectiveReadOnly() || mOverWrite) {
        return;
    }

    QMimeData* cutData = selectionAsMimeData();
    if (!cutData) {
        return;
    }

    QApplication::clipboard()->setMimeData(cutData, mode);

    removeSelectedData();
}

void AbstractByteArrayViewPrivate::copyToClipboard(QClipboard::Mode mode) const
{
    QMimeData* cutData = selectionAsMimeData();
    if (!cutData) {
        return;
    }

//     if( mode == QClipboard::Selection )
//         q->disconnect( QApplication::clipboard(), SIGNAL(selectionChanged()) );

    QApplication::clipboard()->setMimeData(cutData, mode);

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

    const bool oldHasSelection = mTableRanges->hasSelection();

    if (mOverWrite) {
        Size lengthOfInserted;
        if (mTableRanges->hasSelection()) {
            // replacing the selection:
            // we restrict the replacement to the minimum length of selection and input
            AddressRange selection = mTableRanges->removeSelection();
            selection.restrictEndByWidth(data.size());
            lengthOfInserted = mByteArrayModel->replace(selection, reinterpret_cast<const Byte*>(data.constData()), selection.width());
        } else {
            const Size length = mTableLayout->length();
            if (!isCursorBehind() && length > 0) {
                // replacing the normal data, at least until the end
                AddressRange insertRange = AddressRange::fromWidth(cursorPosition(), data.size());
                insertRange.restrictEndTo(length - 1);
                lengthOfInserted = mByteArrayModel->replace(insertRange, reinterpret_cast<const Byte*>(data.constData()), insertRange.width());
            } else {
                lengthOfInserted = 0;
            }
        }
        // if inserting ourself we want to place the cursor at the end of the inserted data
        if (lengthOfInserted > 0) {
            pauseCursor();
            mTableCursor->gotoNextByte(lengthOfInserted);
            unpauseCursor();
            emit q->cursorPositionChanged(cursorPosition());
        }
    } else {
        if (mTableRanges->hasSelection()) {
            // replacing the selection
            const AddressRange selection = mTableRanges->removeSelection();
            mByteArrayModel->replace(selection, data);
        } else {
            mByteArrayModel->insert(cursorPosition(), data);
        }
    }

    const bool newHasSelection = mTableRanges->hasSelection();
    emit q->selectionChanged(mTableRanges->selection());
    if (oldHasSelection != newHasSelection) {
        emit q->hasSelectedDataChanged(newHasSelection);
    }
}

void AbstractByteArrayViewPrivate::removeSelectedData()
{
    // Can't we do this?
    if (isEffectiveReadOnly() || mOverWrite) { // TODO: || mValueEditor->isInEditMode() )
        return;
    }

    const AddressRange selection = mTableRanges->removeSelection();

    mByteArrayModel->remove(selection);

//     clearUndoRedo();

    // emit q->selectionChanged( -1, -1 );
}

bool AbstractByteArrayViewPrivate::getNextChangedRange(CoordRange* changedRange, const CoordRange& visibleRange) const
{
    const bool result = mTableRanges->overlapsChanges(visibleRange, changedRange);

    if (result) {
        changedRange->restrictTo(visibleRange);
    }

    return result;
}

void AbstractByteArrayViewPrivate::adaptController()
{
    AbstractController* controller =
        isEffectiveReadOnly() ?                                 static_cast<AbstractController*>(mKeyNavigator) :
        activeCoding() == AbstractByteArrayView::CharCodingId ? static_cast<AbstractController*>(mCharEditor) :
                                                                static_cast<AbstractController*>(mValueEditor);
    setController(controller);
}

void AbstractByteArrayViewPrivate::updateViewByWidth()
{
    Q_Q(AbstractByteArrayView);

    pauseCursor();

    adjustToLayoutNoOfBytesPerLine();
    adjustLayoutToSize();

    q->viewport()->update();

    mTableCursor->updateCoord();
    // TODO: see for what actions if would be usable to have ensureCursorVisible()
    // TODO: think about adding action "showCursor"/"show FocusItem" to menu
    // disabled as this prevents splitting views with aligned areas from working
    // ensureCursorVisible();

    unpauseCursor();
    emit q->cursorPositionChanged(cursorPosition());
}

void AbstractByteArrayViewPrivate::adjustLayoutToSize()
{
    Q_Q(AbstractByteArrayView);

    // check whether there is a change with the numbers of fitting bytes per line
    if (mResizeStyle != AbstractByteArrayView::FixedLayoutStyle) {
        // changes?
        if (mTableLayout->setNoOfBytesPerLine(fittingBytesPerLine())) {
            adjustToLayoutNoOfBytesPerLine();
        }
    }

    q->setNoOfLines(mTableLayout->noOfLines());
}

void AbstractByteArrayViewPrivate::startCursor()
{
    mCursorPaused = false;

    updateCursors();

    mCursorBlinkTimer->start(QApplication::cursorFlashTime() / 2);
}

void AbstractByteArrayViewPrivate::stopCursor()
{
    mCursorBlinkTimer->stop();

    pauseCursor();
}

void AbstractByteArrayViewPrivate::unpauseCursor()
{
    mCursorPaused = false;

    if (mCursorBlinkTimer->isActive()) {
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
    Q_UNUSED(position);

    Q_Q(AbstractByteArrayView);

    auto menu = new QMenu(q);

    if (mUndoRedoController->addContextMenuActions(menu) > 0) {
        menu->addSeparator();
    }

    if (mClipboardController->addContextMenuActions(menu) > 0) {
        menu->addSeparator();
    }

    mKeyNavigator->addContextMenuActions(menu);

    return menu;
}

void AbstractByteArrayViewPrivate::mousePressEvent(QMouseEvent* mouseEvent)
{
    Q_Q(AbstractByteArrayView);

    if (mMouseController->handleMousePressEvent(mouseEvent)) {
        mouseEvent->accept();
    } else {
        q->ColumnsView::mousePressEvent(mouseEvent);
    }
}

void AbstractByteArrayViewPrivate::mouseMoveEvent(QMouseEvent* mouseEvent)
{
    Q_Q(AbstractByteArrayView);

    if (mMouseController->handleMouseMoveEvent(mouseEvent)) {
        mouseEvent->accept();
    } else {
        q->ColumnsView::mouseMoveEvent(mouseEvent);
    }
}

void AbstractByteArrayViewPrivate::mouseReleaseEvent(QMouseEvent* mouseEvent)
{
    Q_Q(AbstractByteArrayView);

    if (mMouseController->handleMouseReleaseEvent(mouseEvent)) {
        mouseEvent->accept();
    } else {
        q->ColumnsView::mouseReleaseEvent(mouseEvent);
    }
}

// gets called after press and release instead of a plain press event (?)
void AbstractByteArrayViewPrivate::mouseDoubleClickEvent(QMouseEvent* mouseEvent)
{
    Q_Q(AbstractByteArrayView);

    if (mMouseController->handleMouseDoubleClickEvent(mouseEvent)) {
        mouseEvent->accept();
    } else {
        q->ColumnsView::mouseDoubleClickEvent(mouseEvent);
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
    } else if ((event->type() == QEvent::ContextMenu) &&
               (static_cast<QContextMenuEvent*>(event)->reason() == QContextMenuEvent::Keyboard)) {
        ensureCursorVisible();

        const QPoint cursorPos = cursorRect().center();
        QContextMenuEvent adaptedContextMenuEvent(QContextMenuEvent::Keyboard, cursorPos,
                                                  q->viewport()->mapToGlobal(cursorPos));
        adaptedContextMenuEvent.setAccepted(event->isAccepted());

        const bool result = q->ColumnsView::event(&adaptedContextMenuEvent);
        event->setAccepted(adaptedContextMenuEvent.isAccepted());

        return result;
    }

    return q->ColumnsView::event(event);
}

bool AbstractByteArrayViewPrivate::viewportEvent(QEvent* event)
{
    Q_Q(AbstractByteArrayView);

    if (event->type() == QEvent::ToolTip) {
        auto* helpEvent = static_cast<QHelpEvent*>(event);

        QString toolTip;

        Bookmarkable* bookmarks = qobject_cast<Bookmarkable*>(mByteArrayModel);
        if (bookmarks) {
            const Address index = indexByPoint(q->viewportToColumns(helpEvent->pos()));
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
    }

    return q->ColumnsView::viewportEvent(event);
}

void AbstractByteArrayViewPrivate::resizeEvent(QResizeEvent* resizeEvent)
{
    Q_Q(AbstractByteArrayView);

    if (mResizeStyle != AbstractByteArrayView::FixedLayoutStyle) {
        // changes?
        if (mTableLayout->setNoOfBytesPerLine(fittingBytesPerLine())) {
            q->setNoOfLines(mTableLayout->noOfLines());
            updateViewByWidth();
        }
    }

    q->ColumnsView::resizeEvent(resizeEvent);

    mTableLayout->setNoOfLinesPerPage(q->noOfLinesPerPage());   // TODO: doesn't work with the new size!!!
}

void AbstractByteArrayViewPrivate::focusInEvent(QFocusEvent* focusEvent)
{
    Q_Q(AbstractByteArrayView);

    q->ColumnsView::focusInEvent(focusEvent);
    startCursor();

    const Qt::FocusReason focusReason = focusEvent->reason();
    if (focusReason != Qt::ActiveWindowFocusReason
        && focusReason != Qt::PopupFocusReason) {
        emit q->focusChanged(true);
    }
}

void AbstractByteArrayViewPrivate::focusOutEvent(QFocusEvent* focusEvent)
{
    Q_Q(AbstractByteArrayView);

    stopCursor();
    q->ColumnsView::focusOutEvent(focusEvent);

    const Qt::FocusReason focusReason = focusEvent->reason();
    if (focusReason != Qt::ActiveWindowFocusReason
        && focusReason != Qt::PopupFocusReason) {
        emit q->focusChanged(false);
    }
}

void AbstractByteArrayViewPrivate::dragEnterEvent(QDragEnterEvent* dragEnterEvent)
{
    if (mDropper->handleDragEnterEvent(dragEnterEvent)) {
        dragEnterEvent->accept();
    } else {
        dragEnterEvent->ignore();
    }
}

void AbstractByteArrayViewPrivate::dragMoveEvent(QDragMoveEvent* dragMoveEvent)
{
    if (mDropper->handleDragMoveEvent(dragMoveEvent)) {
        dragMoveEvent->accept();
    } else {
        dragMoveEvent->ignore();
    }
}

void AbstractByteArrayViewPrivate::dragLeaveEvent(QDragLeaveEvent* dragLeaveEvent)
{
    if (mDropper->handleDragLeaveEvent(dragLeaveEvent)) {
        dragLeaveEvent->accept();
    } else {
        dragLeaveEvent->ignore();
    }
}

void AbstractByteArrayViewPrivate::dropEvent(QDropEvent* dropEvent)
{
    if (mDropper->handleDropEvent(dropEvent)) {
        dropEvent->accept();
    } else {
        dropEvent->ignore();
    }
}

void AbstractByteArrayViewPrivate::contextMenuEvent(QContextMenuEvent* contextMenuEvent)
{
    auto menu = createStandardContextMenu(contextMenuEvent->pos());
    menu->setAttribute(Qt::WA_DeleteOnClose);

    menu->popup(contextMenuEvent->globalPos());
}

void AbstractByteArrayViewPrivate::onBookmarksChange(const QVector<Bookmark>& bookmarks)
{
    for (const Bookmark& bookmark : bookmarks) {
        const Address position = bookmark.offset();
        mTableRanges->addChangedRange(position, position);
    }

    unpauseCursor();
    updateChanged();
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
        emit q->readOnlyChanged(isByteArrayReadOnly);
    }
}

void AbstractByteArrayViewPrivate::onContentsChanged(const ArrayChangeMetricsList& changeList)
{
    Q_Q(AbstractByteArrayView);

    pauseCursor();

    const bool atEnd = mTableCursor->atEnd();
    const Size oldLength = mTableLayout->length(); // TODO: hack for mDataCursor->adaptSelectionToChange
    // update lengths
    int oldNoOfLines = q->noOfLines();
    mTableLayout->setLength(mByteArrayModel->size());
    const int newNoOfLines = mTableLayout->noOfLines();
    if (oldNoOfLines != newNoOfLines) {
        q->setNoOfLines(newNoOfLines);
        const LineRange changedLines = (oldNoOfLines < newNoOfLines) ?
                                       LineRange(oldNoOfLines, newNoOfLines - 1) :
                                       LineRange(newNoOfLines, oldNoOfLines - 1);
        mTableRanges->addChangedOffsetLines(changedLines);
    }

    // adapt cursor(s)
    if (atEnd) {
        mTableCursor->gotoEnd();
    } else {
        mTableCursor->adaptToChanges(changeList, oldLength);
    }

    mTableRanges->adaptToChanges(changeList, oldLength);
    // qCDebug(LOG_OKTETA_GUI) << "Cursor:"<<mDataCursor->index()<<", selection:"<<mTableRanges->selectionStart()<<"-"<<mTableRanges->selectionEnd()
    //          <<", BytesPerLine: "<<mTableLayout->noOfBytesPerLine()<<endl;

    ensureCursorVisible();
    updateChanged();
    unpauseCursor();

    emit q->cursorPositionChanged(cursorPosition());
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
