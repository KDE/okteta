/*
    SPDX-FileCopyrightText: 2008-2012 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "bytearrayjanusview.hpp"

// Okteta gui
#include <Okteta/ByteArrayColumnView>
#include <Okteta/ByteArrayRowView>
// Qt
#include <QMimeData>
#include <QScrollBar>

namespace Okteta {

ByteArrayJanusView::ByteArrayJanusView(QWidget* parent)
    : Kasten::UserMessagesOverlayedWidget(parent)
{
    setViewModus(ColumnViewId);
}

ByteArrayJanusView::~ByteArrayJanusView() = default;

void ByteArrayJanusView::setByteArrayModel(AbstractByteArrayModel* byteArrayModel)
{
    mView->setByteArrayModel(byteArrayModel);
}

void ByteArrayJanusView::setViewModus(int viewModus)
{
    if (viewModus == mViewModus) {
        return;
    }

    AbstractByteArrayView* newView = (viewModus == ColumnViewId) ?
                                     (AbstractByteArrayView*)new ByteArrayColumnView(this) :
                                     (AbstractByteArrayView*)new ByteArrayRowView(this);

    const bool hasFocus = mView ? mView->hasFocus() : false;
    if (mView) {
        // start by preparing content constraints, for view settings which depend on those
        // avoids potential scrollbar flickering on resze after show
        newView->resize(mView->size());
        newView->setFont(mView->font());

        newView->setByteArrayModel(mView->byteArrayModel());
        newView->setReadOnly(mView->isReadOnly());
        newView->setOverwriteMode(mView->isOverwriteMode());
        newView->setZoomScale(mView->zoomScale());
        newView->setShowsNonprinting(mView->showsNonprinting());
        newView->setValueCoding(mView->valueCoding());
        newView->setCharCoding(mView->charCodingName());
        newView->setVisibleCodings(mView->visibleCodings());
        newView->setActiveCoding(mView->activeCoding());
        newView->toggleOffsetColumn(mView->offsetColumnVisible());
        newView->setOffsetCoding(mView->offsetCoding());
        newView->setStartOffset(mView->startOffset());
        newView->setFirstLineOffset(mView->firstLineOffset());
        newView->setNoOfBytesPerLine(mView->noOfBytesPerLine());
        newView->setNoOfGroupedBytes(mView->noOfGroupedBytes());
        newView->setLayoutStyle(mView->layoutStyle());
        newView->setSubstituteChar(mView->substituteChar());
        newView->setUndefinedChar(mView->undefinedChar());
        newView->setCursorPosition(mView->cursorPosition());
        newView->setSelection(mView->selection());
        newView->setMarking(mView->marking());

        delete mView;
    }

    mView = newView;

    setMainWidget(mView);

    setFocusProxy(mView);
    if (hasFocus) {
        mView->setFocus();
    }
    mViewModus = viewModus;

    mView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(mView, &QWidget::customContextMenuRequested, this, &ByteArrayJanusView::viewContextMenuRequested);

    connect(mView, &AbstractByteArrayView::hasSelectedBytesChanged, this, &ByteArrayJanusView::hasSelectedBytesChanged);
    connect(mView, &AbstractByteArrayView::selectionChanged, this, &ByteArrayJanusView::selectionChanged);
    connect(mView, &AbstractByteArrayView::readOnlyChanged, this, &ByteArrayJanusView::readOnlyChanged);
    connect(mView, &AbstractByteArrayView::overwriteModeChanged, this, &ByteArrayJanusView::overwriteModeChanged);
    connect(mView, &AbstractByteArrayView::cursorPositionChanged, this, &ByteArrayJanusView::cursorPositionChanged);
    connect(mView, &AbstractByteArrayView::valueCodingChanged, this, &ByteArrayJanusView::valueCodingChanged);
    connect(mView, &AbstractByteArrayView::charCodecChanged, this, &ByteArrayJanusView::charCodecChanged);
    connect(mView, &AbstractByteArrayView::focusChanged, this, &ByteArrayJanusView::focusChanged);

    connect(mView, &AbstractByteArrayView::offsetColumnVisibleChanged, this, &ByteArrayJanusView::offsetColumnVisibleChanged);
    connect(mView, &AbstractByteArrayView::offsetCodingChanged, this, &ByteArrayJanusView::offsetCodingChanged);
    connect(mView, &AbstractByteArrayView::visibleByteArrayCodingsChanged, this, &ByteArrayJanusView::visibleByteArrayCodingsChanged);
    connect(mView, &AbstractByteArrayView::layoutStyleChanged, this, &ByteArrayJanusView::layoutStyleChanged);
    connect(mView, &AbstractByteArrayView::noOfBytesPerLineChanged, this, &ByteArrayJanusView::noOfBytesPerLineChanged);
    connect(mView, &AbstractByteArrayView::showsNonprintingChanged, this, &ByteArrayJanusView::showsNonprintingChanged);
    connect(mView, &AbstractByteArrayView::substituteCharChanged, this, &ByteArrayJanusView::substituteCharChanged);
    connect(mView, &AbstractByteArrayView::undefinedCharChanged, this, &ByteArrayJanusView::undefinedCharChanged);
    connect(mView, &AbstractByteArrayView::noOfGroupedBytesChanged, this, &ByteArrayJanusView::noOfGroupedBytesChanged);
    connect(mView, &AbstractByteArrayView::zoomScaleChanged, this, &ByteArrayJanusView::zoomScaleChanged);
    connect(mView, &AbstractByteArrayView::zoomLevelsChanged, this, &ByteArrayJanusView::zoomLevelsChanged);

    Q_EMIT viewModusChanged(mViewModus);
}

bool ByteArrayJanusView::isReadOnly()             const { return mView->isReadOnly(); }
void ByteArrayJanusView::setReadOnly(bool isReadOnly) { mView->setReadOnly(isReadOnly); }

void ByteArrayJanusView::setZoomScale(double zoomScale)
{
    mView->setZoomScale(zoomScale);
}

double ByteArrayJanusView::zoomScale() const
{
    return mView->zoomScale();
}

int ByteArrayJanusView::zoomInLevelsSize() const
{
    return mView->zoomInLevelsSize();
}

int ByteArrayJanusView::zoomOutLevelsSize() const
{
    return mView->zoomOutLevelsSize();
}

double ByteArrayJanusView::zoomScaleForLevel(int zoomLevel) const
{
    return mView->zoomScaleForLevel(zoomLevel);
}

int ByteArrayJanusView::zoomLevelForScale(double zoomScale) const
{
    return mView->zoomLevelForScale(zoomScale);
}

void ByteArrayJanusView::selectAll(bool selectAll)
{
    mView->selectAll(selectAll);
}

bool ByteArrayJanusView::hasSelectedBytes() const
{
    return mView->hasSelectedBytes();
}

std::unique_ptr<QMimeData> ByteArrayJanusView::selectedBytesAsMimeData() const
{
    return mView->selectedBytesAsMimeData();
}

void ByteArrayJanusView::insertBytesFromMimeData(const QMimeData* data)
{
    mView->insertBytesFromMimeData(data);
}

void ByteArrayJanusView::removeSelectedBytes()
{
    mView->removeSelectedBytes();
}

bool ByteArrayJanusView::canInsertBytesFromMimeData(const QMimeData* data) const
{
    return mView->canInsertBytesFromMimeData(data);
}

void ByteArrayJanusView::setCursorPosition(Address cursorPosition)
{
    mView->setCursorPosition(cursorPosition);
}
void ByteArrayJanusView::setSelectionCursorPosition(Address index)
{
    mView->setSelectionCursorPosition(index);
}
Address ByteArrayJanusView::cursorPosition() const
{
    return mView->cursorPosition();
}
QRect ByteArrayJanusView::cursorRect() const
{
    // Okteta Gui uses viewport coordinates like QTextEdit,
    // but here view coordinates are used, so map the rect as needed
    QRect cursorRect = mView->cursorRect();
    const QPoint viewTopLeft = mView->viewport()->mapToParent(cursorRect.topLeft());
    cursorRect.moveTopLeft(viewTopLeft);
    return cursorRect;
}

void ByteArrayJanusView::setStartOffset(Address startOffset)
{
    mView->setStartOffset(startOffset);
}
void ByteArrayJanusView::setFirstLineOffset(Address firstLineOffset)
{
    mView->setFirstLineOffset(firstLineOffset);
}
void ByteArrayJanusView::setNoOfBytesPerLine(int noOfBytesPerLine)
{
    mView->setNoOfBytesPerLine(noOfBytesPerLine);
}
Address ByteArrayJanusView::startOffset() const
{
    return mView->startOffset();
}
Address ByteArrayJanusView::firstLineOffset() const
{
    return mView->firstLineOffset();
}
int ByteArrayJanusView::noOfBytesPerLine() const
{
    return mView->noOfBytesPerLine();
}

int ByteArrayJanusView::valueCoding() const
{
    return mView->valueCoding();
}

QString ByteArrayJanusView::charCodingName() const
{
    return mView->charCodingName();
}

void ByteArrayJanusView::setValueCoding(int valueCoding)
{
    mView->setValueCoding((AbstractByteArrayView::ValueCoding)valueCoding);
}

void ByteArrayJanusView::setCharCoding(const QString& charCodingName)
{
    mView->setCharCoding(charCodingName);
}

AddressRange ByteArrayJanusView::selection() const
{
    return mView->selection();
}

void ByteArrayJanusView::setSelection(Address start, Address end)
{
    mView->setSelection(start, end);
}

void ByteArrayJanusView::setMarking(const AddressRange& marking)
{
    mView->setMarking(marking);
}

void ByteArrayJanusView::ensureVisible(const AddressRange& range)
{
    mView->ensureVisible(range);
}

void ByteArrayJanusView::insertBytes(const QByteArray& byteArray)
{
    mView->insertBytes(byteArray);
}

bool ByteArrayJanusView::showsNonprinting() const
{
    return false; // TODOSHOWNONPRINTING pin to false for now return mView->showsNonprinting();
}

bool ByteArrayJanusView::offsetColumnVisible() const
{
    return mView->offsetColumnVisible();
}

int ByteArrayJanusView::offsetCoding() const
{
    return mView->offsetCoding();
}

int ByteArrayJanusView::layoutStyle() const
{
    return (int)mView->layoutStyle();
}

int ByteArrayJanusView::visibleCodings() const
{
    return (int)mView->visibleCodings();
}

bool ByteArrayJanusView::isOverwriteMode() const
{
    return mView->isOverwriteMode();
}

void ByteArrayJanusView::setShowsNonprinting(bool showsNonprinting)
{
    Q_UNUSED(showsNonprinting)
    // TODOSHOWNONPRINTING mView->setShowsNonprinting(on);
}

void ByteArrayJanusView::setNoOfGroupedBytes(int noOfGroupedBytes)
{
    mView->setNoOfGroupedBytes(noOfGroupedBytes);
}

void ByteArrayJanusView::setSubstituteChar(QChar substituteChar)
{
    mView->setSubstituteChar(substituteChar);
}

void ByteArrayJanusView::setUndefinedChar(QChar undefinedChar)
{
    mView->setUndefinedChar(undefinedChar);
}

void ByteArrayJanusView::toggleOffsetColumn(bool offsetColumnVisible)
{
    mView->toggleOffsetColumn(offsetColumnVisible);
}

void ByteArrayJanusView::setOffsetCoding(int offsetCoding)
{
    mView->setOffsetCoding((AbstractByteArrayView::OffsetCoding)offsetCoding);
}

void ByteArrayJanusView::setLayoutStyle(int layoutStyle)
{
    mView->setLayoutStyle((AbstractByteArrayView::LayoutStyle)layoutStyle);
}

void ByteArrayJanusView::setVisibleCodings(int visibleColumns)
{
    mView->setVisibleCodings(visibleColumns);
}

QChar ByteArrayJanusView::substituteChar() const
{
    return mView->substituteChar();
}
QChar ByteArrayJanusView::undefinedChar() const
{
    return mView->undefinedChar();
}

int ByteArrayJanusView::byteSpacingWidth() const
{
    return mView->byteSpacingWidth();
}
int ByteArrayJanusView::noOfGroupedBytes() const
{
    return mView->noOfGroupedBytes();
}
int ByteArrayJanusView::groupSpacingWidth() const
{
    return mView->groupSpacingWidth();
}
int ByteArrayJanusView::binaryGapWidth() const
{
    return mView->binaryGapWidth();
}

bool ByteArrayJanusView::isOverwriteOnly() const
{
    return mView->isOverwriteOnly();
}

void ByteArrayJanusView::setOverwriteMode(bool overwriteMode)
{
    mView->setOverwriteMode(overwriteMode);
}

void ByteArrayJanusView::setViewPos(QPoint pos)
{
    mView->horizontalScrollBar()->setValue(pos.x());
    mView->verticalScrollBar()->setValue(pos.y());
}
QRect ByteArrayJanusView::viewRect() const
{
    // TODO: find why mView->viewport()->rect() doesn't work but is always at pos (0.0)
    const QRect result(
        QPoint(mView->horizontalScrollBar()->value(), mView->verticalScrollBar()->value()),
        mView->viewport()->size());
    return result;
}

void ByteArrayJanusView::propagateFont(const QFont& font)
{
    // TODO: nothing currently queries the font, so okay to not set it on the wrapper
    // Though message widgets use the parent widget, i.e. this, to take the font.
    // Needs some way to specify the font used in the message displays.
    // setFont(font);
    mView->setFont(font);
}

}

#include "moc_bytearrayjanusview.cpp"
