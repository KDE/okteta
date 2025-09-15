/*
    SPDX-FileCopyrightText: 2004, 2008, 2025 Friedrich W. H. Kossebau <kossebau@kde.org>

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
    void handleShortcutOverrideEvent(QKeyEvent* keyEvent) const override;
    [[nodiscard]]
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
