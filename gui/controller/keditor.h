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

#ifndef OKTETA_KEDITOR_H
#define OKTETA_KEDITOR_H

// lib
#include "kcontroller.h"


namespace Okteta
{
class AbstractByteArrayView;
class ByteArrayTableCursor;


class KEditor : public KController
{
  protected:
    enum KEditAction { CharDelete, WordDelete, CharBackspace, WordBackspace };

  protected:
    KEditor( ByteArrayTableCursor* cursor, AbstractByteArrayView* view, KController* parent );
  public:
    virtual ~KEditor();

  public: // KController API
    virtual bool handleKeyPress( QKeyEvent* keyEvent );

  protected:
    /** executes keyboard Action \a Action. This is normally called by a key event handler. */
    void doEditAction( KEditAction Action );

  protected:
    ByteArrayTableCursor* mCursor;
    AbstractByteArrayView* mView;
};

}

#endif
