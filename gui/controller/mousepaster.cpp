/*
    This file is part of the Okteta Gui library, made within the KDE community.

    Copyright 2008-2009 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "mousepaster.h"

// lib
#include <bytearraycolumnview.h>
#include <bytearraytableranges.h>
#include <bytearraytablecursor.h>
#include <bytearraytablelayout.h>
// Qt
#include <QtGui/QMouseEvent>


namespace Okteta
{

MousePaster::MousePaster( AbstractByteArrayView* view, AbstractMouseController* parent )
  : AbstractMouseController( view, parent )
{
}

bool MousePaster::handleMousePressEvent( QMouseEvent* mouseEvent )
{
    bool eventUsed = false;

    if( mouseEvent->button() == Qt::MidButton && ! mView->isReadOnly() )
    {
        mView->pauseCursor();
        mView->finishByteEdit();

        ByteArrayTableRanges* tableRanges = mView->tableRanges();
        tableRanges->removeSelection();

        if( tableRanges->isModified() )
        {
            mView->updateChanged();
            mView->viewport()->setCursor( mView->isReadOnly() ? Qt::ArrowCursor : Qt::IBeamCursor );
        }
        mView->unpauseCursor();

        eventUsed = true;
    }

    return eventUsed ? true : AbstractMouseController::handleMousePressEvent( mouseEvent );
}

bool MousePaster::handleMouseReleaseEvent( QMouseEvent* mouseEvent )
{
    bool eventUsed = false;
    // middle mouse button paste?
    if( mouseEvent->button() == Qt::MidButton && ! mView->isReadOnly() )
    {
        const QPoint releasePoint = mView->viewportToColumns( mouseEvent->pos() );

        mView->pauseCursor();
        mView->finishByteEdit();

        mView->placeCursor( releasePoint );

        ByteArrayTableRanges* tableRanges = mView->tableRanges();
        ByteArrayTableCursor* tableCursor = mView->tableCursor();
        // replace no selection?
        if( tableRanges->hasSelection() && ! tableRanges->selectionIncludes(tableCursor->index()) )
            tableRanges->removeSelection();

        mView->pasteFromClipboard( QClipboard::Selection );

        // ensure selection changes to be drawn TODO: create a insert/pasteAtCursor that leaves out drawing
        mView->updateChanged();

        mView->ensureCursorVisible();
        mView->unpauseCursor();

        emit mView->cursorPositionChanged( tableCursor->realIndex() );

        if( tableRanges->selectionJustStarted() )
            tableRanges->removeSelection();

        if( ! mView->isOverwriteMode() ) emit mView->cutAvailable( tableRanges->hasSelection() );
        emit mView->copyAvailable( tableRanges->hasSelection() );
        emit mView->hasSelectedDataChanged( tableRanges->hasSelection() );

        eventUsed = true;
    }

    return eventUsed ? true : AbstractMouseController::handleMouseReleaseEvent( mouseEvent );
}

MousePaster::~MousePaster() {}

}
