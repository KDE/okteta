/*
    This file is part of the Kasten Framework, made within the KDE community.

    Copyright 2011 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "singleviewarea_p.hpp"

// lib
#include "toolinlineviewwidget.hpp"
#include <abstracttoolinlineview.hpp>
// Kasten core
#include <Kasten/AbstractDocument>

namespace Kasten {

SingleViewAreaPrivate::SingleViewAreaPrivate(SingleViewArea* parent)
    : AbstractViewAreaPrivate(parent)
{
}

SingleViewAreaPrivate::~SingleViewAreaPrivate()
{
    delete mViewAreaBox;
}

void SingleViewAreaPrivate::init()
{
//     Q_Q( SingleViewArea );

    mViewAreaBox = new ViewAreaBox(nullptr);   // TODO: atm cannot replace this

//     TODO: get drag/drop events
//     q->connect( mTabWidget, SIGNAL(testCanDecode(const QDragMoveEvent*,bool&)),
//                 SLOT(onDragMoveEvent(const QDragMoveEvent*,bool&)) );
//     q->connect( mTabWidget, SIGNAL(receivedDropEvent(QDropEvent*)),
//                 SLOT(onDropEvent(QDropEvent*)) );
}

void SingleViewAreaPrivate::setCurrentToolInlineView(AbstractToolInlineView* view)
{
    if (mCurrentToolInlineView != view) {
        mCurrentToolInlineView = view;
        QWidget* toolInlineViewWidget = mCurrentToolInlineView ? mCurrentToolInlineView->widget() : nullptr;
        mViewAreaBox->setBottomToolWidget(toolInlineViewWidget);
    }

    if (mCurrentToolInlineView) {
        mViewAreaBox->showBottomToolWidget();
    }
}

void SingleViewAreaPrivate::setView(AbstractView* view)
{
    mCurrentView = view;
    mViewAreaBox->setCentralWidget(view ? view->widget() : nullptr);
}

}
