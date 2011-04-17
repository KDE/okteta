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

#include "kvalueeditor.h"

// lib
#include <bytearraytablecursor.h>
#include <abstractbytearrayview.h>
// Okteta core
#include <abstractbytearraymodel.h>
#include <changesdescribable.h>
#include <valuecodec.h>
// KDE
#include <KLocale>
// Qt
#include <QtGui/QKeyEvent>


namespace Okteta
{

KValueEditor::KValueEditor( ByteArrayTableCursor* cursor, AbstractByteArrayView* view, KController* parent )
  : KEditor( cursor, view, parent ),
  mInEditMode( false ),
  mEditModeByInsert( false )
{
}

void KValueEditor::adaptToValueCodecChange()
{
    const uint newCodingWidth = mView->valueCodec()->encodingWidth();
    mValueString.resize( newCodingWidth );
}

void KValueEditor::startEdit( const QString &description )
{
    Q_ASSERT( !mInEditMode );

    Okteta::AbstractByteArrayModel* byteArrayModel = mView->byteArrayModel();
    Okteta::ChangesDescribable *changesDescribable =
        qobject_cast<Okteta::ChangesDescribable*>( byteArrayModel );

    if( changesDescribable )
        changesDescribable->openGroupedChange( description );

    mInEditMode = true;
}

void KValueEditor::cancelEdit( bool undoChanges )
{
//     Q_ASSERT( mInEditMode );
    if( !mInEditMode )
        return;

    mInEditMode = false;

    if( undoChanges )
    {
        Okteta::AbstractByteArrayModel* byteArrayModel = mView->byteArrayModel();
        Okteta::ChangesDescribable* changesDescribable =
            qobject_cast<Okteta::ChangesDescribable*>( byteArrayModel );

        // TODO: if !changesDescribable the changes need to be undone, too
        if( changesDescribable )
            changesDescribable->cancelGroupedChange();
    }
}

void KValueEditor::finishEdit()
{
    if( !mInEditMode )
        return;

    mInEditMode = false;

    Okteta::AbstractByteArrayModel* byteArrayModel = mView->byteArrayModel();
    Okteta::ChangesDescribable *changesDescribable =
        qobject_cast<Okteta::ChangesDescribable*>( byteArrayModel );

    if( changesDescribable )
        changesDescribable->closeGroupedChange();
}


bool KValueEditor::handleKeyPress( QKeyEvent *keyEvent )
{
    bool keyUsed = true;

    // TODO: for now we don't touch it if there are selections
    if( !mView->hasSelectedData() )
    {
        //
        switch( keyEvent->key() )
        {
        case Qt::Key_Plus:
            doValueEditAction( IncValue );
            break;
        case Qt::Key_Minus:
            doValueEditAction( DecValue );
            break;
        case Qt::Key_Space:
            if( !mInEditMode )
            {
                keyUsed = false;
                break;
            }
        case Qt::Key_Enter:
        case Qt::Key_Return:
            doValueEditAction( mInEditMode?LeaveValue:EnterValue );
            break;
        case Qt::Key_Escape:
            if( mInEditMode )
                cancelEdit();
            else
                keyUsed = false;
            break;
        case Qt::Key_Backspace:
            if( mInEditMode )
                doValueEditAction( ValueBackspace );
            else
                keyUsed = false;
            break;
        default:
            // is plain char?
            if( keyEvent->text().length() > 0
                && ( !(keyEvent->modifiers()&( Qt::CTRL | Qt::ALT | Qt::META )) ) )
            {
                const int input = keyEvent->text()[0].toAscii();
                // no usable char?
                if( input < 32 )
                {
                    keyUsed = false;
                    break;
                }

                const Okteta::ValueCodec* valueCodec = mView->valueCodec();
                if( mInEditMode )
                {
                    if( mInsertedDigitsCount < valueCodec->encodingWidth() )
                        doValueEditAction( ValueAppend, input );
                }
                else
                {
                    Byte inputValue = 0;
                    // valid digit?
                    if( valueCodec->appendDigit(&inputValue,input) )
                    {
                        if( mView->isOverwriteMode() )
                            doValueEditAction( ValueEdit, inputValue );
                        else
                        {
                            const Address index = mCursor->realIndex();

                            startEdit( i18nc( "name of the change", "Insert" ) );
                            if( mView->byteArrayModel()->insert(index,&inputValue,1) > 0 )
                            {
                                mEditModeByInsert = true;
                                mOldValue = mEditValue = inputValue;
                                mInsertedDigitsCount = 1;
                                valueCodec->encode( mValueString, 0, mEditValue );

                                mCursor->gotoIndex(index);
                                mView->ensureCursorVisible();
//                                 mView->updateCursors();
                                emit mView->cursorPositionChanged( mCursor->realIndex() );
                            }
                            else
                                cancelEdit();
                        }
                    }
                }
            }
            else
                keyUsed = false;
        }
    }
    else
        keyUsed = false;

    return keyUsed ? true : KEditor::handleKeyPress(keyEvent);
}


void KValueEditor::doValueEditAction( KValueEditAction Action, int input )
{
    const Okteta::ValueCodec* valueCodec = mView->valueCodec();

    // we are not yet in edit mode?
    if( !mInEditMode )
    {
        const Address validIndex = mCursor->validIndex();
        // no valid cursor position?
        if( validIndex == -1 || (!mView->isOverwriteMode() && input == -1) || mCursor->isBehind() )
            return;

        startEdit( i18nc( "name of the change", "Replace" ) );
        mEditModeByInsert = false; // default, to be overwritten if so

        // save old value
        mOldValue = mEditValue = (unsigned char)mView->byteArrayModel()->byte( validIndex );
        mInsertedDigitsCount = valueCodec->encodingWidth();
    }

    // 
    Byte newValue = mEditValue;
    bool stayInEditMode = true;
    bool moveToNext = false;

    switch( Action )
    {
    case ValueEdit:
        newValue = input;
        mEditValue = newValue^255; // force update
        mEditModeByInsert = true;
        mInsertedDigitsCount = 1;
        break;
    case ValueBackspace:
        if( mInsertedDigitsCount > 0 )
        {
            if( newValue > 0 )
                valueCodec->removeLastDigit( &newValue );
            --mInsertedDigitsCount;
        }
        break;
    case EnterValue:
        mEditValue ^= 255; // force update
        break;
    case IncValue:
        if( newValue < 255 )
        {
            ++newValue;
            mInsertedDigitsCount = valueCodec->encodingWidth();
        }
        break;
    case DecValue:
        if( newValue > 0 )
        {
            --newValue;
            mInsertedDigitsCount = valueCodec->encodingWidth();
        }
        break;
    case ValueAppend:
        if( valueCodec->appendDigit(&newValue,input) )
        {
            ++mInsertedDigitsCount;
            if( mEditModeByInsert &&
                (newValue >= valueCodec->digitsFilledLimit() || mInsertedDigitsCount == valueCodec->encodingWidth()) )
            {
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
    if( newValue != mEditValue )
    {
        // sync value
        mEditValue = newValue;
        valueCodec->encode( mValueString, 0, mEditValue );
        mView->byteArrayModel()->replace( mCursor->index(), 1, &mEditValue, 1 );
    }

//     mView->updateCursors();

    if( !stayInEditMode )
    {
        mView->pauseCursor();
        finishEdit();

        if( moveToNext )
            mCursor->gotoNextByte();

        mView->unpauseCursor();
        if( moveToNext )
            emit mView->cursorPositionChanged( mCursor->realIndex() );
    }
}

KValueEditor::~KValueEditor() {}

}
