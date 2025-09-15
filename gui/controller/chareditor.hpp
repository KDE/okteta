/*
    SPDX-FileCopyrightText: 2004, 2008, 2025 Friedrich W. H. Kossebau <kossebau@kde.org>

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
    CharEditor(AbstractByteArrayViewPrivate* view, AbstractController* parent);
    ~CharEditor() override;

public: // AbstractController API
    void handleShortcutOverrideEvent(QKeyEvent* keyEvent) const override;
    [[nodiscard]]
    bool handleKeyPress(QKeyEvent* keyEvent) override;
};

}

#endif
