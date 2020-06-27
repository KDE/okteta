/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "abstractlinepopup_p.hpp"

// Qt
#include <QIcon>
#include <QLayout>
#include <QLabel>
#include <QApplication>

namespace Kasten {

void AbstractLinePopupPrivate::init()
{
    // TODO: what kind of border should there be? like a QMenu?

    p->setAttribute(Qt::WA_X11NetWmWindowTypePopupMenu);
//     setAttribute( Qt::WA_DeleteOnClose );
    p->setMouseTracking(true);

    mBaseLayout = new QHBoxLayout(p);
    mBaseLayout->setContentsMargins(0, 0, 0, 0);
    mBaseLayout->setSpacing(0);

    mIconLabel = new QLabel(p);
    mBaseLayout->addWidget(mIconLabel);

    qApp->installEventFilter(p);
}

void AbstractLinePopupPrivate::setIcon(const QIcon& icon)
{
    mIconLabel->setPixmap(icon.pixmap(22));   // TODO: correct call, using KDE size
}

void AbstractLinePopupPrivate::setWidget(QWidget* widget)
{
    mWidget = widget;
    mBaseLayout->addWidget(widget, 10);
}

void AbstractLinePopupPrivate::setPosition(QPoint globalPosition)
{
    p->move(globalPosition.x(), globalPosition.y() - p->height());
}

void AbstractLinePopupPrivate::setVisible(bool visible)
{
    p->QWidget::setVisible(visible);
    if (!visible && mEventLoop) {
        mEventLoop->exit();
    }
}

int AbstractLinePopupPrivate::exec()
{
    open();

    QEventLoop eventLoop;
    mEventLoop = &eventLoop;
    eventLoop.exec();
    mEventLoop = nullptr;

    emit p->finished(mResult);

    return mResult;
}

void AbstractLinePopupPrivate::open()
{
    if (mWidget) {
        mWidget->setFocus();
    }
    p->show();
}

void AbstractLinePopupPrivate::accept()
{
    done(AbstractLinePopup::Accepted);
}

void AbstractLinePopupPrivate::reject()
{
    done(AbstractLinePopup::Rejected);
}

void AbstractLinePopupPrivate::done(int result)
{
    setResult(result);

    p->hide();

    emit p->finished(result);

    p->close();
}

}
