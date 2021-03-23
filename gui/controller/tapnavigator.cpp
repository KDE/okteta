/*
    This file is part of the Okteta Gui library, made within the KDE community.

    SPDX-FileCopyrightText: 2021 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "tapnavigator.hpp"

// lib
#include <abstractbytearrayview.hpp>
#include <bytearraytableranges.hpp>
// Qt
#include <QTapGesture>

namespace Okteta {

TapNavigator::TapNavigator(AbstractByteArrayView* view)
    : mView(view)
{
}

bool TapNavigator::handleTapGesture(QTapGesture* tapGesture)
{
    if (tapGesture->state() == Qt::GestureFinished) {
        mView->pauseCursor();
        mView->finishByteEdit();

        const QPoint viewportPos = tapGesture->position().toPoint();;
        const QPoint tapPoint = mView->viewportToColumns(viewportPos);
        mView->placeCursor(tapPoint);
        ByteArrayTableRanges* tableRanges = mView->tableRanges();
        tableRanges->removeSelection();
        mView->ensureCursorVisible();

        if (tableRanges->isModified()) {
            mView->updateChanged();
        }
        mView->unpauseCursor();
        mView->emitSelectionSignals();
    }

    return true;
}

}
