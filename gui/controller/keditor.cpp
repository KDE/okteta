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

#include "keditor.h"

// lib
#include <bytearraytablelayout.h>
#include <bytearraytablecursor.h>
#include <abstractbytearrayview.h>
// Okteta core
#include <abstractbytearraymodel.h>
#include <wordbytearrayservice.h>
// Qt
#include <QtGui/QKeyEvent>


namespace Okteta
{

KEditor::KEditor( ByteArrayTableCursor* cursor, AbstractByteArrayView* view, KController *parent )
  : KController( parent ),
    mCursor( cursor ),
    mView( view )
{
}


bool KEditor::handleKeyPress( QKeyEvent* keyEvent )
{
    const bool shiftPressed =   keyEvent->modifiers() & Qt::SHIFT;
    const bool controlPressed = keyEvent->modifiers() & Qt::CTRL;
    const bool altPressed =     keyEvent->modifiers() & Qt::ALT;

    bool keyUsed = true;
    // we only care for cursor keys and the like, won't hardcode any other keys
    // we also don't check whether the commands are allowed
    // as the commands are also available as API so the check has to be done
    // in each command anyway
    switch( keyEvent->key() )
    {
    case Qt::Key_Delete:
        if( shiftPressed )
            mView->cut();
        else if( mView->hasSelectedData() )
            mView->removeSelectedData();
        else
            doEditAction( controlPressed ? WordDelete : CharDelete );
        break;
    case Qt::Key_Insert:
        if( shiftPressed )
            mView->paste();
        else if( controlPressed )
            mView->copy();
        else
            mView->setOverwriteMode( !mView->isOverwriteMode() );
        break;
    case Qt::Key_Backspace:
        if( altPressed )
            break;
        else if( mView->hasSelectedData() )
        {
            mView->removeSelectedData();
            break;
        }

        doEditAction( controlPressed ? WordBackspace : CharBackspace );
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

    return keyUsed ? true : KController::handleKeyPress(keyEvent);
}



void KEditor::doEditAction( KEditAction action )
{
    Okteta::AbstractByteArrayModel* byteArrayModel = mView->byteArrayModel();
    switch( action )
    {
    case CharDelete:
        if( !mView->isOverwriteMode() )
        {
            const Address index = mCursor->realIndex();
            if( index < mView->layout()->length() )
                byteArrayModel->remove( AddressRange::fromWidth(index,1) );
        }
        break;
    case WordDelete: // kills data until the start of the next word
        if( !mView->isOverwriteMode() )
        {
            const Address index = mCursor->realIndex();
            if( index < mView->layout()->length() )
            {
                const WordByteArrayService WBS( byteArrayModel, mView->charCodec() );
                const Address end = WBS.indexOfBeforeNextWordStart( index );
                byteArrayModel->remove( AddressRange(index,end) );
            }
        }
        break;
    case CharBackspace:
        if( mView->isOverwriteMode() )
        {
            mView->pauseCursor();
            mCursor->gotoPreviousByte();
            mView->ensureCursorVisible();
            mView->unpauseCursor();
        }
        else
        {
            const Address deleteIndex = mCursor->realIndex() - 1;
            if( deleteIndex >= 0 )
                byteArrayModel->remove( AddressRange::fromWidth(deleteIndex,1) );
        }
        break;
    case WordBackspace:
        {
            const int leftIndex = mCursor->realIndex() - 1;
            if( leftIndex >= 0 )
            {
                const WordByteArrayService WBS( byteArrayModel, mView->charCodec() );
                const Address wordStart = WBS.indexOfPreviousWordStart( leftIndex );
                if( !mView->isOverwriteMode() )
                    byteArrayModel->remove( AddressRange(wordStart,leftIndex) );
            }
        }
    }
}

KEditor::~KEditor() {}

}
