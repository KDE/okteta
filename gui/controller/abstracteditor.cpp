/*
    This file is part of the Okteta Gui library, made within the KDE community.

    SPDX-FileCopyrightText: 2004, 2008, 2025 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "abstracteditor.hpp"

// lib
#include <bytearraytablelayout.hpp>
#include <bytearraytablecursor.hpp>
#include <abstractbytearrayview.hpp>
// Okteta core
#include <Okteta/AbstractByteArrayModel>
#include <Okteta/TextByteArrayAnalyzer>
// Qt
#include <QKeyEvent>

namespace Okteta {

AbstractEditor::AbstractEditor(ByteArrayTableCursor* cursor, AbstractByteArrayView* view, AbstractController* parent)
    : AbstractController(parent)
    , mCursor(cursor)
    , mView(view)
{
}

AbstractEditor::~AbstractEditor() = default;

bool AbstractEditor::handleKeyPress(QKeyEvent* keyEvent)
{
    bool keyUsed = false;

    // we only care for cursor keys and the like, won't hardcode any other keys
    // we also don't check whether the commands are allowed
    // as the commands are also available as API so the check has to be done
    // in each command anyway
    if (keyEvent->matches(QKeySequence::Delete)) {
        if (mView->hasSelectedData()) {
            mView->removeSelectedData();
        } else {
            doEditAction(CharDelete);
        }
        keyUsed = true;
    } else if (keyEvent->matches(QKeySequence::DeleteStartOfWord)) {
        // TODO: descide behaviour with selection, for now okteta legacy one
        if (mView->hasSelectedData()) {
            mView->removeSelectedData();
        } else {
            doEditAction(WordBackspace);
        }
        keyUsed = true;
    } else if (keyEvent->matches(QKeySequence::DeleteEndOfWord)) {
        // TODO: descide behaviour with selection, for now okteta legacy one
        if (mView->hasSelectedData()) {
            mView->removeSelectedData();
        } else {
            doEditAction(WordDelete);
        }
        keyUsed = true;
    } else {
        const Qt::KeyboardModifiers keyModifiers = keyEvent->modifiers() & ~(Qt::KeypadModifier | Qt::GroupSwitchModifier);;

        if (keyModifiers == Qt::NoModifier) {
            switch (keyEvent->key()) {
            case Qt::Key_Insert:
                mView->setOverwriteMode(!mView->isOverwriteMode());
                keyUsed = true;
                break;
            case Qt::Key_Backspace:
                if (mView->hasSelectedData()) {
                    mView->removeSelectedData();
                } else {
                    doEditAction(CharBackspace);
                }
                keyUsed = true;
                break;
            default:
                break;
            }
        }
    }

    return keyUsed ? true : AbstractController::handleKeyPress(keyEvent);
}

void AbstractEditor::doEditAction(EditAction action)
{
    Okteta::AbstractByteArrayModel* byteArrayModel = mView->byteArrayModel();
    switch (action)
    {
    case CharDelete:
        if (!mView->isOverwriteMode()) {
            const Address index = mCursor->realIndex();
            if (index < mView->layout()->length()) {
                byteArrayModel->remove(AddressRange::fromWidth(index, 1));
            }
        }
        break;
    case WordDelete: // kills data until the start of the next word
        if (!mView->isOverwriteMode()) {
            const Address index = mCursor->realIndex();
            if (index < mView->layout()->length()) {
                const TextByteArrayAnalyzer textAnalyzer(byteArrayModel, mView->charCodec());
                const Address end = textAnalyzer.indexOfBeforeNextWordStart(index);
                byteArrayModel->remove(AddressRange(index, end));
            }
        }
        break;
    case CharBackspace:
        if (mView->isOverwriteMode()) {
            mView->pauseCursor();
            mCursor->gotoPreviousByte();
            mView->ensureCursorVisible();
            mView->unpauseCursor();
        } else {
            const Address deleteIndex = mCursor->realIndex() - 1;
            if (deleteIndex >= 0) {
                byteArrayModel->remove(AddressRange::fromWidth(deleteIndex, 1));
            }
        }
        break;
    case WordBackspace:
    {
        const int leftIndex = mCursor->realIndex() - 1;
        if (leftIndex >= 0) {
            const TextByteArrayAnalyzer textAnalyzer(byteArrayModel, mView->charCodec());
            const Address wordStart = textAnalyzer.indexOfPreviousWordStart(leftIndex);
            if (!mView->isOverwriteMode()) {
                byteArrayModel->remove(AddressRange(wordStart, leftIndex));
            }
        }
    }
    }
}

}
