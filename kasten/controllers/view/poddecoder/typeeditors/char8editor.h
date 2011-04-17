/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    Copyright 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#ifndef CHAR8EDITOR_H
#define CHAR8EDITOR_H

// lib
#include "../types/char8.h"
// KDE
#include <KLineEdit>

namespace Okteta {
class CharCodec;
}

class Char8Editor : public KLineEdit
{
  Q_OBJECT
  Q_PROPERTY( Char8 data READ data WRITE setData USER true )

  public:
    Char8Editor( Okteta::CharCodec* charCodec, QWidget* parent );

    virtual ~Char8Editor();

  public:
    void setData( Char8 data );
    Char8 data() const;
};

#endif
