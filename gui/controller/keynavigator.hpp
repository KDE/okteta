/*
    This file is part of the Okteta Gui library, made within the KDE community.

    SPDX-FileCopyrightText: 2004, 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef OKTETA_KEYNAVIGATOR_HPP
#define OKTETA_KEYNAVIGATOR_HPP

// lib
#include "abstractcontroller.hpp"

class QMenu;

namespace Okteta {
class AbstractByteArrayView;

class KeyNavigator : public AbstractController
{
private:
    enum MoveAction
    {
        MoveBackward,
        MoveWordBackward,
        MoveForward,
        MoveWordForward,
        MoveUp,
        MovePgUp,
        MoveDown,
        MovePgDown,
        MoveLineStart,
        MoveHome,
        MoveLineEnd,
        MoveEnd
    };

public:
    KeyNavigator(AbstractByteArrayView* view, AbstractController* parent);

public: // AbstractController API
    bool handleKeyPress(QKeyEvent* keyEvent) override;

public:
    int addContextMenuActions(QMenu* menu);

private:
    /** moves the cursor according to the action, handles all drawing */
    void moveCursor(MoveAction action, bool select);
    void selectAll();

private:
    AbstractByteArrayView* mView;
};

}

#endif
