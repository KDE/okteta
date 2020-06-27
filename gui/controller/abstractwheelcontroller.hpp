/*
    This file is part of the Okteta Gui library, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef OKTETA_ABSTRACTWHEELCONTROLLER_HPP
#define OKTETA_ABSTRACTWHEELCONTROLLER_HPP

class QWheelEvent;

namespace Okteta {

class AbstractWheelController
{
protected:
    explicit AbstractWheelController(AbstractWheelController* parent);

public:
    AbstractWheelController() = delete;
    virtual ~AbstractWheelController();

public: // API to be implemented
    virtual bool handleWheelEvent(QWheelEvent* wheelEvent);

protected:
    AbstractWheelController* mParent;
};

}

#endif
