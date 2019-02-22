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

    if (!mView->isReadOnly() && !mView->byteArrayModel()->isReadOnly()) {
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
    if (mView->isReadOnly() || mView->byteArrayModel()->isReadOnly()) {
        return 0;
    }

    auto versionable = qobject_cast<Versionable*>(mView->byteArrayModel());

    if (!versionable) {
        return 0;
    }

    const auto versionIndex = versionable->versionIndex();
    const auto versionCount = versionable->versionCount();

    auto undoAction = menu->addAction(QIcon::fromTheme(QStringLiteral("edit-undo")),
                                      i18n("&Undo") + QLatin1Char('\t') + QKeySequence(QKeySequence::Undo).toString(QKeySequence::NativeText),
                                      mView, [this] { undo(); });
    undoAction->setEnabled(0 < versionIndex);
    undoAction->setObjectName(QStringLiteral("edit-undo"));

    auto redoAction = menu->addAction(QIcon::fromTheme(QStringLiteral("edit-redo")),
                                      i18n("Re&do") + QLatin1Char('\t') + QKeySequence(QKeySequence::Redo).toString(QKeySequence::NativeText),
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
