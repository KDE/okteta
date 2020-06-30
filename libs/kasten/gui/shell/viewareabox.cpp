/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2009, 2011 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
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
