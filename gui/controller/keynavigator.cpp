/*
    This file is part of the Okteta Gui library, made within the KDE community.

    Copyright 2004,2008 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "keynavigator.hpp"

// lib
#include <bytearraytableranges.hpp>
#include <bytearraytablecursor.hpp>
#include <abstractbytearrayview.hpp>
// Okteta core
#include <Okteta/TextByteArrayAnalyzer>
#include <Okteta/AbstractByteArrayModel>
// KF
#include <KLocalizedString>
// Qt
#include <QKeyEvent>
#include <QMenu>
#include <QAction>

namespace Okteta {

KeyNavigator::KeyNavigator(AbstractByteArrayView* view, AbstractController* parent)
    : AbstractController(parent)
    , mView(view)
{
}

bool KeyNavigator::handleKeyPress(QKeyEvent* keyEvent)
{
    bool keyUsed;

    if (keyEvent == QKeySequence::SelectAll) {
        selectAll();
        keyUsed = true;
    } else if (keyEvent->modifiers() & Qt::ALT) {
        // currently there is no input with the Alt modifier used, so ignore them all
        keyUsed = false;
    } else {
        keyUsed = true;

        const bool shiftPressed =  keyEvent->modifiers() & Qt::SHIFT;
        const bool controlPressed = keyEvent->modifiers() & Qt::CTRL;

        // we only care for cursor keys and the like, won't hardcode any other keys
        // we also don't check whether the commands are allowed
        // as the commands are also available as API so the check has to be done
        // in each command anyway
        switch (keyEvent->key())
        {
        case Qt::Key_Left:
            moveCursor(controlPressed ? MoveWordBackward : MoveBackward, shiftPressed);
            break;
        case Qt::Key_Right:
            moveCursor(controlPressed ? MoveWordForward : MoveForward, shiftPressed);
            break;
        case Qt::Key_Up:
            moveCursor(controlPressed ? MovePgUp : MoveUp, shiftPressed);
            break;
        case Qt::Key_Down:
            moveCursor(controlPressed ? MovePgDown : MoveDown, shiftPressed);
            break;
        case Qt::Key_Home:
            moveCursor(controlPressed ? MoveHome : MoveLineStart, shiftPressed);
            break;
        case Qt::Key_End:
            moveCursor(controlPressed ? MoveEnd : MoveLineEnd, shiftPressed);
            break;
        case Qt::Key_PageUp:
            moveCursor(MovePgUp, shiftPressed);
            break;
        case Qt::Key_PageDown:
            moveCursor(MovePgDown, shiftPressed);
            break;
        default:
            keyUsed = false;
        }
    }

    return keyUsed ? true : AbstractController::handleKeyPress(keyEvent);
}

void KeyNavigator::moveCursor(MoveAction action, bool select)
{
    mView->pauseCursor();
    mView->finishByteEdit();

    ByteArrayTableCursor* tableCursor = mView->tableCursor();
    ByteArrayTableRanges* tableRanges = mView->tableRanges();

    if (select) {
        if (!tableRanges->selectionStarted()) {
            tableRanges->setSelectionStart(tableCursor->realIndex());
        }
    } else {
        tableRanges->removeSelection();
    }

    switch (action)
    {
    case MoveBackward:     tableCursor->gotoPreviousByte(); break;
    case MoveWordBackward: {
        const Okteta::TextByteArrayAnalyzer textAnalyzer(mView->byteArrayModel(), mView->charCodec());
        const int newIndex = textAnalyzer.indexOfPreviousWordStart(tableCursor->realIndex());
        tableCursor->gotoIndex(newIndex);
        break;
    }
    case MoveForward:      tableCursor->gotoNextByte();     break;
    case MoveWordForward:  {
        const Okteta::TextByteArrayAnalyzer textAnalyzer(mView->byteArrayModel(), mView->charCodec());
        const int newIndex = textAnalyzer.indexOfNextWordStart(tableCursor->realIndex());
        tableCursor->gotoCIndex(newIndex);
        break;
    }
    case MoveUp:           tableCursor->gotoUp();             break;
    case MovePgUp:         tableCursor->gotoPageUp();         break;
    case MoveDown:         tableCursor->gotoDown();           break;
    case MovePgDown:       tableCursor->gotoPageDown();       break;
    case MoveLineStart:    tableCursor->gotoLineStart();      break;
    case MoveHome:         tableCursor->gotoStart();          break;
    case MoveLineEnd:      tableCursor->gotoLineEnd();        break;
    case MoveEnd:          tableCursor->gotoEnd();            break;
    }

    if (select) {
        tableRanges->setSelectionEnd(tableCursor->realIndex());
    }

    if (tableRanges->isModified()) {
        mView->emitSelectionSignals(); // TODO: can this be moved somewhere
    }
    emit mView->cursorPositionChanged(tableCursor->realIndex());
    mView->updateChanged();
    mView->ensureCursorVisible();

    mView->unpauseCursor();
}

int KeyNavigator::addContextMenuActions(QMenu* menu)
{
    auto selectAllAction = menu->addAction(QIcon::fromTheme(QStringLiteral("edit-select-all")),
                                           i18nc("@action:inmenu", "Select &All") + QLatin1Char('\t') + QKeySequence(QKeySequence::SelectAll).toString(QKeySequence::NativeText),
                                           mView, [this] { selectAll(); });
    selectAllAction->setEnabled(mView->byteArrayModel()->size() > 0);

    selectAllAction->setObjectName(QStringLiteral("select-all"));

    return 1;
}

void KeyNavigator::selectAll()
{
    mView->selectAll(true);
}

}
