/*
    This file is part of the Okteta Gui library, made within the KDE community.

    Copyright 2019 Friedrich W. H. Kossebau <kossebau@kde.org>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) version 3, or any
    later version accepted by the membership of KDE e.V. (or its
    successor approved by the membership of KDE e.V.), which shall
    act as a proxy defined in Section 6 of version 3 of the license.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library. If not, see <http://www.gnu.org/licenses/>.
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
