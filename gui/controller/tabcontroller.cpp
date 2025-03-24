/*
    This file is part of the Okteta Gui library, made within the KDE community.

    SPDX-FileCopyrightText: 2004, 2008, 2025 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "tabcontroller.hpp"

// lib
#include <abstractbytearrayview.hpp>
// Qt
#include <QKeyEvent>

namespace Okteta {

TabController::TabController(AbstractByteArrayView* view, AbstractController* parent)
    : AbstractController(parent)
    , mView(view)
    , mTabChangesFocus(true)
{
}

void TabController::handleShortcutOverrideEvent(QKeyEvent* keyEvent) const
{
    bool isKeyToUse = false;

    const Qt::KeyboardModifiers keyModifiers = keyEvent->modifiers() & ~(Qt::KeypadModifier | Qt::GroupSwitchModifier);;

    const bool isAtMostShiftModifier = !(keyModifiers & (Qt::ControlModifier | Qt::AltModifier | Qt::MetaModifier));

    if (isAtMostShiftModifier) {
        const int keyCode = keyEvent->key();
        const bool backTabPressed = (keyCode == Qt::Key_Backtab) ||
                                    ((keyCode == Qt::Key_Tab) && (keyModifiers & Qt::ShiftModifier));
        const bool tabPressed = ((keyCode == Qt::Key_Tab) && !(keyModifiers & Qt::ShiftModifier));

        if (tabPressed || backTabPressed) {
            isKeyToUse = true;
        }
    }

    if (isKeyToUse) {
        keyEvent->accept();
    } else {
        AbstractController::handleShortcutOverrideEvent(keyEvent);
    }
}

bool TabController::handleKeyPress(QKeyEvent* keyEvent)
{
    bool keyUsed = false;

    const Qt::KeyboardModifiers keyModifiers = keyEvent->modifiers() & ~(Qt::KeypadModifier | Qt::GroupSwitchModifier);;

    const bool isAtMostShiftModifier = !(keyModifiers & (Qt::ControlModifier | Qt::AltModifier | Qt::MetaModifier));

    if (isAtMostShiftModifier) {
        const int keyCode = keyEvent->key();
        const bool backTabPressed = (keyCode == Qt::Key_Backtab) ||
                                    ((keyCode == Qt::Key_Tab) && (keyModifiers & Qt::ShiftModifier));
        const bool tabPressed = ((keyCode == Qt::Key_Tab) && !(keyModifiers & Qt::ShiftModifier));

        if (tabPressed || backTabPressed) {
            const int visibleCodings = mView->visibleCodings();
            // are we in the char column?
            if (mView->activeCoding() == AbstractByteArrayView::CharCodingId) {
                // in last column we care about tab changes focus
                if ((visibleCodings & AbstractByteArrayView::ValueCodingId) && (!mTabChangesFocus || backTabPressed)) {
                    mView->setActiveCoding(AbstractByteArrayView::ValueCodingId);
                    keyUsed = true;
                }
            }
            // value column then
            else {
                // in last column we care about tab changes focus
                if ((visibleCodings & AbstractByteArrayView::CharCodingId) && (!mTabChangesFocus || tabPressed)) {
                    mView->setActiveCoding(AbstractByteArrayView::CharCodingId);
                    keyUsed = true;
                }
            }
        }
    }

    return keyUsed ? true : AbstractController::handleKeyPress(keyEvent);
}

}
