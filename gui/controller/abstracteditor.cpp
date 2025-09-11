/*
    This file is part of the Okteta Gui library, made within the KDE community.

    SPDX-FileCopyrightText: 2004, 2008, 2025 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "abstracteditor.hpp"

// lib
#include <bytearraytablelayout.hpp>
#include <bytearraytablecursor.hpp>
#include <abstractbytearrayview_p.hpp>
// Okteta core
#include <Okteta/AbstractByteArrayModel>
#include <Okteta/TextByteArrayAnalyzer>
// Qt
#include <QKeyEvent>

namespace Okteta {

AbstractEditor::AbstractEditor(AbstractByteArrayViewPrivate* view, AbstractController* parent)
    : AbstractController(parent)
    , mView(view)
{
}

AbstractEditor::~AbstractEditor() = default;

void AbstractEditor::handleShortcutOverrideEvent(QKeyEvent* keyEvent) const
{
    bool isKeyToUse = false;

    if (keyEvent->matches(QKeySequence::Delete) ||
        keyEvent->matches(QKeySequence::DeleteStartOfWord) ||
        keyEvent->matches(QKeySequence::DeleteEndOfWord)) {
        isKeyToUse = true;
    } else {
        const Qt::KeyboardModifiers keyModifiers = keyEvent->modifiers() & ~(Qt::KeypadModifier | Qt::GroupSwitchModifier);;

        if (keyModifiers == Qt::NoModifier) {
            const int keyCode = keyEvent->key();
            if ((keyCode == Qt::Key_Insert) ||
                (keyCode == Qt::Key_Backspace)) {
                isKeyToUse = true;
            }
        }
    }

    if (isKeyToUse) {
        keyEvent->accept();
    } else {
        AbstractController::handleShortcutOverrideEvent(keyEvent);
    }
}


bool AbstractEditor::handleKeyPress(QKeyEvent* keyEvent)
{
    bool keyUsed = false;

    // we only care for cursor keys and the like, won't hardcode any other keys
    // we also don't check whether the commands are allowed
    // as the commands are also available as API so the check has to be done
    // in each command anyway
    if (keyEvent->matches(QKeySequence::Delete)) {
        if (mView->hasSelectedBytes()) {
            mView->removeSelectedBytes();
        } else {
            doEditAction(CharDelete);
        }
        keyUsed = true;
    } else if (keyEvent->matches(QKeySequence::DeleteStartOfWord)) {
        // TODO: decide behaviour with selection, for now okteta legacy one
        if (mView->hasSelectedBytes()) {
            mView->removeSelectedBytes();
        } else {
            doEditAction(WordBackspace);
        }
        keyUsed = true;
    } else if (keyEvent->matches(QKeySequence::DeleteEndOfWord)) {
        // TODO: decide behaviour with selection, for now okteta legacy one
        if (mView->hasSelectedBytes()) {
            mView->removeSelectedBytes();
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
                if (mView->hasSelectedBytes()) {
                    mView->removeSelectedBytes();
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
    ByteArrayTableCursor* const tableCursor = mView->tableCursor();

    switch (action)
    {
    case CharDelete:
        if (!mView->isOverwriteMode()) {
            const Address index = tableCursor->realIndex();
            if (index < mView->tableLayout()->length()) {
                std::ignore = byteArrayModel->removeBytes(AddressRange::fromWidth(index, 1));
            }
        }
        break;
    case WordDelete: // kills data until the start of the next word
        if (!mView->isOverwriteMode()) {
            const Address index = tableCursor->realIndex();
            if (index < mView->tableLayout()->length()) {
                const TextByteArrayAnalyzer textAnalyzer(byteArrayModel, mView->charCodec());
                const Address end = textAnalyzer.indexOfBeforeNextWordStart(index);
                std::ignore = byteArrayModel->removeBytes(AddressRange(index, end));
            }
        }
        break;
    case CharBackspace:
        if (mView->isOverwriteMode()) {
            mView->pauseCursor();
            tableCursor->gotoPreviousByte();
            mView->ensureCursorVisible();
            mView->unpauseCursor();
        } else {
            const Address deleteIndex = tableCursor->realIndex() - 1;
            if (deleteIndex >= 0) {
                std::ignore = byteArrayModel->removeBytes(AddressRange::fromWidth(deleteIndex, 1));
            }
        }
        break;
    case WordBackspace:
    {
        const int leftIndex = tableCursor->realIndex() - 1;
        if (leftIndex >= 0) {
            const TextByteArrayAnalyzer textAnalyzer(byteArrayModel, mView->charCodec());
            const Address wordStart = textAnalyzer.indexOfPreviousWordStart(leftIndex);
            if (!mView->isOverwriteMode()) {
                std::ignore = byteArrayModel->removeBytes(AddressRange(wordStart, leftIndex));
            }
        }
    }
    }
}

}
