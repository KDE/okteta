/*
    This file is part of the Okteta Gui library, made within the KDE community.

    SPDX-FileCopyrightText: 2019, 2025 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "clipboardcontroller.hpp"

// lib
#include <abstractbytearrayview_p.hpp>
// KF
#include <KLocalizedString>
// Qt
#include <QApplication>
#include <QClipboard>
#include <QKeyEvent>
#include <QMenu>
#include <QAction>

namespace Okteta {

ClipboardController::ClipboardController(AbstractByteArrayViewPrivate* view, AbstractController* parent)
    : AbstractController(parent)
    , mView(view)
{
}

void ClipboardController::handleShortcutOverrideEvent(QKeyEvent* keyEvent) const
{
    bool isKeyToUse = false;

    if (keyEvent->matches(QKeySequence::Copy)) {
        isKeyToUse = true;
    } else if (!mView->isEffectiveReadOnly()) {
        if (keyEvent->matches(QKeySequence::Cut)) {
            if (!mView->isOverwriteMode()) {
                isKeyToUse = true;
            }
        } else if (keyEvent->matches(QKeySequence::Paste)) {
            isKeyToUse = true;
        }
    }

    if (isKeyToUse) {
        keyEvent->accept();
    } else {
        AbstractController::handleShortcutOverrideEvent(keyEvent);
    }
}

bool ClipboardController::handleKeyPress(QKeyEvent* keyEvent)
{
    bool keyUsed = false;

    if (keyEvent->matches(QKeySequence::Copy)) {
        mView->copyToClipboard();
        keyUsed = true;
    } else if (!mView->isEffectiveReadOnly()) {
        if (keyEvent->matches(QKeySequence::Cut)) {
            if (!mView->isOverwriteMode()) {
                mView->cutToClipboard();
                keyUsed = true;
            }
        } else if (keyEvent->matches(QKeySequence::Paste)) {
            mView->pasteFromClipboard();
            keyUsed = true;
        }
    }

    return keyUsed ? true : AbstractController::handleKeyPress(keyEvent);
}

int ClipboardController::addContextMenuActions(QMenu* menu)
{
    QAction* copyAction = menu->addAction(QIcon::fromTheme(QStringLiteral("edit-copy")),
                                          i18nc("@action:inmenu", "&Copy") + QLatin1Char('\t') + QKeySequence(QKeySequence::Copy).toString(QKeySequence::NativeText),
                                          mView->q_func(), [this] { mView->copyToClipboard(); });
    copyAction->setEnabled(mView->hasSelectedBytes());
    copyAction->setObjectName(QStringLiteral("edit-copy"));

    if (mView->isEffectiveReadOnly()) {
        return 1;
    }

    QAction* cutAction = menu->addAction(QIcon::fromTheme(QStringLiteral("edit-cut")),
                                         i18nc("@action:inmenu", "Cu&t") + QLatin1Char('\t') + QKeySequence(QKeySequence::Cut).toString(QKeySequence::NativeText),
                                         mView->q_func(), [this] { mView->cutToClipboard(); });
    const bool canCutData = mView->hasSelectedBytes() && !mView->isOverwriteMode();
    cutAction->setEnabled(canCutData);
    cutAction->setObjectName(QStringLiteral("edit-cut"));

    QAction* pasteAction = menu->addAction(QIcon::fromTheme(QStringLiteral("edit-paste")),
                                           i18nc("@action:inmenu", "&Paste") + QLatin1Char('\t') + QKeySequence(QKeySequence::Paste).toString(QKeySequence::NativeText),
                                           mView->q_func(), [this] { mView->pasteFromClipboard(); });
    const QMimeData* clipboardMimeData = QApplication::clipboard()->mimeData(QClipboard::Clipboard);
    const bool canInsertBytesFromMimeData = mView->canInsertBytesFromMimeData(clipboardMimeData);
    pasteAction->setEnabled(canInsertBytesFromMimeData);
    pasteAction->setObjectName(QStringLiteral("edit-paste"));

    return 3;
}

}
