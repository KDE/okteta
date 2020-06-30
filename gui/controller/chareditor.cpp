/*
    This file is part of the Okteta Gui library, made within the KDE community.

    SPDX-FileCopyrightText: 2004, 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
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
