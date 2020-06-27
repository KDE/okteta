/*
    This file is part of the Okteta Gui library, made within the KDE community.

    SPDX-FileCopyrightText: 2019 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "clipboardcontroller.hpp"

// lib
#include <abstractbytearrayview.hpp>
#include <abstractbytearraymodel.hpp>
#include <versionable.hpp>
// KF
#include <KLocalizedString>
// Qt
#include <QApplication>
#include <QClipboard>
#include <QKeyEvent>
#include <QMenu>
#include <QAction>

namespace Okteta {

ClipboardController::ClipboardController(AbstractByteArrayView* view, AbstractController* parent)
    : AbstractController(parent)
    , mView(view)
{
}

bool ClipboardController::handleKeyPress(QKeyEvent* keyEvent)
{
    bool keyUsed = false;

    if (keyEvent == QKeySequence::Copy) {
        mView->copy();
        keyUsed = true;
    } else if (!mView->isReadOnly() && !mView->byteArrayModel()->isReadOnly()) {
        if (keyEvent == QKeySequence::Cut) {
            mView->cut();
            keyUsed = true;
        } else if (keyEvent == QKeySequence::Paste) {
            mView->paste();
            keyUsed = true;
        }
    }

    return keyUsed ? true : AbstractController::handleKeyPress(keyEvent);
}

int ClipboardController::addContextMenuActions(QMenu* menu)
{
    auto copyAction = menu->addAction(QIcon::fromTheme(QStringLiteral("edit-copy")),
                                      i18nc("@action:inmenu", "&Copy") + QLatin1Char('\t') + QKeySequence(QKeySequence::Copy).toString(QKeySequence::NativeText),
                                      mView, &AbstractByteArrayView::copy);
    copyAction->setEnabled(mView->hasSelectedData());
    copyAction->setObjectName(QStringLiteral("edit-copy"));

    if (mView->isReadOnly() || mView->byteArrayModel()->isReadOnly()) {
        return 1;
    }

    auto cutAction = menu->addAction(QIcon::fromTheme(QStringLiteral("edit-cut")),
                                     i18nc("@action:inmenu", "Cu&t") + QLatin1Char('\t') + QKeySequence(QKeySequence::Cut).toString(QKeySequence::NativeText),
                                     mView, &AbstractByteArrayView::cut);
    cutAction->setEnabled(mView->hasSelectedData());
    cutAction->setObjectName(QStringLiteral("edit-cut"));

    auto pasteAction = menu->addAction(QIcon::fromTheme(QStringLiteral("edit-paste")),
                                        i18nc("@action:inmenu", "&Paste") + QLatin1Char('\t') + QKeySequence(QKeySequence::Paste).toString(QKeySequence::NativeText),
                                        mView, &AbstractByteArrayView::paste);
    const QMimeData* clipboardMimeData = QApplication::clipboard()->mimeData(QClipboard::Clipboard);
    const bool canReadData = mView->canReadData(clipboardMimeData);
    pasteAction->setEnabled(canReadData);
    pasteAction->setObjectName(QStringLiteral("edit-paste"));

    return 3;
}

}
