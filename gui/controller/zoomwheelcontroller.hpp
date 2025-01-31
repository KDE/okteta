/*
    This file is part of the Okteta Gui library, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef OKTETA_ZOOMWHEELCONTROLLER_HPP
#define OKTETA_ZOOMWHEELCONTROLLER_HPP

// lib
#include "abstractwheelcontroller.hpp"

namespace Okteta {
class AbstractByteArrayViewPrivate;

class ZoomWheelController : public AbstractWheelController
{
public:
    ZoomWheelController(AbstractByteArrayViewPrivate* view, AbstractWheelController* parent);
    ~ZoomWheelController() override;

public: // AbstractWheelController API
    [[nodiscard]]
    bool handleWheelEvent(QWheelEvent* wheelEvent) override;

private:
    AbstractByteArrayViewPrivate* const mView;
};

}

#endif
