/*
    This file is part of the Okteta Gui library, made within the KDE community.

    SPDX-FileCopyrightText: 2019 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "undoredocontroller.hpp"

// lib
#include <abstractbytearrayview.hpp>
#include <abstractbytearraymodel.hpp>
#include <versionable.hpp>
// KF
#include <KLocalizedString>
// Qt
#include <QKeyEvent>
#include <QMenu>
#include <QAction>

namespace Okteta {

UndoRedoController::UndoRedoController(AbstractByteArrayView* view, AbstractController* parent)
    : AbstractController(parent)
    , mView(view)
{
}


UndoRedoController::~UndoRedoController() = default;

bool UndoRedoController::handleKeyPress(QKeyEvent* keyEvent)
{
    bool keyUsed = false;

    if (!mView->isReadOnly()) {
        if (keyEvent == QKeySequence::Undo) {
            keyUsed = undo();
        } else if (keyEvent == QKeySequence::Redo) {
            keyUsed = redo();
        }
    }

    return keyUsed ? true : AbstractController::handleKeyPress(keyEvent);
}

int UndoRedoController::addContextMenuActions(QMenu* menu)
{
    if (mView->isReadOnly()) {
        return 0;
    }

    auto versionable = qobject_cast<Versionable*>(mView->byteArrayModel());

    if (!versionable) {
        return 0;
    }

    const auto versionIndex = versionable->versionIndex();
    const auto versionCount = versionable->versionCount();

    auto undoAction = menu->addAction(QIcon::fromTheme(QStringLiteral("edit-undo")),
                                      i18nc("@action:inmenu", "&Undo") + QLatin1Char('\t') + QKeySequence(QKeySequence::Undo).toString(QKeySequence::NativeText),
                                      mView, [this] { undo(); });
    undoAction->setEnabled(0 < versionIndex);
    undoAction->setObjectName(QStringLiteral("edit-undo"));

    auto redoAction = menu->addAction(QIcon::fromTheme(QStringLiteral("edit-redo")),
                                      i18nc("@action:inmenu", "Re&do") + QLatin1Char('\t') + QKeySequence(QKeySequence::Redo).toString(QKeySequence::NativeText),
                                      mView, [this] { redo(); });
    redoAction->setEnabled(versionIndex+1 < versionCount);
    redoAction->setObjectName(QStringLiteral("edit-redo"));

    return 2;
}

bool UndoRedoController::undo()
{
    auto versionable = qobject_cast<Okteta::Versionable*>(mView->byteArrayModel());

    if (!versionable) {
        return false;
    }

    const auto versionIndex = versionable->versionIndex();

    if (0 >= versionIndex) {
        return false;
    }

    versionable->revertToVersionByIndex(versionIndex - 1);
    return true;
}

bool UndoRedoController::redo()
{
    auto versionable = qobject_cast<Okteta::Versionable*>(mView->byteArrayModel());

    if (!versionable) {
        return false;
    }

    const auto versionIndex = versionable->versionIndex();

    if (versionIndex + 1 >= versionable->versionCount()) {
        return false;
    }

    versionable->revertToVersionByIndex(versionIndex + 1);
    return true;
}

}
