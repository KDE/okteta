/*
    This file is part of the Okteta Gui library, made within the KDE community.

    SPDX-FileCopyrightText: 2004, 2008, 2025 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "valueeditor.hpp"

// lib
#include <bytearraytablecursor.hpp>
#include <abstractbytearrayview_p.hpp>
// Okteta core
#include <Okteta/AbstractByteArrayModel>
#include <Okteta/ChangesDescribable>
#include <Okteta/ValueCodec>
// KF
#include <KLocalizedString>
// Qt
#include <QKeyEvent>

namespace Okteta {

ValueEditor::ValueEditor(AbstractByteArrayViewPrivate* view, AbstractController* parent)
    : AbstractEditor(view, parent)
    , mInEditMode(false)
    , mEditModeByInsert(false)
{
}

ValueEditor::~ValueEditor() = default;

void ValueEditor::adaptToValueCodecChange()
{
    const uint newCodingWidth = mView->valueCodec()->encodingWidth();
    mValueString.resize(newCodingWidth);
}

void ValueEditor::startEdit(const QString& description)
{
    Q_ASSERT(!mInEditMode);

    Okteta::AbstractByteArrayModel* byteArrayModel = mView->byteArrayModel();
    auto* changesDescribable = qobject_cast<Okteta::ChangesDescribable*>(byteArrayModel);

    if (changesDescribable) {
        changesDescribable->openGroupedChange(description);
    }

    mInEditMode = true;
}

void ValueEditor::cancelEdit(bool undoChanges)
{
//     Q_ASSERT( mInEditMode );
    if (!mInEditMode) {
        return;
    }

    mInEditMode = false;

    if (undoChanges) {
        Okteta::AbstractByteArrayModel* byteArrayModel = mView->byteArrayModel();
        auto* changesDescribable = qobject_cast<Okteta::ChangesDescribable*>(byteArrayModel);

        // TODO: if !changesDescribable the changes need to be undone, too
        if (changesDescribable) {
            changesDescribable->cancelGroupedChange();
        }
    }
}

void ValueEditor::finishEdit()
{
    if (!mInEditMode) {
        return;
    }

    mInEditMode = false;

    Okteta::AbstractByteArrayModel* byteArrayModel = mView->byteArrayModel();
    auto* changesDescribable = qobject_cast<Okteta::ChangesDescribable*>(byteArrayModel);

    if (changesDescribable) {
        changesDescribable->closeGroupedChange();
    }
}

void ValueEditor::handleShortcutOverrideEvent(QKeyEvent* keyEvent) const
{
    bool isKeyToUse = false;

    if (!mView->hasSelectedData()) {
        if (keyEvent->matches(QKeySequence::Cancel)) {
            if (mInEditMode) {
                isKeyToUse = true;
            }
        } else {
            const Qt::KeyboardModifiers keyModifiers = keyEvent->modifiers() & ~(Qt::KeypadModifier | Qt::GroupSwitchModifier);;

            switch (keyEvent->key()) {
            case Qt::Key_Plus:
                if (keyModifiers == Qt::NoModifier) {
                    isKeyToUse = true;
                }
                break;
            case Qt::Key_Minus:
                if (keyModifiers == Qt::NoModifier) {
                    isKeyToUse = true;
                }
                break;
            case Qt::Key_Space:
                if (!mInEditMode) {
                    break;
                }
                [[fallthrough]];
            case Qt::Key_Enter:
            case Qt::Key_Return:
                if (keyModifiers == Qt::NoModifier) {
                    isKeyToUse = true;
                }
                break;
            case Qt::Key_Backspace:
                if ((keyModifiers == Qt::NoModifier) && mInEditMode) {
                    isKeyToUse = true;
                }
                break;
            default: {
                // is plain char?
                const QString text = keyEvent->text();
                if (text.isEmpty()) {
                    break;
                }

                const Qt::KeyboardModifiers shiftLessKeyModifiers = keyModifiers & ~Qt::ShiftModifier;
                if (shiftLessKeyModifiers != Qt::NoModifier) {
                    break;
                }

                const QChar enteredChar = text.at(0);
                // no usable char?
                if (!enteredChar.isPrint()) {
                    break;
                }
                isKeyToUse = true;
            }
            }
        }
    }

    if (isKeyToUse) {
        keyEvent->accept();
    } else {
        AbstractEditor::handleShortcutOverrideEvent(keyEvent);
    }
}

bool ValueEditor::handleKeyPress(QKeyEvent* keyEvent)
{
    bool keyUsed = false;

    // TODO: for now we don't touch it if there are selections
    if (!mView->hasSelectedData()) {
        if (keyEvent->matches(QKeySequence::Cancel)) {
            if (mInEditMode) {
                cancelEdit();
                keyUsed = true;
            }
        } else {
            const Qt::KeyboardModifiers keyModifiers = keyEvent->modifiers() & ~(Qt::KeypadModifier | Qt::GroupSwitchModifier);;

            switch (keyEvent->key()) {
            case Qt::Key_Plus:
                if (keyModifiers == Qt::NoModifier) {
                    doValueEditAction(IncValue);
                    keyUsed = true;
                }
                break;
            case Qt::Key_Minus:
                if (keyModifiers == Qt::NoModifier) {
                    doValueEditAction(DecValue);
                    keyUsed = true;
                }
                break;
            case Qt::Key_Space:
                if (!mInEditMode) {
                    break;
                }
            // fallthrough
            case Qt::Key_Enter:
            case Qt::Key_Return:
                if (keyModifiers == Qt::NoModifier) {
                    doValueEditAction(mInEditMode ? LeaveValue : EnterValue);
                    keyUsed = true;
                }
                break;
            case Qt::Key_Backspace:
                if ((keyModifiers == Qt::NoModifier) && mInEditMode) {
                    doValueEditAction(ValueBackspace);
                    keyUsed = true;
                }
                break;
            default: {
                // is plain char?
                const QString text = keyEvent->text();
                if (text.isEmpty()) {
                    break;
                }

                const Qt::KeyboardModifiers shiftLessKeyModifiers = keyModifiers & ~Qt::ShiftModifier;
                if (shiftLessKeyModifiers != Qt::NoModifier) {
                    break;
                }

                const QChar enteredChar = text.at(0);
                // no usable char?
                if (!enteredChar.isPrint()) {
                    break;
                }

                const int input = enteredChar.toLatin1();

                const Okteta::ValueCodec* valueCodec = mView->valueCodec();
                if (mInEditMode) {
                    if (mInsertedDigitsCount < valueCodec->encodingWidth()) {
                        doValueEditAction(ValueAppend, input);
                    }
                } else {
                    Byte inputValue = 0;
                    // valid digit?
                    if (valueCodec->appendDigit(&inputValue, input)) {
                        if (mView->isOverwriteMode()) {
                            doValueEditAction(ValueEdit, inputValue);
                        } else {
                            ByteArrayTableCursor* const tableCursor = mView->tableCursor();
                            const Address index = tableCursor->realIndex();

                            startEdit(i18nc("@item name of the change", "Insert"));
                            if (mView->byteArrayModel()->insertBytes(index, &inputValue, 1) > 0) {
                                mEditModeByInsert = true;
                                mOldValue = mEditValue = inputValue;
                                mInsertedDigitsCount = 1;
                                valueCodec->encode(&mValueString, 0, mEditValue);

                                tableCursor->gotoIndex(index);
                                mView->ensureCursorVisible();
//                                 mView->updateCursors();
                                mView->emitCursorPositionChanged();
                            } else {
                                cancelEdit();
                            }
                        }
                    }
                }
                keyUsed = true;
            }
            }
        }
    }

    return keyUsed ? true : AbstractEditor::handleKeyPress(keyEvent);
}

void ValueEditor::doValueEditAction(ValueEditAction Action, int input)
{
    const Okteta::ValueCodec* valueCodec = mView->valueCodec();
    ByteArrayTableCursor* const tableCursor = mView->tableCursor();

    // we are not yet in edit mode?
    if (!mInEditMode) {
        const Address validIndex = tableCursor->validIndex();
        // no valid cursor position?
        if (validIndex == -1 || (!mView->isOverwriteMode() && input == -1) || tableCursor->isBehind()) {
            return;
        }

        startEdit(i18nc("@item name of the change", "Replace"));
        mEditModeByInsert = false; // default, to be overwritten if so

        // save old value
        mOldValue = mEditValue = (unsigned char)mView->byteArrayModel()->byte(validIndex);
        mInsertedDigitsCount = valueCodec->encodingWidth();
    }

    //
    Byte newValue = mEditValue;
    bool stayInEditMode = true;
    bool moveToNext = false;

    switch (Action)
    {
    case ValueEdit:
        newValue = input;
        mEditValue = newValue ^ 255; // force update
        mEditModeByInsert = true;
        mInsertedDigitsCount = 1;
        break;
    case ValueBackspace:
        if (mInsertedDigitsCount > 0) {
            if (newValue > 0) {
                valueCodec->removeLastDigit(&newValue);
            }
            --mInsertedDigitsCount;
        }
        break;
    case EnterValue:
        mEditValue ^= 255; // force update
        break;
    case IncValue:
        if (newValue < 255) {
            ++newValue;
            mInsertedDigitsCount = valueCodec->encodingWidth();
        }
        break;
    case DecValue:
        if (newValue > 0) {
            --newValue;
            mInsertedDigitsCount = valueCodec->encodingWidth();
        }
        break;
    case ValueAppend:
        if (valueCodec->appendDigit(&newValue, input)) {
            ++mInsertedDigitsCount;
            if (mEditModeByInsert &&
                (newValue >= valueCodec->digitsFilledLimit() || mInsertedDigitsCount == valueCodec->encodingWidth())) {
                stayInEditMode = false;
                moveToNext = true;
            }
        }
        break;
    case LeaveValue:
        stayInEditMode = false;
        moveToNext = mEditModeByInsert;
        break;
    }

    // change happened?
    if (newValue != mEditValue) {
        // sync value
        mEditValue = newValue;
        valueCodec->encode(&mValueString, 0, mEditValue);
        std::ignore = mView->byteArrayModel()->replace(tableCursor->index(), 1, &mEditValue, 1);
    }

//     mView->updateCursors();

    if (!stayInEditMode) {
        mView->pauseCursor();
        finishEdit();

        if (moveToNext) {
            tableCursor->gotoNextByte();
        }

        mView->unpauseCursor();
        if (moveToNext) {
            mView->emitCursorPositionChanged();
        }
    }
}

}
