/*
    This file is part of the Okteta Kasten module, part of the KDE project.

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

#include "float32editor.h"

// KDE
#include <KDoubleValidator>
// C++
#include <limits>


Float32Editor::Float32Editor( QWidget* parent )
  : QLineEdit( parent )
{
    const float floatMax = std::numeric_limits<float>::max();
    const float floatMin = std::numeric_limits<float>::min();

    KDoubleValidator* validator = new KDoubleValidator( this );
    validator->setNotation( QDoubleValidator::ScientificNotation );
    validator->setRange( floatMin, floatMax );

    setValidator( validator );
}

void Float32Editor::setData( Float32 data )
{
    setText( data.toString() );
}

Float32 Float32Editor::data() const
{
//     interpretText();
    return text().toFloat();
}

Float32Editor::~Float32Editor() {}
