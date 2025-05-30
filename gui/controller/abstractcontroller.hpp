/*
    This file is part of the Okteta Gui library, made within the KDE community.

    SPDX-FileCopyrightText: 2004, 2008, 2025 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef OKTETA_ABSTRACTCONTROLLER_HPP
#define OKTETA_ABSTRACTCONTROLLER_HPP

class QKeyEvent;

namespace Okteta {

class AbstractController
{
protected:
    explicit AbstractController(AbstractController* parent);

public:
    AbstractController(const AbstractController&) = delete;
    AbstractController(AbstractController&&) = delete;

    virtual ~AbstractController();

    AbstractController& operator=(const AbstractController&) = delete;
    AbstractController& operator=(AbstractController&&) = delete;

public: // API to be implemented
    virtual void handleShortcutOverrideEvent(QKeyEvent* keyEvent) const;
    [[nodiscard]]
    virtual bool handleKeyPress(QKeyEvent* keyEvent);

protected:
    AbstractController* const mParent;
};

}

#endif
