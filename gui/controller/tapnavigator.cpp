/*
    This file is part of the Okteta Gui library, made within the KDE community.

    SPDX-FileCopyrightText: 2021 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "tapnavigator.hpp"

// lib
#include <abstractbytearrayview_p.hpp>
#include <bytearraytableranges.hpp>
// Qt
#include <QTapGesture>

namespace Okteta {

TapNavigator::TapNavigator(AbstractByteArrayViewPrivate* view)
    : mView(view)
{
}

bool TapNavigator::handleTapGesture(QTapGesture* tapGesture)
{
    if (tapGesture->state() == Qt::GestureFinished) {
        mView->pauseCursor();
        mView->finishByteEditor();

        const QPoint viewportPos = tapGesture->position().toPoint();;
        const QPoint tapPoint = mView->viewportToColumns(viewportPos);
        mView->placeCursor(tapPoint);
        ByteArrayTableRanges* const tableRanges = mView->tableRanges();
        tableRanges->removeSelection();
        mView->ensureCursorVisible();

        if (tableRanges->isModified()) {
            mView->updateChanged();
        }
        mView->unpauseCursor();
        mView->emitSelectionUpdates();
    }

    return true;
}

}
