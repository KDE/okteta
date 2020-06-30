/*
    This file is part of the Okteta Gui library, made within the KDE community.

    SPDX-FileCopyrightText: 2008-2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef OKTETA_ABSTRACTMOUSECONTROLLER_HPP
#define OKTETA_ABSTRACTMOUSECONTROLLER_HPP

class QMouseEvent;

namespace Okteta {

class AbstractByteArrayView;

class AbstractMouseController
{
protected:
    AbstractMouseController(AbstractByteArrayView* view, AbstractMouseController* parent);

public:
    AbstractMouseController() = delete;
    virtual ~AbstractMouseController();

public: // API to be implemented
    virtual bool handleMousePressEvent(QMouseEvent* mouseEvent);
    virtual bool handleMouseMoveEvent(QMouseEvent* mouseEvent);
    virtual bool handleMouseReleaseEvent(QMouseEvent* mouseEvent);
    virtual bool handleMouseDoubleClickEvent(QMouseEvent* mouseEvent);

protected:
    AbstractMouseController* mParent;
    AbstractByteArrayView* mView;
};

}

#endif
