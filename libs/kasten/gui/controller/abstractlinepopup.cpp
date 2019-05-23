/*
    This file is part of the Kasten Framework, made within the KDE community.

    Copyright 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) version 3, or any
    later version accepted by the membership of KDE e.V. (or its
    successor approved by the membership of KDE e.V.), which shall
    act as a proxy defined in Section 6 of version 3 of the license.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library. If not, see <http://www.gnu.org/licenses/>.
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
