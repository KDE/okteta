/*
    This file is part of the Okteta Gui library, made within the KDE community.

    SPDX-FileCopyrightText: 2004, 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

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
    const bool shiftPressed =   keyEvent->modifiers() & Qt::SHIFT;
    const bool controlPressed = keyEvent->modifiers() & Qt::CTRL;
    const bool altPressed =     keyEvent->modifiers() & Qt::ALT;

    bool keyUsed = true;
    // we only care for cursor keys and the like, won't hardcode any other keys
    // we also don't check whether the commands are allowed
    // as the commands are also available as API so the check has to be done
    // in each command anyway
    switch (keyEvent->key())
    {
    case Qt::Key_Delete:
        if (shiftPressed) {
            mView->cut();
        } else if (mView->hasSelectedData()) {
            mView->removeSelectedData();
        } else {
            doEditAction(controlPressed ? WordDelete : CharDelete);
        }
        break;
    case Qt::Key_Insert:
        if (shiftPressed) {
            mView->paste();
        } else if (controlPressed) {
            mView->copy();
        } else {
            mView->setOverwriteMode(!mView->isOverwriteMode());
        }
        break;
    case Qt::Key_Backspace:
        if (altPressed) {
            break;
        } else if (mView->hasSelectedData()) {
            mView->removeSelectedData();
            break;
        }

        doEditAction(controlPressed ? WordBackspace : CharBackspace);
        break;
    case Qt::Key_F16: // "Copy" key on Sun keyboards
        mView->copy();
        break;
    case Qt::Key_F18: // "Paste" key on Sun keyboards
        mView->paste();
        break;
    case Qt::Key_F20: // "Cut" key on Sun keyboards
        mView->cut();
        break;
    default:
        keyUsed = false;
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
