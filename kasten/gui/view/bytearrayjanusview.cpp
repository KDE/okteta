/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2008-2012 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "bytearrayjanusview.hpp"

// Okteta gui
#include <Okteta/ByteArrayColumnView>
#include <Okteta/ByteArrayRowView>
// Qt
#include <QLayout>
#include <QScrollBar>

namespace Okteta {

ByteArrayJanusView::ByteArrayJanusView(QWidget* parent)
    : QWidget(parent)
{
    mLayout = new QHBoxLayout(this);
    mLayout->setContentsMargins(0, 0, 0, 0);
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
        newView->setFont(mView->font());

        newView->setByteArrayModel(mView->byteArrayModel());
        newView->setReadOnly(mView->isReadOnly());
        newView->setOverwriteMode(mView->isOverwriteMode());
        newView->setZoomLevel(mView->zoomLevel());
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

        mLayout->removeWidget(mView);
        delete mView;
    }

    mView = newView;

    mLayout->addWidget(mView);
    setFocusProxy(mView);
    if (hasFocus) {
        mView->setFocus();
    }
    mViewModus = viewModus;

    mView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(mView, &QWidget::customContextMenuRequested, this, &ByteArrayJanusView::viewContextMenuRequested);

    connect(mView, &AbstractByteArrayView::hasSelectedDataChanged, this, &ByteArrayJanusView::hasSelectedDataChanged);
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
    connect(mView, &AbstractByteArrayView::zoomLevelChanged, this, &ByteArrayJanusView::zoomLevelChanged);

    emit viewModusChanged(mViewModus);
}

bool ByteArrayJanusView::isReadOnly()             const { return mView->isReadOnly(); }
void ByteArrayJanusView::setReadOnly(bool isReadOnly) { mView->setReadOnly(isReadOnly); }

void ByteArrayJanusView::setZoomLevel(double Level)
{
    mView->setZoomLevel(Level);
}

double ByteArrayJanusView::zoomLevel() const
{
    return mView->zoomLevel();
}

void ByteArrayJanusView::selectAll(bool selectAll)
{
    mView->selectAll(selectAll);
}

bool ByteArrayJanusView::hasSelectedData() const
{
    return mView->hasSelectedData();
}

QMimeData* ByteArrayJanusView::selectionAsMimeData() const
{
    return mView->selectionAsMimeData();
}

void ByteArrayJanusView::pasteData(const QMimeData* data)
{
    mView->pasteData(data);
}

void ByteArrayJanusView::removeSelectedData()
{
    mView->removeSelectedData();
}

bool ByteArrayJanusView::canReadData(const QMimeData* data) const
{
    return mView->canReadData(data);
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

void ByteArrayJanusView::insert(const QByteArray& byteArray)
{
    mView->insert(byteArray);
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

void ByteArrayJanusView::setShowsNonprinting(bool on)
{
    Q_UNUSED(on);
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

void ByteArrayJanusView::toggleOffsetColumn(bool on)
{
    mView->toggleOffsetColumn(on);
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
    setFont(font);
    mView->setFont(font);
}

}
