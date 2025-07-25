/*
    This file is part of the Okteta Gui library, made within the KDE community.

    SPDX-FileCopyrightText: 2008-2010 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "abstractbytearrayview.hpp"
#include "abstractbytearrayview_p.hpp"

// lib
#include "bytearraytablelayout.hpp"
// Qt
#include <QMimeData>

namespace Okteta {

AbstractByteArrayView::AbstractByteArrayView(std::unique_ptr<AbstractByteArrayViewPrivate>&& dd, QWidget* parent)
    : QAbstractScrollArea(parent)
    , d_ptr(std::move(dd))
{
    Q_D(AbstractByteArrayView);
    d->init();
}

AbstractByteArrayView::~AbstractByteArrayView() = default;

Okteta::AbstractByteArrayModel* AbstractByteArrayView::byteArrayModel() const
{
    Q_D(const AbstractByteArrayView);
    return d->byteArrayModel();
}
bool AbstractByteArrayView::isOverwriteMode() const
{
    Q_D(const AbstractByteArrayView);
    return d->isOverwriteMode();
}
bool AbstractByteArrayView::isOverwriteOnly() const
{
    Q_D(const AbstractByteArrayView);
    return d->isOverwriteOnly();
}
bool AbstractByteArrayView::isReadOnly() const
{
    Q_D(const AbstractByteArrayView);
    return d->isEffectiveReadOnly();
}
bool AbstractByteArrayView::isModified() const
{
    Q_D(const AbstractByteArrayView);
    return d->isModified();
}

double AbstractByteArrayView::zoomScale() const
{
    Q_D(const AbstractByteArrayView);
    return d->zoomScale();
}

int AbstractByteArrayView::zoomInLevelsSize() const
{
    Q_D(const AbstractByteArrayView);
    return d->zoomInLevelsSize();
}
int AbstractByteArrayView::zoomOutLevelsSize() const
{
    Q_D(const AbstractByteArrayView);
    return d->zoomOutLevelsSize();
}
double AbstractByteArrayView::zoomScaleForLevel(int zoomLevel) const
{
    Q_D(const AbstractByteArrayView);
    return d->zoomScaleForLevel(zoomLevel);
}
int AbstractByteArrayView::zoomLevelForScale(double zoomScale) const
{
    Q_D(const AbstractByteArrayView);
    return d->zoomLevelForScale(zoomScale);
}

AbstractByteArrayView::ValueCoding AbstractByteArrayView::valueCoding() const
{
    Q_D(const AbstractByteArrayView);
    return d->valueCoding();
}
const Okteta::ValueCodec* AbstractByteArrayView::valueCodec() const
{
    Q_D(const AbstractByteArrayView);
    return d->valueCodec();
}
QString AbstractByteArrayView::charCodingName() const
{
    Q_D(const AbstractByteArrayView);
    return d->charCodingName();
}
const Okteta::CharCodec* AbstractByteArrayView::charCodec() const
{
    Q_D(const AbstractByteArrayView);
    return d->charCodec();
}

int AbstractByteArrayView::noOfBytesPerLine() const
{
    Q_D(const AbstractByteArrayView);
    return d->noOfBytesPerLine();
}
Address AbstractByteArrayView::firstLineOffset()  const
{
    Q_D(const AbstractByteArrayView);
    return d->firstLineOffset();
}
Address AbstractByteArrayView::startOffset() const
{
    Q_D(const AbstractByteArrayView);
    return d->startOffset();
}

Address AbstractByteArrayView::cursorPosition() const
{
    Q_D(const AbstractByteArrayView);
    return d->cursorPosition();
}
bool AbstractByteArrayView::isCursorBehind() const
{
    Q_D(const AbstractByteArrayView);
    return d->isCursorBehind();
}
AbstractByteArrayView::LayoutStyle AbstractByteArrayView::layoutStyle() const
{
    Q_D(const AbstractByteArrayView);
    return d->layoutStyle();
}

bool AbstractByteArrayView::offsetColumnVisible() const
{
    Q_D(const AbstractByteArrayView);
    return d->offsetColumnVisible();
}

AbstractByteArrayView::OffsetCoding AbstractByteArrayView::offsetCoding() const
{
    Q_D(const AbstractByteArrayView);
    return d->offsetCoding();
}

AddressRange AbstractByteArrayView::selection() const
{
    Q_D(const AbstractByteArrayView);
    return d->selection();
}
bool AbstractByteArrayView::hasSelectedBytes() const
{
    Q_D(const AbstractByteArrayView);
    return d->hasSelectedBytes();
}

QByteArray AbstractByteArrayView::selectedBytes() const
{
    Q_D(const AbstractByteArrayView);
    return d->selectedBytes();
}
std::unique_ptr<QMimeData> AbstractByteArrayView::selectedBytesAsMimeData() const
{
    Q_D(const AbstractByteArrayView);
    return d->selectedBytesAsMimeData();
}

AddressRange AbstractByteArrayView::marking() const
{
    Q_D(const AbstractByteArrayView);
    return d->marking();
}

void AbstractByteArrayView::insertBytesFromMimeData(const QMimeData* data)
{
    Q_D(AbstractByteArrayView);
    d->insertBytesFromMimeData(data);
}

bool AbstractByteArrayView::canInsertBytesFromMimeData(const QMimeData* data) const
{
    Q_D(const AbstractByteArrayView);
    return d->canInsertBytesFromMimeData(data);
}

void AbstractByteArrayView::cut()
{
    Q_D(AbstractByteArrayView);
    d->cutToClipboard();
}

void AbstractByteArrayView::copy() const
{
    Q_D(const AbstractByteArrayView);
    d->copyToClipboard();
}

void AbstractByteArrayView::paste()
{
    Q_D(AbstractByteArrayView);
    d->pasteFromClipboard();
}

void AbstractByteArrayView::insertBytes(const QByteArray& bytes)
{
    Q_D(AbstractByteArrayView);
    d->insertBytes(bytes);
}

void AbstractByteArrayView::removeSelectedBytes()
{
    Q_D(AbstractByteArrayView);
    d->removeSelectedBytes();
}

void AbstractByteArrayView::setByteArrayModel(Okteta::AbstractByteArrayModel* byteArrayModel)
{
    Q_D(AbstractByteArrayView);
    d->setByteArrayModel(byteArrayModel);
}

void AbstractByteArrayView::setModified(bool modified)
{
    Q_D(AbstractByteArrayView);
    d->setModified(modified);
}

void AbstractByteArrayView::setOverwriteMode(bool overwriteMode)
{
    Q_D(AbstractByteArrayView);
    d->setOverwriteMode(overwriteMode);
}

void AbstractByteArrayView::setOverwriteOnly(bool overwriteOnly)
{
    Q_D(AbstractByteArrayView);
    d->setOverwriteOnly(overwriteOnly);
}

void AbstractByteArrayView::setLayoutStyle(LayoutStyle layoutStyle)
{
    Q_D(AbstractByteArrayView);
    d->setLayoutStyle(layoutStyle);
}

void AbstractByteArrayView::setNoOfBytesPerLine(int noOfBytesPerLine)
{
    Q_D(AbstractByteArrayView);
    d->setNoOfBytesPerLine(noOfBytesPerLine);
}

void AbstractByteArrayView::setStartOffset(Address startOffset)
{
    Q_D(AbstractByteArrayView);
    d->setStartOffset(startOffset);
}

void AbstractByteArrayView::setFirstLineOffset(Address firstLineOffset)
{
    Q_D(AbstractByteArrayView);
    d->setFirstLineOffset(firstLineOffset);
}

void AbstractByteArrayView::setReadOnly(bool readOnly)
{
    Q_D(AbstractByteArrayView);
    d->setReadOnly(readOnly);
}

AbstractByteArrayView::CodingTypes AbstractByteArrayView::visibleCodings() const
{
    Q_D(const AbstractByteArrayView);
    return d->visibleCodings();
}
bool AbstractByteArrayView::tabChangesFocus() const
{
    Q_D(const AbstractByteArrayView);
    return d->tabChangesFocus();
}
AbstractByteArrayView::CodingTypeId AbstractByteArrayView::activeCoding() const
{
    Q_D(const AbstractByteArrayView);
    return d->activeCoding();
}

void AbstractByteArrayView::setVisibleCodings(int visibleCodings)
{
    Q_D(AbstractByteArrayView);
    d->setVisibleCodings(visibleCodings);
}
void AbstractByteArrayView::setActiveCoding(CodingTypeId codingId)
{
    Q_D(AbstractByteArrayView);
    d->setActiveCoding(codingId);
}

void AbstractByteArrayView::setTabChangesFocus(bool tabChangesFocus)
{
    Q_D(AbstractByteArrayView);
    d->setTabChangesFocus(tabChangesFocus);
}

void AbstractByteArrayView::toggleOffsetColumn(bool offsetColumnVisible)
{
    Q_D(AbstractByteArrayView);
    d->toggleOffsetColumn(offsetColumnVisible);
}

void AbstractByteArrayView::setOffsetCoding(AbstractByteArrayView::OffsetCoding offsetCoding)
{
    Q_D(AbstractByteArrayView);
    d->setOffsetCoding(offsetCoding);
}

void AbstractByteArrayView::selectAll(bool select)
{
    Q_D(AbstractByteArrayView);
    d->selectAll(select);
}

bool AbstractByteArrayView::selectWord(Address index)
{
    Q_D(AbstractByteArrayView);
    return d->selectWord(index);
}

void AbstractByteArrayView::setCursorPosition(Address index, bool isBehind)
{
    Q_D(AbstractByteArrayView);
    d->setCursorPosition(index, isBehind);
}

void AbstractByteArrayView::setSelectionCursorPosition(Address index)
{
    Q_D(AbstractByteArrayView);
    d->setSelectionCursorPosition(index);
}

void AbstractByteArrayView::setSelection(Address start, Address end)
{
    Q_D(AbstractByteArrayView);
    d->setSelection(AddressRange(start, end));
}

void AbstractByteArrayView::setSelection(const AddressRange& selection)
{
    Q_D(AbstractByteArrayView);
    d->setSelection(selection);
}

void AbstractByteArrayView::setMarking(Address start, Address end)
{
    Q_D(AbstractByteArrayView);
    d->setMarking(AddressRange(start, end));
}

void AbstractByteArrayView::setMarking(const AddressRange& marking)
{
    Q_D(AbstractByteArrayView);
    d->setMarking(marking);
}

void AbstractByteArrayView::ensureVisible(const AddressRange& range, bool ensureStartVisible)
{
    Q_D(AbstractByteArrayView);
    d->ensureVisible(range, ensureStartVisible);
}

void AbstractByteArrayView::ensureCursorVisible()
{
    Q_D(AbstractByteArrayView);
    d->ensureCursorVisible();
}

void AbstractByteArrayView::placeCursor(QPoint point)
{
    Q_D(AbstractByteArrayView);
    d->placeCursor(point);
}

void AbstractByteArrayView::startCursor()
{
    Q_D(AbstractByteArrayView);
    d->startCursor();
}

void AbstractByteArrayView::unpauseCursor()
{
    Q_D(AbstractByteArrayView);
    d->unpauseCursor();
}

void AbstractByteArrayView::stopCursor()
{
    Q_D(AbstractByteArrayView);
    d->stopCursor();
}

void AbstractByteArrayView::pauseCursor()
{
    Q_D(AbstractByteArrayView);
    d->pauseCursor();
}

void AbstractByteArrayView::zoomIn(int pointInc)
{
    Q_D(AbstractByteArrayView);
    d->zoomIn(pointInc);
}
void AbstractByteArrayView::zoomIn()
{
    Q_D(AbstractByteArrayView);
    d->zoomIn();
}
void AbstractByteArrayView::zoomOut(int pointDec)
{
    Q_D(AbstractByteArrayView);
    d->zoomOut(pointDec);
}
void AbstractByteArrayView::zoomOut()
{
    Q_D(AbstractByteArrayView);
    d->zoomOut();
}
void AbstractByteArrayView::zoomTo(int pointSize)
{
    Q_D(AbstractByteArrayView);
    d->zoomTo(pointSize);
}
void AbstractByteArrayView::unZoom()
{
    Q_D(AbstractByteArrayView);
    d->unZoom();
}
void AbstractByteArrayView::setZoomScale(double zoomScale)
{
    Q_D(AbstractByteArrayView);
    d->setZoomScale(zoomScale);
}

void AbstractByteArrayView::changeEvent(QEvent* event)
{
    Q_D(AbstractByteArrayView);
    d->changeEvent(event);
}

QSize AbstractByteArrayView::sizeHint() const
{
    // TODO: calculate a sensible hint based on dynamic layout settings
    return {400, 120};  // {columnsWidth(), columnsHeight()};
}

Address AbstractByteArrayView::indexByPoint(QPoint point) const
{
    Q_D(const AbstractByteArrayView);
    return d->indexByPoint(point);
}

QRect AbstractByteArrayView::cursorRect() const
{
    Q_D(const AbstractByteArrayView);
    return d->cursorRect();
}

QMenu* AbstractByteArrayView::createStandardContextMenu(QPoint position)
{
    Q_D(AbstractByteArrayView);
    return d->createStandardContextMenu(position);
}

void AbstractByteArrayView::scrollContentsBy(int dx, int dy)
{
    Q_D(AbstractByteArrayView);

    d->scrollContentsBy(dx, dy);
}

void AbstractByteArrayView::paintEvent(QPaintEvent* paintEvent)
{
    Q_D(AbstractByteArrayView);

    QAbstractScrollArea::paintEvent(paintEvent);

    d->paintEvent(paintEvent);
}

bool AbstractByteArrayView::event(QEvent* event)
{
    Q_D(AbstractByteArrayView);
    return d->event(event);
}

void AbstractByteArrayView::showEvent(QShowEvent* showEvent)
{
    Q_D(AbstractByteArrayView);

    QAbstractScrollArea::showEvent(showEvent);
    // TODO: why is this needed?
    d->tableLayout()->setNoOfLinesPerPage(d->noOfLinesPerPage());
}

void AbstractByteArrayView::resizeEvent(QResizeEvent* resizeEvent)
{
    Q_D(AbstractByteArrayView);

    d->resizeEvent(resizeEvent);
}

void AbstractByteArrayView::focusInEvent(QFocusEvent* focusEvent)
{
    Q_D(AbstractByteArrayView);

    d->focusInEvent(focusEvent);
}

void AbstractByteArrayView::focusOutEvent(QFocusEvent* focusEvent)
{
    Q_D(AbstractByteArrayView);

    d->focusOutEvent(focusEvent);
}

void AbstractByteArrayView::keyPressEvent(QKeyEvent* keyEvent)
{
    Q_D(AbstractByteArrayView);
    d->keyPressEvent(keyEvent);
}

void AbstractByteArrayView::mousePressEvent(QMouseEvent* mousePressEvent)
{
    Q_D(AbstractByteArrayView);
    d->mousePressEvent(mousePressEvent);
}

void AbstractByteArrayView::mouseMoveEvent(QMouseEvent* mouseMoveEvent)
{
    Q_D(AbstractByteArrayView);
    d->mouseMoveEvent(mouseMoveEvent);
}

void AbstractByteArrayView::mouseReleaseEvent(QMouseEvent* mouseReleaseEvent)
{
    Q_D(AbstractByteArrayView);
    d->mouseReleaseEvent(mouseReleaseEvent);
}

// gets called after press and release instead of a plain press event (?)
void AbstractByteArrayView::mouseDoubleClickEvent(QMouseEvent* mouseDoubleClickEvent)
{
    Q_D(AbstractByteArrayView);
    d->mouseDoubleClickEvent(mouseDoubleClickEvent);
}

void AbstractByteArrayView::wheelEvent(QWheelEvent* wheelEvent)
{
    Q_D(AbstractByteArrayView);
    d->wheelEvent(wheelEvent);
}

void AbstractByteArrayView::dragEnterEvent(QDragEnterEvent* dragEnterEvent)
{
    Q_D(AbstractByteArrayView);
    d->dragEnterEvent(dragEnterEvent);
}
void AbstractByteArrayView::dragMoveEvent(QDragMoveEvent* dragMoveEvent)
{
    Q_D(AbstractByteArrayView);
    d->dragMoveEvent(dragMoveEvent);
}
void AbstractByteArrayView::dragLeaveEvent(QDragLeaveEvent* dragLeaveEvent)
{
    Q_D(AbstractByteArrayView);
    d->dragLeaveEvent(dragLeaveEvent);
}
void AbstractByteArrayView::dropEvent(QDropEvent* dropEvent)
{
    Q_D(AbstractByteArrayView);
    d->dropEvent(dropEvent);
}
void AbstractByteArrayView::contextMenuEvent(QContextMenuEvent* contextMenuEvent)
{
    Q_D(AbstractByteArrayView);
    d->contextMenuEvent(contextMenuEvent);
}

void AbstractByteArrayView::timerEvent(QTimerEvent* timerEvent)
{
    Q_D(AbstractByteArrayView);
    d->timerEvent(timerEvent);
}

bool AbstractByteArrayView::viewportEvent(QEvent* event)
{
    Q_D(AbstractByteArrayView);
    return d->viewportEvent(event);
}

}

// have to include this because of Q_PRIVATE_SLOT
#include "moc_abstractbytearrayview.cpp"
