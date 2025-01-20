/*
    This file is part of the Okteta Gui library, made within the KDE community.

    SPDX-FileCopyrightText: 2004, 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef OKTETA_ABSTRACTEDITOR_HPP
#define OKTETA_ABSTRACTEDITOR_HPP

// lib
#include "abstractcontroller.hpp"

namespace Okteta {
class AbstractByteArrayViewPrivate;

class AbstractEditor : public AbstractController
{
protected:
    enum EditAction
    {
        CharDelete,
        WordDelete,
        CharBackspace,
        WordBackspace
    };

protected:
    AbstractEditor(AbstractByteArrayViewPrivate* view, AbstractController* parent);

public:
    ~AbstractEditor() override;

public: // AbstractController API
    [[nodiscard]]
    bool handleKeyPress(QKeyEvent* keyEvent) override;

protected:
    /** executes edit action \p action. This is normally called by a key event handler. */
    void doEditAction(EditAction action);

protected:
    AbstractByteArrayViewPrivate* const mView;
};

}

#endif
