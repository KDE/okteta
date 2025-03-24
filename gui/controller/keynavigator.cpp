/*
    This file is part of the Okteta Gui library, made within the KDE community.

    SPDX-FileCopyrightText: 2004, 2008, 2025 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
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

void KeyNavigator::handleShortcutOverrideEvent(QKeyEvent* keyEvent) const
{
    bool isKeyToUse = false;

    if (keyEvent->matches(QKeySequence::SelectAll) ||
        keyEvent->matches(QKeySequence::MoveToPreviousChar) ||
        keyEvent->matches(QKeySequence::SelectPreviousChar) ||
        keyEvent->matches(QKeySequence::MoveToPreviousWord) ||
        keyEvent->matches(QKeySequence::SelectPreviousWord) ||
        keyEvent->matches(QKeySequence::MoveToNextChar) ||
        keyEvent->matches(QKeySequence::SelectNextChar) ||
        keyEvent->matches(QKeySequence::MoveToNextWord) ||
        keyEvent->matches(QKeySequence::SelectNextWord) ||
        keyEvent->matches(QKeySequence::MoveToStartOfLine) ||
        keyEvent->matches(QKeySequence::SelectStartOfLine)  ||
        keyEvent->matches(QKeySequence::MoveToEndOfLine) ||
        keyEvent->matches(QKeySequence::SelectEndOfLine) ||
        keyEvent->matches(QKeySequence::MoveToPreviousLine) ||
        keyEvent->matches(QKeySequence::SelectPreviousLine) ||
        keyEvent->matches(QKeySequence::MoveToNextLine) ||
        keyEvent->matches(QKeySequence::SelectNextLine) ||
        keyEvent->matches(QKeySequence::MoveToPreviousPage) ||
        keyEvent->matches(QKeySequence::SelectPreviousPage) ||
        keyEvent->matches(QKeySequence::MoveToNextPage) ||
        keyEvent->matches(QKeySequence::SelectNextPage) ||
        keyEvent->matches(QKeySequence::MoveToStartOfDocument) ||
        keyEvent->matches(QKeySequence::SelectStartOfDocument) ||
        keyEvent->matches(QKeySequence::MoveToEndOfDocument) ||
        keyEvent->matches(QKeySequence::SelectEndOfDocument)) {
        isKeyToUse = true;
    }

    if (isKeyToUse) {
        keyEvent->accept();
    } else {
        AbstractController::handleShortcutOverrideEvent(keyEvent);
    }
}

bool KeyNavigator::handleKeyPress(QKeyEvent* keyEvent)
{
    bool keyUsed = false;

    // we only care for cursor keys and the like, won't hardcode any other keys
    // we also don't check whether the commands are allowed
    // as the commands are also available as API so the check has to be done
    // in each command anyway
    if (keyEvent->matches(QKeySequence::SelectAll)) {
        selectAll();
        keyUsed = true;
    } else if (keyEvent->matches(QKeySequence::MoveToPreviousChar)) {
        moveCursor(MoveBackward, Unselect);
        keyUsed = true;
    } else if (keyEvent->matches(QKeySequence::SelectPreviousChar)) {
        moveCursor(MoveBackward, Select);
        keyUsed = true;
    } else if (keyEvent->matches(QKeySequence::MoveToPreviousWord)) {
        moveCursor(MoveWordBackward, Unselect);
        keyUsed = true;
    } else if (keyEvent->matches(QKeySequence::SelectPreviousWord)) {
        moveCursor(MoveWordBackward, Select);
        keyUsed = true;
    } else if (keyEvent->matches(QKeySequence::MoveToNextChar)) {
        moveCursor(MoveForward, Unselect);
        keyUsed = true;
    } else if (keyEvent->matches(QKeySequence::SelectNextChar)) {
        moveCursor(MoveForward, Select);
        keyUsed = true;
    } else if (keyEvent->matches(QKeySequence::MoveToNextWord)) {
        moveCursor(MoveWordForward, Unselect);
        keyUsed = true;
    } else if (keyEvent->matches(QKeySequence::SelectNextWord)) {
        moveCursor(MoveWordForward, Select);
        keyUsed = true;
    } else if (keyEvent->matches(QKeySequence::MoveToStartOfLine)) {
        moveCursor(MoveLineStart, Unselect);
        keyUsed = true;
    } else if (keyEvent->matches(QKeySequence::SelectStartOfLine)) {
        moveCursor(MoveLineStart, Select);
        keyUsed = true;
    } else if (keyEvent->matches(QKeySequence::MoveToEndOfLine)) {
        moveCursor(MoveLineEnd, Unselect);
        keyUsed = true;
    } else if (keyEvent->matches(QKeySequence::SelectEndOfLine)) {
        moveCursor(MoveLineEnd, Select);
        keyUsed = true;
    } else if (keyEvent->matches(QKeySequence::MoveToPreviousLine)) {
        moveCursor(MoveUp, Unselect);
        keyUsed = true;
    } else if (keyEvent->matches(QKeySequence::SelectPreviousLine)) {
        moveCursor(MoveUp, Select);
        keyUsed = true;
    } else if (keyEvent->matches(QKeySequence::MoveToNextLine)) {
        moveCursor(MoveDown, Unselect);
        keyUsed = true;
    } else if (keyEvent->matches(QKeySequence::SelectNextLine)) {
        moveCursor(MoveDown, Select);
        keyUsed = true;
    } else if (keyEvent->matches(QKeySequence::MoveToPreviousPage)) {
        moveCursor(MovePgUp, Unselect);
        keyUsed = true;
    } else if (keyEvent->matches(QKeySequence::SelectPreviousPage)) {
        moveCursor(MovePgUp, Select);
        keyUsed = true;
    } else if (keyEvent->matches(QKeySequence::MoveToNextPage)) {
        moveCursor(MovePgDown, Unselect);
        keyUsed = true;
    } else if (keyEvent->matches(QKeySequence::SelectNextPage)) {
        moveCursor(MovePgDown, Select);
        keyUsed = true;
    } else if (keyEvent->matches(QKeySequence::MoveToStartOfDocument)) {
        moveCursor(MoveHome, Unselect);
        keyUsed = true;
    } else if (keyEvent->matches(QKeySequence::SelectStartOfDocument)) {
        moveCursor(MoveHome, Select);
        keyUsed = true;
    } else if (keyEvent->matches(QKeySequence::MoveToEndOfDocument)) {
        moveCursor(MoveEnd, Unselect);
        keyUsed = true;
    } else if (keyEvent->matches(QKeySequence::SelectEndOfDocument)) {
        moveCursor(MoveEnd, Select);
        keyUsed = true;
    }

    return keyUsed ? true : AbstractController::handleKeyPress(keyEvent);
}

void KeyNavigator::moveCursor(MoveAction action, SelectAction selectAction)
{
    mView->pauseCursor();
    mView->finishByteEdit();

    ByteArrayTableCursor* tableCursor = mView->tableCursor();
    ByteArrayTableRanges* tableRanges = mView->tableRanges();

    if (selectAction == Select) {
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

    if (selectAction == Select) {
        tableRanges->setSelectionEnd(tableCursor->realIndex());
    }

    mView->ensureCursorVisible();

    mView->updateChanged();
    mView->unpauseCursor();
    mView->emitSelectionSignals();
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
