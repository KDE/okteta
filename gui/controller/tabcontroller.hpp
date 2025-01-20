/*
    This file is part of the Okteta Gui library, made within the KDE community.

    SPDX-FileCopyrightText: 2004, 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef OKTETA_TABCONTROLLER_HPP
#define OKTETA_TABCONTROLLER_HPP

// lib
#include "abstractcontroller.hpp"

namespace Okteta {
class AbstractByteArrayViewPrivate;

class TabController : public AbstractController
{
public:
    TabController(AbstractByteArrayViewPrivate* view, AbstractController* parent);

public: // AbstractController API
    bool handleKeyPress(QKeyEvent* keyEvent) override;

public:
    [[nodiscard]]
    bool tabChangesFocus() const;
    /// default is true
    void setTabChangesFocus(bool tabChangesFocus);

private:
    AbstractByteArrayViewPrivate* const mView;
    /** flag if tab key should be ignored */
    bool mTabChangesFocus : 1;
};

inline bool TabController::tabChangesFocus()    const { return mTabChangesFocus; }
inline void TabController::setTabChangesFocus(bool tabChangesFocus) { mTabChangesFocus = tabChangesFocus; }

}

#endif
