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

#ifndef OKTETA_KNAVIGATOR_H
#define OKTETA_KNAVIGATOR_H


// lib
#include "kcontroller.h"


namespace Okteta
{
class AbstractByteArrayView;


class KNavigator : public KController
{
  protected:
    enum KMoveAction { MoveBackward, MoveWordBackward, MoveForward, MoveWordForward,
                       MoveUp, MovePgUp, MoveDown, MovePgDown,
                       MoveLineStart, MoveHome, MoveLineEnd, MoveEnd };
  public:
    KNavigator( AbstractByteArrayView* view, KController* parent );

  public: // KController API
    virtual bool handleKeyPress( QKeyEvent* keyEvent );

  protected:
    /** moves the cursor according to the action, handles all drawing */
    void moveCursor( KMoveAction Action, bool Select );

  protected:
    AbstractByteArrayView* mView;
};

}

#endif
