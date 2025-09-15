/*
    SPDX-FileCopyrightText: 2019, 2025 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef OKTETA_UNDOREDOCONTROLLER_HPP
#define OKTETA_UNDOREDOCONTROLLER_HPP

// lib
#include "abstractcontroller.hpp"

class QMenu;

namespace Okteta {

class AbstractByteArrayViewPrivate;

class UndoRedoController : public AbstractController
{
public:
    UndoRedoController(AbstractByteArrayViewPrivate* view, AbstractController* parent);
    ~UndoRedoController() override;

public: // AbstractController API
    void handleShortcutOverrideEvent(QKeyEvent* keyEvent) const override;
    [[nodiscard]]
    bool handleKeyPress(QKeyEvent* keyEvent) override;

public:
    int addContextMenuActions(QMenu* menu);

private:
    [[nodiscard]]
    bool undo();
    [[nodiscard]]
    bool redo();

private:
    AbstractByteArrayViewPrivate* const mView;
};

}

#endif
