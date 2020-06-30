/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2011 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
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
