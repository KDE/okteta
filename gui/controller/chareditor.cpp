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

#include "chareditor.hpp"

// lib
#include <abstractbytearrayview.hpp>
// Okteta core
#include <Okteta/CharCodec>
// Qt
#include <QKeyEvent>

namespace Okteta {

CharEditor::CharEditor(ByteArrayTableCursor* cursor, AbstractByteArrayView* view, AbstractController* parent)
    : AbstractEditor(cursor, view, parent)
{
}

CharEditor::~CharEditor() = default;

bool CharEditor::handleKeyPress(QKeyEvent* keyEvent)
{
    bool keyUsed = false;

    const QString text = keyEvent->text();

    // some input that should be inserted?
    if (text.length() > 0
        && !(keyEvent->modifiers() & (Qt::CTRL | Qt::ALT | Qt::META))) {

        const QChar enteredChar = text.at(0);
        if (enteredChar.isPrint()) {
            Byte byte;
            if (mView->charCodec()->encode(&byte, enteredChar)) {
                QByteArray data(1, byte);
                mView->insert(data);

                keyUsed = true;
            }
        }
    }

    return keyUsed ? true : AbstractEditor::handleKeyPress(keyEvent);
}

}
