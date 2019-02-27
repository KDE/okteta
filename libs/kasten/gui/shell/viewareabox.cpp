/*
    This file is part of the Kasten Framework, made within the KDE community.

    Copyright 2009,2011 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "viewareabox.hpp"

// lib
#include "toolinlineviewwidget.hpp"
// Qt
#include <QVBoxLayout>
#include <QShortcut>

namespace Kasten {

ViewAreaBox::ViewAreaBox(QWidget* centralWidget, QWidget* parent)
    : QWidget(parent)
    , mCentralWidget(centralWidget)
{
    setFocusProxy(mCentralWidget);

    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    if (mCentralWidget) {
        layout->addWidget(mCentralWidget);
    }

    mEscapeShortcut = new QShortcut(Qt::Key_Escape, this);
    mEscapeShortcut->setContext(Qt::WidgetWithChildrenShortcut);
    mEscapeShortcut->setEnabled(false);
    connect(mEscapeShortcut, &QShortcut::activated, this, &ViewAreaBox::onDone);   // TODO: better use onCancelled
}

ViewAreaBox::~ViewAreaBox()
{
    if (mCentralWidget) {
        mCentralWidget->setParent(nullptr);
    }
}

QWidget* ViewAreaBox::centralWidget() const { return mCentralWidget; }
QWidget* ViewAreaBox::bottomToolWidget()  const { return mBottomToolWidget; }

void ViewAreaBox::setCentralWidget(QWidget* centralWidget)
{
    if (mCentralWidget == centralWidget) {
        return;
    }

    auto* layout = static_cast<QVBoxLayout*>(this->layout());
    const bool centralWidgetIsFocusProxy =
        mCentralWidget ? (focusProxy() == mCentralWidget) : false;
    // TODO: works if focus is on childwidget?
    const bool centralWidgetHasFocus =
        mCentralWidget ? mCentralWidget->hasFocus() : false;
    if (mCentralWidget) {
        layout->removeWidget(mCentralWidget);
    }

    mCentralWidget = centralWidget;

    if (mCentralWidget) {
        layout->insertWidget(0, mCentralWidget);
        mCentralWidget->show(); // TODO: needed?
        if (centralWidgetHasFocus) {
            mCentralWidget->setFocus();
        }
        if (centralWidgetIsFocusProxy) {
            setFocusProxy(mCentralWidget);
        }
    }
}

void ViewAreaBox::setBottomToolWidget(QWidget* bottomToolWidget)
{
    if (mBottomToolWidget == bottomToolWidget) {
        return;
    }

    auto* layout = static_cast<QVBoxLayout*>(this->layout());

    if (mToolInlineViewWidget) {
        layout->removeWidget(mToolInlineViewWidget);
        delete mToolInlineViewWidget;
        mToolInlineViewWidget = nullptr;
        setFocusProxy(mCentralWidget);
    }

    mBottomToolWidget = bottomToolWidget;

    if (mBottomToolWidget) {
        mToolInlineViewWidget = new ToolInlineViewWidget(mBottomToolWidget);

        connect(mToolInlineViewWidget, &ToolInlineViewWidget::done, this, &ViewAreaBox::onDone);
        layout->addWidget(mToolInlineViewWidget);
    }
}

void ViewAreaBox::showBottomToolWidget()
{
    if (!mToolInlineViewWidget) {
        return;
    }

    setFocusProxy(mToolInlineViewWidget);
    mToolInlineViewWidget->show();
    mBottomToolWidget->setFocus();

    mEscapeShortcut->setEnabled(true);
}

void ViewAreaBox::onDone()
{
    if (!mToolInlineViewWidget) {
        return;
    }

    setFocusProxy(mCentralWidget);
    mToolInlineViewWidget->hide();
    mEscapeShortcut->setEnabled(false);
}

}
