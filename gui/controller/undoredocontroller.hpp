/*
    This file is part of the Okteta Gui library, made within the KDE community.

    SPDX-FileCopyrightText: 2019 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef OKTETA_UNDOREDOCONTROLLER_HPP
#define OKTETA_UNDOREDOCONTROLLER_HPP

// lib
#include "abstractcontroller.hpp"

class QMenu;

namespace Okteta {

class AbstractByteArrayView;

class UndoRedoController : public AbstractController
{
public:
    UndoRedoController(AbstractByteArrayView* view, AbstractController* parent);
    ~UndoRedoController() override;

public: // AbstractController API
    bool handleKeyPress(QKeyEvent* keyEvent) override;

public:
    int addContextMenuActions(QMenu* menu);

private:
    bool undo();
    bool redo();

private:
    AbstractByteArrayView* const mView;
};

}

#endif
