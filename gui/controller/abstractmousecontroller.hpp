/*
    This file is part of the Okteta Gui library, made within the KDE community.

    SPDX-FileCopyrightText: 2008-2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef OKTETA_ABSTRACTMOUSECONTROLLER_HPP
#define OKTETA_ABSTRACTMOUSECONTROLLER_HPP

class QMouseEvent;

namespace Okteta {

class AbstractByteArrayViewPrivate;

class AbstractMouseController
{
protected:
    AbstractMouseController(AbstractByteArrayViewPrivate* view, AbstractMouseController* parent);

public:
    AbstractMouseController(const AbstractMouseController&) = delete;

    virtual ~AbstractMouseController();

    AbstractMouseController& operator=(const AbstractMouseController&) = delete;

public: // API to be implemented
    [[nodiscard]]
    virtual bool handleMousePressEvent(QMouseEvent* mouseEvent);
    [[nodiscard]]
    virtual bool handleMouseMoveEvent(QMouseEvent* mouseEvent);
    [[nodiscard]]
    virtual bool handleMouseReleaseEvent(QMouseEvent* mouseEvent);
    [[nodiscard]]
    virtual bool handleMouseDoubleClickEvent(QMouseEvent* mouseEvent);

protected:
    AbstractMouseController* mParent;
    AbstractByteArrayViewPrivate* const mView;
};

}

#endif
