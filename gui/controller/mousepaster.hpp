/*
    This file is part of the Okteta Gui library, made within the KDE community.

    SPDX-FileCopyrightText: 2008-2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef OKTETA_MOUSEPASTER_HPP
#define OKTETA_MOUSEPASTER_HPP

// lib
#include "abstractmousecontroller.hpp"

namespace Okteta {

class MousePaster : public AbstractMouseController
{
public:
    MousePaster(AbstractByteArrayViewPrivate* view, AbstractMouseController* parent);

    ~MousePaster() override;

public: // AbstractMouseController API
    [[nodiscard]]
    bool handleMousePressEvent(QMouseEvent* mouseEvent) override;
    [[nodiscard]]
    bool handleMouseReleaseEvent(QMouseEvent* mouseEvent) override;

private:
};

}

#endif
