/*
    This file is part of the Okteta Gui library, made within the KDE community.

    SPDX-FileCopyrightText: 2004, 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef OKTETA_CHAREDITOR_HPP
#define OKTETA_CHAREDITOR_HPP

// lib
#include "abstracteditor.hpp"

namespace Okteta {

class CharEditor : public AbstractEditor
{
public:
    CharEditor(ByteArrayTableCursor* cursor, AbstractByteArrayView* view, AbstractController* parent);
    ~CharEditor() override;

public: // AbstractController API
    bool handleKeyPress(QKeyEvent* keyEvent) override;
};

}

#endif
