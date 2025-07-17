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
#include "controller/tapnavigator.hpp"
#include "controller/zoomwheelcontroller.hpp"
#include "controller/zoompinchcontroller.hpp"
#include "controller/touchonlytapandholdgesture.hpp"
#include "controller/touchonlytapandholdgesturerecognizer.hpp"

#include "widgetcolumnstylist.hpp"
#include "cursor.hpp"
#include "bordercolumnrenderer.hpp"
#include "oktetagui.hpp"
// liboktetawidgets
#include <bytearraychar8stringdecoder.hpp>
#include <bytearrayvaluestringdecoder.hpp>
// Okteta core
#include <Okteta/ValueCodec>
#include <Okteta/Bookmarkable>
#include <Okteta/Versionable>
#include <Okteta/TextByteArrayAnalyzer>
#include <Okteta/Bookmark>
#include <Okteta/Character>
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
#include <QStyle>
#include <QStyleHints>
#include <QInputMethod>
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

Qt::GestureType touchOnlyTapAndHoldGestureType()
{
    static Qt::GestureType type =
        QGestureRecognizer::registerRecognizer(new TouchOnlyTapAndHoldGestureRecognizer);
    return type;
}

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
    , mCursorPaused(false)
    , mBlinkCursorVisible(false)
    , mCursorVisible(false)
    , m_wasFocussedByMouseClick(false)
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

    delete mZoomPinchController;
    delete mTapNavigator;

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

    // TODO: check if the crash noted with the constructor of FontScalingZoomState still holds
    // if not, move to constructor init
    m_fontScalingZoomState.initFont(q->font());

    // initialize layout
    mTableLayout->setLength(mByteArrayModel->size());
    mTableLayout->setNoOfLinesPerPage(q->noOfLinesPerPage());

    mStylist = new WidgetColumnStylist(q);

    mOffsetColumn =
        new OffsetColumnRenderer(mStylist, mTableLayout, OffsetFormat::Hexadecimal);
    const int headerMargin = q->style()->pixelMetric(QStyle::PM_HeaderMargin);
    mOffsetColumn->setMargin(headerMargin);
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
    mTapNavigator = new TapNavigator(q);

    mZoomWheelController = new ZoomWheelController(q, nullptr);
    mDropper = new Dropper(q);
    mZoomPinchController = new ZoomPinchController(q);

    setWheelController(mZoomWheelController);

    q->setAttribute(Qt::WA_InputMethodEnabled);
    q->setInputMethodHints(Qt::ImhNoAutoUppercase | Qt::ImhNoPredictiveText);

    QObject::connect(QGuiApplication::styleHints(), &QStyleHints::cursorFlashTimeChanged,
                     q, [this](int flashTime) { onCursorFlashTimeChanged(flashTime); });

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
    mTableLayout->setLength(mByteArrayModel->size());
    adjustLayoutToSize();

    // if the model is readonly make the view too, per default
    if (mByteArrayModel->isReadOnly()) {
        setReadOnly(true);
    }

    QObject::connect(mByteArrayModel, &AbstractByteArrayModel::readOnlyChanged,
                     q, [this](bool isReadOnly) { onByteArrayReadOnlyChange(isReadOnly); });
    QObject::connect(mByteArrayModel, &AbstractByteArrayModel::contentsChanged,
                     q, [this](const Okteta::ArrayChangeMetricsList& changeList) { onContentsChanged(changeList); });

    auto* bookmarks = qobject_cast<Bookmarkable*>(mByteArrayModel);
    if (bookmarks) {
        QObject::connect(mByteArrayModel, SIGNAL(bookmarksAdded(QVector<Okteta::Bookmark>)),
                         q, SLOT(onBookmarksChange(QVector<Okteta::Bookmark>)));
        QObject::connect(mByteArrayModel, SIGNAL(bookmarksRemoved(QVector<Okteta::Bookmark>)),
                         q, SLOT(onBookmarksChange(QVector<Okteta::Bookmark>)));
    }
    auto* versionControl = qobject_cast<Versionable*>(mByteArrayModel);
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

    switch (event->type()) {
    case QEvent::FontChange: {
        // update rendering font to new font, using current scaling
        m_fontScalingZoomState.setFont(q->font());

        adjustTToScaledFont();

        // might this need atomic signalling rather?
        emit q->zoomLevelChanged(m_fontScalingZoomState.scale());
        emit q->zoomLevelsChanged();
        break;
    }
    case QEvent::StyleChange: {
        // get new values
        const int headerMargin = q->style()->pixelMetric(QStyle::PM_HeaderMargin);
        mOffsetColumn->setMargin(headerMargin);

        // update all dependent structures
        mTableLayout->setNoOfLinesPerPage(q->noOfLinesPerPage());

        updateViewByWidth();

        if (mCursorVisible) {
            updateCursors();
        }
        break;
    }
    default:
        break;
    }
}

