/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "abstractlinepopup.hpp"
#include "abstractlinepopup_p.hpp"

// Qt
#include <QEvent>

namespace Kasten {

AbstractLinePopup::AbstractLinePopup(QWidget* parent)
    : QWidget(parent, Qt::Popup)
    , d_ptr(new AbstractLinePopupPrivate(this))
{
    Q_D(AbstractLinePopup);

    d->init();
}

AbstractLinePopup::~AbstractLinePopup() = default;

void AbstractLinePopup::setPosition(const QPoint& globalPosition)
{
    Q_D(AbstractLinePopup);

    d->setPosition(globalPosition);
}

void AbstractLinePopup::setIcon(const QIcon& icon)
{
    Q_D(AbstractLinePopup);

    d->setIcon(icon);
}

void AbstractLinePopup::setWidget(QWidget* widget)
{
    Q_D(AbstractLinePopup);

    d->setWidget(widget);
}

void AbstractLinePopup::setResult(int result)
{
    Q_D(AbstractLinePopup);

    d->setResult(result);
}

void AbstractLinePopup::setVisible(bool visible)
{
    Q_D(AbstractLinePopup);

    d->setVisible(visible);
}

int AbstractLinePopup::exec()
{
    Q_D(AbstractLinePopup);

    return d->exec();
}

void AbstractLinePopup::open()
{
    Q_D(AbstractLinePopup);

    d->open();
}

int AbstractLinePopup::result() const
{
    Q_D(const AbstractLinePopup);

    return d->result();
}


void AbstractLinePopup::accept()
{
    Q_D(AbstractLinePopup);

    d->accept();
}

void AbstractLinePopup::reject()
{
    Q_D(AbstractLinePopup);

    d->reject();
}

bool AbstractLinePopup::eventFilter(QObject* object, QEvent* event)
{
    Q_D(AbstractLinePopup);

    switch (event->type())
    {
    case QEvent::WindowDeactivate:
        close();
        break;
    case QEvent::MouseButtonPress:
    case QEvent::Wheel:
        if (!d->isOrContainsObject(object)) {
            close();
        }
        break;
    default:
        break;
    }

    return false;
}

}
