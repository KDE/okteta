/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2006-2012 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "bytearrayview.hpp"

// lib
#include "bytearrayjanusview.hpp"
#include <bytearrayviewprofilesynchronizer.hpp>
// Okteta Kasten core
#include <Kasten/Okteta/ByteArrayDocument>
// Okteta gui
#include <Okteta/AbstractByteArrayView>
// Okteta core
#include <Okteta/AbstractByteArrayModel>
// Qt
#include <QFontDatabase>

namespace Kasten {

// TODO: merge into ByteArrayView on next ABI break
class SelectedDataCutExtension : public QObject, public If::SelectedDataCutable
{
    Q_OBJECT
    Q_INTERFACES(
        Kasten::If::SelectedDataCutable
    )

public:
    explicit SelectedDataCutExtension(ByteArrayView* view);

public: // If::SelectedDataCutable API
    bool canCutSelectedData() const override;
Q_SIGNALS:
    void canCutSelectedDataChanged(bool canCutSelectedData) override;

private Q_SLOTS:
    void onOverwriteModeChanged(bool overwriteMode);
};

SelectedDataCutExtension::SelectedDataCutExtension(ByteArrayView* view)
    : QObject(view)
{
    connect(view, &ByteArrayView::overwriteModeChanged, this, &SelectedDataCutExtension::onOverwriteModeChanged);
}

bool SelectedDataCutExtension::canCutSelectedData() const
{
    return !static_cast<ByteArrayView*>(parent())->isOverwriteMode();
}

void SelectedDataCutExtension::onOverwriteModeChanged(bool overwriteMode)
{
    emit canCutSelectedDataChanged(!overwriteMode);
}


ByteArrayView::ByteArrayView(ByteArrayDocument* document, ByteArrayViewProfileSynchronizer* synchronizer)
    : AbstractView(document)
    , mDocument(document)
    , mByteArrayViewProfileSynchronizer(synchronizer)
{
    init();
    if (synchronizer) {
        synchronizer->setView(this);
    }
}

ByteArrayView::ByteArrayView(ByteArrayView* other, ByteArrayViewProfileSynchronizer* synchronizer,
                             Qt::Alignment alignment)
    : AbstractView(static_cast<ByteArrayDocument*>(other->baseModel()))
    , mDocument(static_cast<ByteArrayDocument*>(other->baseModel()))
    , mByteArrayViewProfileSynchronizer(synchronizer)
{
    init();

    mWidget->setStartOffset(other->startOffset());
    mWidget->setFirstLineOffset(other->firstLineOffset());

    setViewModus(other->viewModus());
    setVisibleByteArrayCodings(other->visibleByteArrayCodings());
    toggleOffsetColumn(other->offsetColumnVisible());
    setOffsetCoding(other->offsetCoding());

    setCharCoding(other->charCodingName());
    setShowsNonprinting(other->showsNonprinting());
    setSubstituteChar(other->substituteChar());
    setUndefinedChar(other->undefinedChar());

    setValueCoding(other->valueCoding());

    setNoOfGroupedBytes(other->noOfGroupedBytes());
    setNoOfBytesPerLine(other->noOfBytesPerLine());
    // TODO: this can lead to different layouts due to possible one-pixel difference in width!
    setLayoutStyle(other->layoutStyle());

    const Okteta::AddressRange selection = other->selection();
    setSelection(selection.start(), selection.end());
    setZoomLevel(other->zoomLevel());
    setCursorPosition(other->cursorPosition());

    setOverwriteMode(other->isOverwriteMode());
    setReadOnly(other->isReadOnly());
    // TODO: all width

    const QRect otherViewRect = other->mWidget->viewRect();

    QPoint viewPos = otherViewRect.topLeft();
    if (alignment == Qt::AlignBottom) {
        viewPos.setY(otherViewRect.bottom() + 1);
    }
    // TODO: care for resize style
    else if (alignment == Qt::AlignRight) {
        viewPos.setX(otherViewRect.right() + 1);
    }
    // TODO: doesn't really work at this stage, because the widget will get resized when inserted
    // and then ensureCursorVisible destroys the fun
    mWidget->setViewPos(viewPos);

    if (synchronizer) {
        synchronizer->setView(this);
    }
}

ByteArrayView::~ByteArrayView()
{
    delete mByteArrayViewProfileSynchronizer;
    delete mWidget;
}

void ByteArrayView::init()
{
    Okteta::AbstractByteArrayModel* content = mDocument->content();
    mWidget = new Okteta::ByteArrayJanusView();
    mWidget->setByteArrayModel(content);

    // TODO: find a signal/event emitted when fixedfont changes
//     connect( KGlobalSettings::self(), &KGlobalSettings::kdisplayFontChanged,
//              this, &ByteArrayView::setFontByGlobalSettings );
    setFontByGlobalSettings();

    mWidget->setNoOfBytesPerLine(16);

    const bool useOverwriteAsDefault = (content->size() > 0);
    mWidget->setOverwriteMode(useOverwriteAsDefault);

    // propagate signals
    using Okteta::ByteArrayJanusView;
    connect(mDocument, &ByteArrayDocument::titleChanged, this, &ByteArrayView::titleChanged);
    connect(mWidget, &ByteArrayJanusView::hasSelectedDataChanged, this, &ByteArrayView::hasSelectedDataChanged);
    connect(mWidget, &ByteArrayJanusView::readOnlyChanged, this, &ByteArrayView::readOnlyChanged);
    connect(mWidget, &ByteArrayJanusView::overwriteModeChanged, this, &ByteArrayView::overwriteModeChanged);
    connect(mWidget, &ByteArrayJanusView::selectionChanged, this, &ByteArrayView::onSelectionChanged);
    connect(mWidget, &ByteArrayJanusView::cursorPositionChanged, this, &ByteArrayView::cursorPositionChanged);
    connect(mWidget, &ByteArrayJanusView::valueCodingChanged, this, &ByteArrayView::valueCodingChanged);
    connect(mWidget, &ByteArrayJanusView::charCodecChanged, this, &ByteArrayView::charCodecChanged);
    connect(mWidget, &ByteArrayJanusView::focusChanged, this, &ByteArrayView::focusChanged);

    connect(mWidget, &ByteArrayJanusView::offsetColumnVisibleChanged, this, &ByteArrayView::offsetColumnVisibleChanged);
    connect(mWidget, &ByteArrayJanusView::offsetCodingChanged, this, &ByteArrayView::offsetCodingChanged);
    connect(mWidget, &ByteArrayJanusView::visibleByteArrayCodingsChanged, this, &ByteArrayView::visibleByteArrayCodingsChanged);
    connect(mWidget, &ByteArrayJanusView::layoutStyleChanged, this, &ByteArrayView::layoutStyleChanged);
    connect(mWidget, &ByteArrayJanusView::noOfBytesPerLineChanged, this, &ByteArrayView::noOfBytesPerLineChanged);
    connect(mWidget, &ByteArrayJanusView::showsNonprintingChanged, this, &ByteArrayView::showsNonprintingChanged);
    connect(mWidget, &ByteArrayJanusView::substituteCharChanged, this, &ByteArrayView::substituteCharChanged);
    connect(mWidget, &ByteArrayJanusView::undefinedCharChanged, this, &ByteArrayView::undefinedCharChanged);
    connect(mWidget, &ByteArrayJanusView::noOfGroupedBytesChanged, this, &ByteArrayView::noOfGroupedBytesChanged);
    connect(mWidget, &ByteArrayJanusView::zoomLevelChanged, this, &ByteArrayView::zoomLevelChanged);
    connect(mWidget, &ByteArrayJanusView::viewModusChanged, this, &ByteArrayView::viewModusChanged);

    connect(mWidget, &ByteArrayJanusView::viewContextMenuRequested, this, &ByteArrayView::viewContextMenuRequested);

    new SelectedDataCutExtension(this);
}

ByteArrayViewProfileSynchronizer* ByteArrayView::synchronizer() const { return mByteArrayViewProfileSynchronizer; }

const AbstractModelSelection* ByteArrayView::modelSelection() const { return &mSelection; }

QWidget* ByteArrayView::widget()             const { return mWidget; }
bool ByteArrayView::hasFocus()               const { return mWidget->focusWidget()->hasFocus(); } // TODO: does this work?

QString ByteArrayView::title()               const { return mDocument->title(); }
bool ByteArrayView::isModifiable()           const { return true; }
bool ByteArrayView::isReadOnly()             const { return mWidget->isReadOnly(); }

void ByteArrayView::setFocus()
{
    mWidget->setFocus();
}

void ByteArrayView::setReadOnly(bool isReadOnly) { mWidget->setReadOnly(isReadOnly); }

void ByteArrayView::setZoomLevel(double Level)
{
    mWidget->setZoomLevel(Level);
}

double ByteArrayView::zoomLevel() const
{
    return mWidget->zoomLevel();
}

void ByteArrayView::selectAllData(bool selectAll)
{
    mWidget->selectAll(selectAll);
}

bool ByteArrayView::hasSelectedData() const
{
    return mWidget->hasSelectedData();
}

QMimeData* ByteArrayView::copySelectedData() const
{
    return mWidget->selectionAsMimeData();
}

void ByteArrayView::insertData(const QMimeData* data)
{
    mWidget->pasteData(data);
}

QMimeData* ByteArrayView::cutSelectedData()
{
    QMimeData* result = mWidget->selectionAsMimeData();
    mWidget->removeSelectedData();
    return result;
}

void ByteArrayView::deleteSelectedData()
{
    mWidget->removeSelectedData();
}

bool ByteArrayView::canReadData(const QMimeData* data) const
{
    return mWidget->canReadData(data);
}

void ByteArrayView::onSelectionChanged(const Okteta::AddressRange& selection)
{
    // TODO: how to make sure the signal hasSelectedDataChanged() is not emitted before?
    mSelection.setRange(selection);
    emit selectedDataChanged(&mSelection);
}

void ByteArrayView::setCursorPosition(Okteta::Address cursorPosition)
{
    mWidget->setCursorPosition(cursorPosition);
}

void ByteArrayView::setSelectionCursorPosition(Okteta::Address index)
{
    mWidget->setSelectionCursorPosition(index);
}

Okteta::Address ByteArrayView::cursorPosition() const
{
    return mWidget->cursorPosition();
}
QRect ByteArrayView::cursorRect() const
{
    return mWidget->cursorRect();
}

Okteta::Address ByteArrayView::startOffset() const
{
    return mWidget->startOffset();
}
Okteta::Address ByteArrayView::firstLineOffset() const
{
    return mWidget->firstLineOffset();
}
int ByteArrayView::noOfBytesPerLine() const
{
    return mWidget->noOfBytesPerLine();
}

int ByteArrayView::valueCoding() const
{
    return mWidget->valueCoding();
}

QString ByteArrayView::charCodingName() const
{
    return mWidget->charCodingName();
}

void ByteArrayView::setValueCoding(int valueCoding)
{
    mWidget->setValueCoding((Okteta::AbstractByteArrayView::ValueCoding)valueCoding);
}

void ByteArrayView::setCharCoding(const QString& charCodingName)
{
    mWidget->setCharCoding(charCodingName);
}

Okteta::AddressRange ByteArrayView::selection() const
{
    return mWidget->selection();
}

void ByteArrayView::setSelection(Okteta::Address start, Okteta::Address end)
{
    mWidget->setSelection(start, end);
}

void ByteArrayView::insert(const QByteArray& byteArray)
{
    mWidget->insert(byteArray);
}

bool ByteArrayView::showsNonprinting() const
{
    return mWidget->showsNonprinting();
}

bool ByteArrayView::offsetColumnVisible() const
{
    return mWidget->offsetColumnVisible();
}

int ByteArrayView::offsetCoding() const
{
    return mWidget->offsetCoding();
}

int ByteArrayView::layoutStyle() const
{
    return (int)mWidget->layoutStyle();
}

int ByteArrayView::visibleByteArrayCodings() const
{
    return (int)mWidget->visibleCodings();
}

bool ByteArrayView::isOverwriteMode() const
{
    return mWidget->isOverwriteMode();
}

void ByteArrayView::setShowsNonprinting(bool on)
{
    mWidget->setShowsNonprinting(on);
}

void ByteArrayView::setNoOfGroupedBytes(int noOfGroupedBytes)
{
    mWidget->setNoOfGroupedBytes(noOfGroupedBytes);
}

void ByteArrayView::toggleOffsetColumn(bool on)
{
    mWidget->toggleOffsetColumn(on);
}

void ByteArrayView::setOffsetCoding(int offsetCoding)
{
    mWidget->setOffsetCoding(offsetCoding);
}

void ByteArrayView::setLayoutStyle(int layoutStyle)
{
    mWidget->setLayoutStyle((Okteta::AbstractByteArrayView::LayoutStyle)layoutStyle);
}

void ByteArrayView::setNoOfBytesPerLine(int noOfBytesPerLine)
{
    mWidget->setNoOfBytesPerLine(noOfBytesPerLine);
}

void ByteArrayView::setVisibleByteArrayCodings(int visibleColumns)
{
    mWidget->setVisibleCodings(visibleColumns);
}

void ByteArrayView::setMarking(const Okteta::AddressRange& range, bool ensureVisible)
{
    mWidget->setMarking(range);
    if (ensureVisible) {
        mWidget->ensureVisible(range);
    }
}

void ByteArrayView::setSubstituteChar(QChar substituteChar)
{
    mWidget->setSubstituteChar(substituteChar);
}

void ByteArrayView::setUndefinedChar(QChar undefinedChar)
{
    mWidget->setUndefinedChar(undefinedChar);
}

QChar ByteArrayView::substituteChar() const
{
    return mWidget->substituteChar();
}
QChar ByteArrayView::undefinedChar() const
{
    return mWidget->undefinedChar();
}

int ByteArrayView::byteSpacingWidth() const
{
    return mWidget->byteSpacingWidth();
}
int ByteArrayView::noOfGroupedBytes() const
{
    return mWidget->noOfGroupedBytes();
}
int ByteArrayView::groupSpacingWidth() const
{
    return mWidget->groupSpacingWidth();
}
int ByteArrayView::binaryGapWidth() const
{
    return mWidget->binaryGapWidth();
}

bool ByteArrayView::isOverwriteOnly() const
{
    return mWidget->isOverwriteOnly();
}

void ByteArrayView::setOverwriteMode(bool overwriteMode)
{
    mWidget->setOverwriteMode(overwriteMode);
}

void ByteArrayView::setViewModus(int viewModus)
{
    mWidget->setViewModus(viewModus);
}
int ByteArrayView::viewModus() const
{
    return mWidget->viewModus();
}

void ByteArrayView::setFontByGlobalSettings()
{
    mWidget->propagateFont(QFontDatabase::systemFont(QFontDatabase::FixedFont));
}

}

// needed for SelectedDataCutExtension
#include "bytearrayview.moc"
