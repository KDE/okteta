/*
    This file is part of the Okteta Gui library, made within the KDE community.

    SPDX-FileCopyrightText: 2019, 2025 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "undoredocontroller.hpp"

// lib
#include <abstractbytearrayview_p.hpp>
#include <abstractbytearraymodel.hpp>
#include <versionable.hpp>
// KF
#include <KLocalizedString>
// Qt
#include <QKeyEvent>
#include <QMenu>
#include <QAction>

namespace Okteta {

UndoRedoController::UndoRedoController(AbstractByteArrayViewPrivate* view, AbstractController* parent)
    : AbstractController(parent)
    , mView(view)
{
}


UndoRedoController::~UndoRedoController() = default;

void UndoRedoController::handleShortcutOverrideEvent(QKeyEvent* keyEvent) const
{
    bool isKeyToUse = false;

    if (!mView->isEffectiveReadOnly()) {
        if (keyEvent->matches(QKeySequence::Undo) ||
            keyEvent->matches(QKeySequence::Redo)) {
            isKeyToUse = true;
        }
    }

    if (isKeyToUse) {
        keyEvent->accept();
    } else {
        AbstractController::handleShortcutOverrideEvent(keyEvent);
    }
}


bool UndoRedoController::handleKeyPress(QKeyEvent* keyEvent)
{
    bool keyUsed = false;

    if (!mView->isEffectiveReadOnly()) {
        if (keyEvent->matches(QKeySequence::Undo)) {
            keyUsed = undo();
        } else if (keyEvent->matches(QKeySequence::Redo)) {
            keyUsed = redo();
        }
    }

    return keyUsed ? true : AbstractController::handleKeyPress(keyEvent);
}

int UndoRedoController::addContextMenuActions(QMenu* menu)
{
    if (mView->isEffectiveReadOnly()) {
        return 0;
    }

    auto* versionable = qobject_cast<Versionable*>(mView->byteArrayModel());

    if (!versionable) {
        return 0;
    }

    const auto versionIndex = versionable->versionIndex();
    const auto versionCount = versionable->versionCount();

    QAction* undoAction = menu->addAction(QIcon::fromTheme(QStringLiteral("edit-undo")),
                                          i18nc("@action:inmenu", "&Undo") + QLatin1Char('\t') + QKeySequence(QKeySequence::Undo).toString(QKeySequence::NativeText),
                                          mView->q_func(), [this] { std::ignore = undo(); });
    undoAction->setEnabled(0 < versionIndex);
    undoAction->setObjectName(QStringLiteral("edit-undo"));

    QAction* redoAction = menu->addAction(QIcon::fromTheme(QStringLiteral("edit-redo")),
                                          i18nc("@action:inmenu", "Re&do") + QLatin1Char('\t') + QKeySequence(QKeySequence::Redo).toString(QKeySequence::NativeText),
                                          mView->q_func(), [this] { std::ignore = redo(); });
    redoAction->setEnabled(versionIndex+1 < versionCount);
    redoAction->setObjectName(QStringLiteral("edit-redo"));

    return 2;
}

bool UndoRedoController::undo()
{
    auto* versionable = qobject_cast<Okteta::Versionable*>(mView->byteArrayModel());

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
    auto* versionable = qobject_cast<Okteta::Versionable*>(mView->byteArrayModel());

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
