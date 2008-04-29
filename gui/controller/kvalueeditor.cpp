/*
    This file is part of the Okteta Gui library, part of the KDE project.

    Copyright 2004 Friedrich W. H. Kossebau <kossebau@kde.org>

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
#include "valuebytearraycolumnrenderer.h"
#include "kdataranges.h"
#include "kdatacursor.h"
#include "kbytearrayview.h"
// Okteta core
#include <kabstractbytearraymodel.h>
// Qt
#include <QtGui/QKeyEvent>


namespace KHEUI {

KValueEditor::KValueEditor( ValueByteArrayColumnRenderer *valueColumn, KDataCursor *dataCursor, KByteArrayView* view, KController *parent )
  : KEditor( dataCursor, view, parent ),
  mValueColumn( valueColumn ),
  mInEditMode( false ),
  mEditModeByInsert( false )
{
}


bool KValueEditor::handleKeyPress( QKeyEvent *keyEvent )
{
    bool keyUsed = true;

    // TODO: for now we don't touch it if there are selections
    if( !mView->mDataRanges->hasSelection() )
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
                doValueEditAction( CancelValue );
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

                if( mInEditMode )
                    doValueEditAction( ValueAppend, input );
                else
                {
                    unsigned char InputValue = 0;
                    const KHECore::ValueCodec *valueCodec = mValueColumn->valueCodec();
                    // valid digit?
                    if( valueCodec->appendDigit(&InputValue,input) )
                    {
                        if( mView->isOverwriteMode() )
                            doValueEditAction( ValueEdit, InputValue );
                        else
                        {
                            const int index = mDataCursor->realIndex();
                            if( mView->mByteArrayModel->insert(index,(char*)&InputValue,1) > 0 )
                            {
                                mInEditMode = true;
                                mEditModeByInsert = true;
                                mOldValue = mEditValue = InputValue;
                                valueCodec->encode( mByteBuffer, 0, mEditValue );

                                mDataCursor->gotoRealIndex();
                                mView->ensureCursorVisible();
                                mView->updateCursors();
                            }
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
    // we are not yet in edit mode?
    if( !mInEditMode )
    {
        const int validIndex = mDataCursor->validIndex();
        // no valid cursor position?
        if( validIndex == -1 || (!mView->isOverwriteMode() && input == -1) || mDataCursor->isBehind() )
            return;

        mInEditMode = true;
        mEditModeByInsert = false; // default, to be overwritten if so

        // save old value
        mOldValue = mEditValue = (unsigned char)mView->mByteArrayModel->datum( validIndex );
    }

    const KHECore::ValueCodec *valueCodec = mValueColumn->valueCodec();
    // 
    unsigned char newValue = mEditValue;
    bool stayInEditMode = true;
    bool moveToNext = false;

    switch( Action )
    {
    case ValueEdit:
        newValue = input;
        mEditValue = newValue^255; // force update
        mEditModeByInsert = true;
        break;
    case ValueBackspace:
        if( newValue > 0 )
            valueCodec->removeLastDigit( &newValue );
        break;
    case EnterValue:
        mEditValue ^= 255; // force update
        break;
    case IncValue:
        if( newValue < 255 )
            ++newValue;
        break;
    case DecValue:
        if( newValue > 0 )
            --newValue;
        break;
    case ValueAppend:
        if( valueCodec->appendDigit(&newValue,input) )
            if( mEditModeByInsert && newValue >= valueCodec->digitsFilledLimit() )
            {
                stayInEditMode = false;
                moveToNext = true;
            }
        break;
    case LeaveValue:
        stayInEditMode = false;
        moveToNext = mEditModeByInsert;
        break;
    case CancelValue:
        newValue = mOldValue;
        stayInEditMode = false;
        break;
    }

    // change happened?
    if( newValue != mEditValue )
    {
        // sync value
        mEditValue = newValue;
        valueCodec->encode( mByteBuffer, 0, mEditValue );
        mView->mByteArrayModel->replace( mDataCursor->index(), 1, (char*)&mEditValue, 1 );
    }

    mView->updateCursors();

    if( !stayInEditMode )
    {
        mView->pauseCursor();
        mInEditMode = false;
        if( moveToNext )
            mDataCursor->gotoNextByte();
        mView->unpauseCursor();
    }
}

KValueEditor::~KValueEditor() {}

}
