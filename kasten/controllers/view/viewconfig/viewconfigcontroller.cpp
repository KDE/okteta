/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2006-2010, 2012 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "viewconfigcontroller.hpp"

// lib
#include "bytesperlinedialog.hpp"
#include "bytespergroupdialog.hpp"
// Okteta Kasten gui
#include <Kasten/Okteta/ByteArrayView>
// Okteta core
#include <Okteta/CharCodec>
// KF
#include <KXMLGUIClient>
#include <KLocalizedString>
#include <KActionCollection>
#include <KSelectAction>
#include <KToggleAction>
// Qt
#include <QApplication>

namespace Kasten {

ViewConfigController::ViewConfigController(KXMLGUIClient* guiClient)
{
    KActionCollection* actionCollection = guiClient->actionCollection();

    // Offset coding
    mOffsetCodingAction = new KSelectAction(i18nc("@title:menu", "&Offset Coding"), this);
    mOffsetCodingAction->setObjectName(QStringLiteral("view_offsetcoding"));
    mOffsetCodingAction->setItems(QStringList {
        i18nc("@item:inmenu offset in the hexadecimal format",
              "&Hexadecimal"),
        i18nc("@item:inmenu offset in the decimal format",
              "&Decimal"),
        i18nc("@item:inmenu offset in the octal format",
              "&Octal"),
    });
    connect(mOffsetCodingAction, &KSelectAction::indexTriggered,
            this, &ViewConfigController::setOffsetCoding);
    mOffsetCodingAction->setEnabled(false);

    // value valueCoding
    mCodingAction = new KSelectAction(i18nc("@title:menu", "&Value Coding"), this);
    mCodingAction->setObjectName(QStringLiteral("view_valuecoding"));
    mCodingAction->setItems(QStringList {
        i18nc("@item:inmenu encoding of the bytes as values in the hexadecimal format",
              "&Hexadecimal"),
        i18nc("@item:inmenu encoding of the bytes as values in the decimal format",
              "&Decimal"),
        i18nc("@item:inmenu encoding of the bytes as values in the octal format",
              "&Octal"),
        i18nc("@item:inmenu encoding of the bytes as values in the binary format",
              "&Binary"),
    });
    connect(mCodingAction, &KSelectAction::indexTriggered,
            this, &ViewConfigController::setValueCoding);
    mCodingAction->setEnabled(false);

    // char valueCoding
    mEncodingAction = new KSelectAction(i18nc("@title:menu", "&Char Coding"), this);
    mEncodingAction->setObjectName(QStringLiteral("view_charencoding"));
    mEncodingAction->setItems(Okteta::CharCodec::codecNames());
    connect(mEncodingAction, &KSelectAction::indexTriggered,
            this, &ViewConfigController::setCharCoding);
    mEncodingAction->setEnabled(false);

    mShowsNonprintingAction = new KToggleAction(i18nc("@option:check", "Show &Non-printing Chars"), this);
    mShowsNonprintingAction->setObjectName(QStringLiteral("view_showsnonprinting"));
    connect(mShowsNonprintingAction, &KToggleAction::triggered,
            this, &ViewConfigController::setShowsNonprinting);
    mShowsNonprintingAction->setEnabled(false);

    // bytes per line
    mSetBytesPerLineAction = new QAction(i18nc("@action:inmenu", "Set Bytes per Line…"), this);
    mSetBytesPerLineAction->setObjectName(QStringLiteral("view_bytesperline"));
    connect(mSetBytesPerLineAction, &QAction::triggered,
            this, &ViewConfigController::showBytesPerLineDialog);
    mSetBytesPerLineAction->setEnabled(false);

    // byte groups size
    mSetBytesPerGroupAction = new QAction(i18nc("@action:inmenu", "Set Bytes per Group…"), this);
    mSetBytesPerGroupAction->setObjectName(QStringLiteral("view_bytespergroup"));
    connect(mSetBytesPerGroupAction, &QAction::triggered,
            this, &ViewConfigController::showBytesPerGroupDialog);
    mSetBytesPerGroupAction->setEnabled(false);

    // resize style
    mResizeStyleAction = new KSelectAction(i18nc("@title:menu", "&Dynamic Layout"), this);
    mResizeStyleAction->setObjectName(QStringLiteral("resizestyle"));
    mResizeStyleAction->setItems(QStringList {
        i18nc("@item:inmenu  The layout will not change on size changes.",
              "&Off"),
        i18nc("@item:inmenu  The layout will adapt to the size, but only with complete groups of bytes.",
              "&Wrap Only Complete Byte Groups"),
        i18nc("@item:inmenu  The layout will adapt to the size and fit in as much bytes per line as possible.",
              "&On"),
    });
    connect(mResizeStyleAction, &KSelectAction::indexTriggered,
            this, &ViewConfigController::setLayoutStyle);
    mResizeStyleAction->setEnabled(false);

    mShowOffsetColumnAction = new KToggleAction(i18nc("@option:check", "Show &Line Offset"), this);
    mShowOffsetColumnAction->setObjectName(QStringLiteral("view_lineoffset"));
    actionCollection->setDefaultShortcut(mShowOffsetColumnAction, QKeySequence(Qt::Key_F11));
    connect(mShowOffsetColumnAction, &KToggleAction::triggered,
            this, &ViewConfigController::toggleOffsetColumn);
    mShowOffsetColumnAction->setEnabled(false);

    // show buffer columns
    mToggleColumnsAction = new KSelectAction(i18nc("@title:menu", "&Show Values or Chars"), this);
    mToggleColumnsAction->setObjectName(QStringLiteral("togglecolumns"));
    mToggleColumnsAction->setItems(QStringList {
        i18nc("@item:inmenu", "&Values"),
        i18nc("@item:inmenu", "&Chars"),
        i18nc("@item:inmenu", "Values && Chars"),
    });
    connect(mToggleColumnsAction, &KSelectAction::indexTriggered,
            this, &ViewConfigController::toggleValueCharColumns);
    mToggleColumnsAction->setEnabled(false);

    actionCollection->addActions({
        mCodingAction,
        mEncodingAction,
        // mShowsNonprintingAction, TODOSHOWNONPRINTING hide from UI for now
        mSetBytesPerLineAction,
        mSetBytesPerGroupAction,
        mResizeStyleAction,
        mShowOffsetColumnAction,
        mOffsetCodingAction,
        mToggleColumnsAction,
    });
}

void ViewConfigController::setTargetModel(AbstractModel* model)
{
    ByteArrayView* const byteArrayView = model ? model->findBaseModel<ByteArrayView*>() : nullptr;
    if (byteArrayView == mByteArrayView) {
        return;
    }

    if (mByteArrayView) {
        mByteArrayView->disconnect(this);
    }

    mByteArrayView = byteArrayView;

    const bool hasView = (mByteArrayView != nullptr);
    if (hasView) {
        onOffsetColumnVisibleChanged(mByteArrayView->offsetColumnVisible());
        onOffsetCodingChanged(mByteArrayView->offsetCoding());
        onShowsNonprintingChanged(mByteArrayView->showsNonprinting());
        onValueCodingChanged(mByteArrayView->valueCoding());
        onCharCodecChanged(mByteArrayView->charCodingName());
        onLayoutStyleChanged(mByteArrayView->layoutStyle());
        onVisibleByteArrayCodingsChanged(mByteArrayView->visibleByteArrayCodings());

        connect(mByteArrayView, &ByteArrayView::offsetColumnVisibleChanged,
                this, &ViewConfigController::onOffsetColumnVisibleChanged);
        connect(mByteArrayView, &ByteArrayView::offsetCodingChanged, this, &ViewConfigController::onOffsetCodingChanged);
        connect(mByteArrayView, &ByteArrayView::showsNonprintingChanged,
                this, &ViewConfigController::onShowsNonprintingChanged);
        connect(mByteArrayView, &ByteArrayView::valueCodingChanged, this, &ViewConfigController::onValueCodingChanged);
        connect(mByteArrayView, &ByteArrayView::charCodecChanged,
                this, &ViewConfigController::onCharCodecChanged);
        connect(mByteArrayView, &ByteArrayView::layoutStyleChanged, this, &ViewConfigController::onLayoutStyleChanged);
        connect(mByteArrayView, &ByteArrayView::visibleByteArrayCodingsChanged,
                this, &ViewConfigController::onVisibleByteArrayCodingsChanged);
    }

    mOffsetCodingAction->setEnabled(hasView);
    mCodingAction->setEnabled(hasView);
    mEncodingAction->setEnabled(hasView);
    mShowsNonprintingAction->setEnabled(hasView);
    mSetBytesPerLineAction->setEnabled(hasView);
    mSetBytesPerGroupAction->setEnabled(hasView);
    mResizeStyleAction->setEnabled(hasView);
    mShowOffsetColumnAction->setEnabled(hasView);
    mToggleColumnsAction->setEnabled(hasView);
}

void ViewConfigController::setValueCoding(int valueCoding)
{
    mByteArrayView->setValueCoding(valueCoding);
}

void ViewConfigController::setShowsNonprinting(bool on)
{
    mByteArrayView->setShowsNonprinting(on);
}

void ViewConfigController::toggleOffsetColumn(bool on)
{
    mByteArrayView->toggleOffsetColumn(on);
}

void ViewConfigController::setOffsetCoding(int offsetCoding)
{
    mByteArrayView->setOffsetCoding(offsetCoding);
}

void ViewConfigController::showBytesPerLineDialog()
{
    auto* dialog = new BytesPerLineDialog(QApplication::activeWindow());
    dialog->setBytesPerLine(mByteArrayView->noOfBytesPerLine());
    connect(dialog, &BytesPerLineDialog::bytesPerLineAccepted,
            this, &ViewConfigController::setBytesPerLine);
    dialog->open();
}

void ViewConfigController::setBytesPerLine(int bytesPerLine)
{
    mByteArrayView->setNoOfBytesPerLine(bytesPerLine);

    // TODO: change should be signalled and the action listen to that
    mResizeStyleAction->setCurrentItem(mByteArrayView->layoutStyle());
}

void ViewConfigController::showBytesPerGroupDialog()
{
    auto* dialog = new BytesPerGroupDialog(QApplication::activeWindow());
    dialog->setGroupedBytesCount(mByteArrayView->noOfGroupedBytes());
    connect(dialog, &BytesPerGroupDialog::bytesPerGroupAccepted,
            this, &ViewConfigController::setBytesPerGroup);
    dialog->open();
}

void ViewConfigController::setBytesPerGroup(int bytesPerGroup)
{
    mByteArrayView->setNoOfGroupedBytes(bytesPerGroup);
}

void ViewConfigController::setLayoutStyle(int layoutStyle)
{
    mByteArrayView->setLayoutStyle(layoutStyle);
}

void ViewConfigController::setCharCoding(int charCoding)
{
    mByteArrayView->setCharCoding(Okteta::CharCodec::codecNames()[charCoding]);
}

void ViewConfigController::toggleValueCharColumns(int visibleColumns)
{
    mByteArrayView->setVisibleByteArrayCodings(visibleColumns + 1);
}

void ViewConfigController::onOffsetColumnVisibleChanged(bool offsetColumnVisible)
{
    mShowOffsetColumnAction->setChecked(offsetColumnVisible);
}

void ViewConfigController::onOffsetCodingChanged(int offsetCoding)
{
    mOffsetCodingAction->setCurrentItem(offsetCoding);
}

void ViewConfigController::onShowsNonprintingChanged(bool showsNonprinting)
{
    mShowsNonprintingAction->setChecked(showsNonprinting);
}

void ViewConfigController::onValueCodingChanged(int valueCoding)
{
    mCodingAction->setCurrentItem(valueCoding);
}

void ViewConfigController::onCharCodecChanged(const QString& charCodecName)
{
    const int charCodingIndex = Okteta::CharCodec::codecNames().indexOf(charCodecName);

    mEncodingAction->setCurrentItem(charCodingIndex);
}

void ViewConfigController::onLayoutStyleChanged(int layoutStyle)
{
    mResizeStyleAction->setCurrentItem(layoutStyle);
}

void ViewConfigController::onVisibleByteArrayCodingsChanged(int visibleByteArrayCodings)
{
    mToggleColumnsAction->setCurrentItem(visibleByteArrayCodings - 1);
}

}

#include "moc_viewconfigcontroller.cpp"
