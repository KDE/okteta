/*
    This file is part of the Okteta Gui library, made within the KDE community.

    SPDX-FileCopyrightText: 2004, 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

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

bool TabController::handleKeyPress(QKeyEvent* keyEvent)
{
    bool keyUsed = false;

    const bool tabPressed = (keyEvent->key() == Qt::Key_Tab);
    const bool backTabPressed = (keyEvent->key() == Qt::Key_Backtab);

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

    return keyUsed ? true : AbstractController::handleKeyPress(keyEvent);
}

}