double AbstractByteArrayViewPrivate::zoomScaleForLevel(int level)   const
{
    return m_fontScalingZoomState.scaleForLevel(level);
}

int AbstractByteArrayViewPrivate::zoomLevelForScale(double zoomScale) const
{
    return m_fontScalingZoomState.levelForScale(zoomScale);
}

void AbstractByteArrayViewPrivate::zoomIn()  { zoomIn(DefaultZoomStep); }
void AbstractByteArrayViewPrivate::zoomOut() { zoomOut(DefaultZoomStep); }

void AbstractByteArrayViewPrivate::zoomIn(int pointIncrement)
{
    Q_Q(AbstractByteArrayView);

    m_fontScalingZoomState.zoomIn(pointIncrement);

    adjustTToScaledFont();

    emit q->zoomLevelChanged(m_fontScalingZoomState.scale());
}

void AbstractByteArrayViewPrivate::zoomOut(int pointDecrement)
{
    Q_Q(AbstractByteArrayView);

    m_fontScalingZoomState.zoomOut(pointDecrement);

    adjustTToScaledFont();

    emit q->zoomLevelChanged(m_fontScalingZoomState.scale());
}

void AbstractByteArrayViewPrivate::zoomTo(int newPointSize)
{
    Q_Q(AbstractByteArrayView);

    if (!m_fontScalingZoomState.zoomTo(newPointSize)) {
        return;
    }

    adjustTToScaledFont();

    emit q->zoomLevelChanged(m_fontScalingZoomState.scale());
}

void AbstractByteArrayViewPrivate::unZoom()
{
    zoomTo(m_fontScalingZoomState.defaultFontSize());
}

