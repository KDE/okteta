/*
    This file is part of the Okteta Gui library, made within the KDE community.

    SPDX-FileCopyrightText: 2019, 2025 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef OKTETA_CLIPBOARDCONTROLLER_HPP
#define OKTETA_CLIPBOARDCONTROLLER_HPP

// lib
#include "abstractcontroller.hpp"

class QMenu;

namespace Okteta {

class AbstractByteArrayViewPrivate;

class ClipboardController : public AbstractController
{
public:
    ClipboardController(AbstractByteArrayViewPrivate* view, AbstractController* parent);

public: // AbstractController API
    void handleShortcutOverrideEvent(QKeyEvent* keyEvent) const override;
    [[nodiscard]]
    bool handleKeyPress(QKeyEvent* keyEvent) override;

public:
    int addContextMenuActions(QMenu* menu);

private:
    AbstractByteArrayViewPrivate* const mView;
};

}

#endif