void AbstractByteArrayViewPrivate::setZoomScale(double zoomScale)
{
    Q_Q(AbstractByteArrayView);

    if (!m_fontScalingZoomState.setScale(zoomScale)) {
        return;
    }

    adjustTToScaledFont();

    emit q->zoomLevelChanged(m_fontScalingZoomState.scale());
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
    bool result = false;

    if (0 <= index && index < mTableLayout->length()) {
        const TextByteArrayAnalyzer textAnalyzer(mByteArrayModel, mCharCodec);
        const AddressRange wordSection = textAnalyzer.wordSection(index);
        if (wordSection.isValid()) {
            pauseCursor();
            finishByteEditor();

            mTableRanges->setFirstWordSelection(wordSection);
            mTableCursor->gotoIndex(wordSection.nextBehindEnd());

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
        mTableRanges->setSelection(AddressRange(0, mTableLayout->length() - 1));
        mTableCursor->gotoEnd();
    } else {
        mTableRanges->removeSelection();
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
    mTableCursor->gotoCIndex(index);
    if (behind) {
        mTableCursor->stepBehind();
    }

    mTableRanges->removeSelection();
    ensureCursorVisible();

    endViewUpdate();
}

void AbstractByteArrayViewPrivate::setSelectionCursorPosition(Address index)
{
    pauseCursor();
    finishByteEditor();

    if (!mTableRanges->selectionStarted()) {
        mTableRanges->setSelectionStart(mTableCursor->realIndex());
    }

    mTableCursor->gotoCIndex(index);

    mTableRanges->setSelectionEnd(mTableCursor->realIndex());
    ensureCursorVisible();

    endViewUpdate();
}

void AbstractByteArrayViewPrivate::setSelection(const AddressRange& _selection)
{
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

    endViewUpdate();
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
    Q_Q(const AbstractByteArrayView);

    if (!mTableRanges->hasSelection()) {
        return nullptr;
    }

    const QByteArray bytes = selectedData();

    auto* mimeData = new QMimeData;
    mimeData->setData(octetStreamFormatName(), bytes);

    QString displayedText;
    if (activeCoding() == AbstractByteArrayView::CharCodingId) {
        const QChar undefinedChar = q->undefinedChar();
        const QChar substituteChar = q->substituteChar();

        const Okteta::CharCodec* const charCodec = this->charCodec();
        const QChar tabChar = QLatin1Char('\t');
        const QChar returnChar = QLatin1Char('\n');

        displayedText.reserve(bytes.size());

        for (const auto byte : bytes) {
            const Okteta::Character byteChar = charCodec->decode(byte);

            const QChar streamChar = byteChar.isUndefined() ?      undefinedChar :
                                     (!byteChar.isPrint()
                                      || byteChar == tabChar
                                      || byteChar == returnChar) ? substituteChar :
                                                                   static_cast<QChar>(byteChar);
            displayedText.append(streamChar);
        }
    } else {
        // prepare
        QString valueString;
        valueString.resize(mValueCodec->encodingWidth());

        const QChar separator = QLatin1Char(' ');
        const int separatorSize = 1;
        displayedText.reserve((valueString.size() + separatorSize) * bytes.size() - separatorSize);

        bool isFirst = true;
        for (const auto byte : bytes) {
            if (Q_LIKELY(!isFirst)) {
                displayedText.append(separator);
            } else {
                isFirst = false;
            }

            mValueCodec->encode(&valueString, 0, byte);

            displayedText.append(valueString);
        }
    }
    mimeData->setText(displayedText);

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
    // If there is an octet stream, use it, otherwise  try to decode values or chars,
    // assuming byte array descriptions by text.
    // If that fails, take the dump of the format with the highest priority.
    // TODO: instead of guessing, ask user for the decoding to try.
    // would need the movement of the encoders into the core library
    QByteArray byteArray;
    const QString dataFormatName = octetStreamFormatName();
    if (data->hasFormat(dataFormatName)) {
        byteArray = data->data(dataFormatName);
    } else {
        // try to parse any encoding in text
        if (data->hasText()) {
            const QString text = data->text();
            if (activeCoding() == AbstractByteArrayView::CharCodingId) {
                ByteArrayChar8StringDecoder char8StringDecoder;
                char8StringDecoder.setCharCodec(charCodingName());
                int usedTextSize = -1;
                const ByteArrayChar8StringDecoder::CodeState evalResult = char8StringDecoder.decode(&byteArray, text, 0, -1, &usedTextSize);
                if ((evalResult != ByteArrayChar8StringDecoder::CodeAcceptable) ||
                    (usedTextSize != text.size())) {
                    byteArray.clear();
                }
            } else {
                ByteArrayValueStringDecoder valueStringDecoder(mValueCodec);
                const ByteArrayValueStringDecoder::CodeState evalResult = valueStringDecoder.decode(&byteArray, text);
                if (evalResult != ByteArrayValueStringDecoder::CodeAcceptable) {
                    byteArray.clear();
                }
            }
        }
        // nothing decoded (for now empty -> failed)? fall back to raw data of first format
        if (byteArray.isEmpty()) {
            const QString firstDataFormatName = data->formats()[0];
            byteArray = data->data(firstDataFormatName);
        }
    }

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
        if (mTableRanges->hasSelection()) {
            // replacing the selection:
            // we restrict the replacement to the minimum length of selection and input
            AddressRange selection = mTableRanges->removeSelection();
            selection.restrictEndByWidth(data.size());
            insertionOffset = selection.start();
            lengthOfInserted = mByteArrayModel->replace(selection, reinterpret_cast<const Byte*>(data.constData()), selection.width());
        } else {
            const Size length = mTableLayout->length();
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
        if (mTableRanges->hasSelection()) {
            // replacing the selection
            const AddressRange selection = mTableRanges->removeSelection();
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
            mTableCursor->gotoCIndex(postInsertionOffset);
            unpauseCursor();
            emit q->cursorPositionChanged(cursorPosition());
        }
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
    painter->setFont(renderingFont());
}

QMenu* AbstractByteArrayViewPrivate::createStandardContextMenu(QPoint position)
{
    Q_UNUSED(position)

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

void AbstractByteArrayViewPrivate::keyPressEvent(QKeyEvent* keyEvent)
{
    Q_Q(AbstractByteArrayView);

    if (!controller()->handleKeyPress(keyEvent)) {
        q->ColumnsView::keyPressEvent(keyEvent);
    }
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

bool isStyleRequestingSoftwareInputPanelOnMouseClick(QWidget* widget)
{
    const auto behavior = QStyle::RequestSoftwareInputPanel(
                    widget->style()->styleHint(QStyle::SH_RequestSoftwareInputPanel));
    return (behavior == QStyle::RSIP_OnMouseClick);
}

void AbstractByteArrayViewPrivate::mouseReleaseEvent(QMouseEvent* mouseEvent)
{
    Q_Q(AbstractByteArrayView);

    if (mMouseController->handleMouseReleaseEvent(mouseEvent)) {
        mouseEvent->accept();
    } else {
        q->ColumnsView::mouseReleaseEvent(mouseEvent);
    }

    if (!isEffectiveReadOnly() && q->rect().contains(mouseEvent->pos())) {
        if (mouseEvent->button() == Qt::LeftButton && qApp->autoSipEnabled()) {
            if (!m_wasFocussedByMouseClick || isStyleRequestingSoftwareInputPanelOnMouseClick(q)) {
                QGuiApplication::inputMethod()->show();
            }
        }
    }

    m_wasFocussedByMouseClick = false;
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

void AbstractByteArrayViewPrivate::wheelEvent(QWheelEvent* wheelEvent)
{
    Q_Q(AbstractByteArrayView);

    if (!wheelController()->handleWheelEvent(wheelEvent)) {
        q->ColumnsView::wheelEvent(wheelEvent);
    }
}

bool AbstractByteArrayViewPrivate::event(QEvent* event)
{
    Q_Q(AbstractByteArrayView);

    //
    switch (event->type()) {
    case QEvent::KeyPress: {
        auto* keyEvent = static_cast<QKeyEvent*>(event);
        if (keyEvent->key() == Qt::Key_Tab || keyEvent->key() == Qt::Key_Backtab) {
            q->keyPressEvent(keyEvent);
            if (keyEvent->isAccepted()) {
                return true;
            }
        }
        break;
    }
    case QEvent::ShortcutOverride: {
        auto* const keyEvent = static_cast<QKeyEvent*>(event);
        controller()->handleShortcutOverrideEvent(keyEvent);
        break;
    }
    case QEvent::MouseMove:
    case QEvent::MouseButtonPress:
    case QEvent::MouseButtonRelease: {
        // discard any events synthesized from touch input
        auto* mouseEvent = static_cast<QMouseEvent*>(event);
        if (mouseEvent->source() == Qt::MouseEventSynthesizedByQt) {
            event->accept();
            return true;
        }
        break;
    }
    case QEvent::PaletteChange: {
        if (mCursorVisible) {
            updateCursors();
        }
        break;
    }
    case QEvent::ContextMenu: {
        auto* const contextMenuEvent = static_cast<QContextMenuEvent*>(event);
        if (contextMenuEvent->reason() == QContextMenuEvent::Keyboard) {
            ensureCursorVisible();

            const QPoint cursorPos = cursorRect().center();
            QContextMenuEvent adaptedContextMenuEvent(QContextMenuEvent::Keyboard, cursorPos,
                                                    q->viewport()->mapToGlobal(cursorPos));
            adaptedContextMenuEvent.setAccepted(event->isAccepted());

            const bool result = q->ColumnsView::event(&adaptedContextMenuEvent);
            event->setAccepted(adaptedContextMenuEvent.isAccepted());

            return result;
        }
        break;
    }
    case QEvent::Gesture: {
        auto* gestureEvent = static_cast<QGestureEvent*>(event);
        if (auto* tapGesture = static_cast<QTapGesture*>(gestureEvent->gesture(Qt::TapGesture))) {
            return mTapNavigator->handleTapGesture(tapGesture);
        }
        if (auto* tapAndHoldGesture = static_cast<TouchOnlyTapAndHoldGesture*>(gestureEvent->gesture(touchOnlyTapAndHoldGestureType()))) {
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

                const bool result = q->ColumnsView::event(&simulatedContextMenuEvent);
                event->setAccepted(simulatedContextMenuEvent.isAccepted());

                return result;
            }
        } else if (auto* pinchGesture = static_cast<QPinchGesture*>(gestureEvent->gesture(Qt::PinchGesture))) {
            return mZoomPinchController->handlePinchGesture(pinchGesture);
        };
        break;
    }
    default:
        break;
    }

    return q->ColumnsView::event(event);
}

bool AbstractByteArrayViewPrivate::viewportEvent(QEvent* event)
{
    Q_Q(AbstractByteArrayView);

    switch (event->type()) {
    case QEvent::ToolTip: {
        auto* helpEvent = static_cast<QHelpEvent*>(event);

        QString toolTip;

        auto* bookmarks = qobject_cast<Bookmarkable*>(mByteArrayModel);
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
    case QEvent::MouseMove:
    case QEvent::MouseButtonPress:
    case QEvent::MouseButtonRelease: {
        // discard any events synthesized from touch input
        auto* mouseEvent = static_cast<QMouseEvent*>(event);
        if (mouseEvent->source() == Qt::MouseEventSynthesizedByQt) {
            event->accept();
            return true;
        }
    }
    default:
        break;
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

    const Qt::FocusReason focusReason = focusEvent->reason();
    if (focusReason == Qt::MouseFocusReason) {
        m_wasFocussedByMouseClick = true;
    }

    q->ColumnsView::focusInEvent(focusEvent);
    startCursor();

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

void AbstractByteArrayViewPrivate::timerEvent(QTimerEvent* timerEvent)
{
    Q_Q(AbstractByteArrayView);

    if (timerEvent->timerId() == mCursorBlinkTimerId) {
        blinkCursor();
    }

    q->ColumnsView::timerEvent(timerEvent);
}

void AbstractByteArrayViewPrivate::onBookmarksChange(const QVector<Bookmark>& bookmarks)
{
    for (const Bookmark& bookmark : bookmarks) {
        const Address position = bookmark.offset();
        mTableRanges->addChangedRange(position, position);
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
    mTableRanges->takeHasSelectionChanged(&hasSelectionChanged, &selectionChanged);

    if (selectionChanged) {
        emit q->selectionChanged(mTableRanges->selection());
    }
    if (hasSelectionChanged) {
        const bool hasSelection = mTableRanges->hasSelection();
        if (!mOverWrite) {
            emit q->cutAvailable(hasSelection);
        }
        emit q->copyAvailable(hasSelection);
        emit q->hasSelectedDataChanged(hasSelection);
    }
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
