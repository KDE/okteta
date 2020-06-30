/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "statusbarlayout.hpp"

// lib
#include <kastengui.hpp>
// Qt
#include <QWidgetItem>
#include <QWidget>
#include <QStyle>

namespace Kasten {

StatusBarLayout::StatusBarLayout(QWidget* parent)
    : QLayout(parent)
    , mIsDirty(true)
    , mIsEmpty(true)
{
    setContentsMargins(0, 0, 0, 0);
}

StatusBarLayout::~StatusBarLayout()
{
//     while( !mWidgetList.isEmpty() )
//     {
//         QWidgetItem *item = mWidgetList.takeFirst();
//         delete item;
//     }
}

int StatusBarLayout::count() const { return mWidgetList.count(); }

bool StatusBarLayout::isEmpty() const
{
    if (mIsDirty) {
        updateLayoutStructs();
    }

    return mIsEmpty;
}

QLayoutItem* StatusBarLayout::itemAt(int index) const
{
    if (index < 0 || mWidgetList.count() <= index) {
        return nullptr;
    }

    return mWidgetList.at(index);
}

int StatusBarLayout::indexOf(QWidget* widget) const
{
    int result = -1;

    for (int i = 0; i < mWidgetList.count(); ++i) {
        if (mWidgetList.at(i)->widget() == widget) {
            result = i;
            break;
        }
    }

    return result;
}

QSize StatusBarLayout::sizeHint() const
{
    updateLayoutStructs();

    return mSizeHint;
}

QSize StatusBarLayout::minimumSize() const
{
    updateLayoutStructs();

    return {0, mSizeHint.height()};
}

void StatusBarLayout::addItem(QLayoutItem* item)
{
    Q_UNUSED(item)
    qCWarning(LOG_KASTEN_GUI) << "not implemented!  Please use addWidget() instead";
    return;
}

QLayoutItem* StatusBarLayout::takeAt(int index)
{
    if (index < 0 || mWidgetList.count() <= index) {
        return nullptr;
    }

    QWidgetItem* item = mWidgetList.takeAt(index);

    // TODO: any need to delete or reparent the widget?

    invalidate();
    return item;
}

void StatusBarLayout::invalidate()
{
    mIsDirty = true;
    QLayout::invalidate();
}

Qt::Orientations StatusBarLayout::expandingDirections() const { return {}; }

void StatusBarLayout::addWidget(QWidget* widget)
{
    if (widget) {
        // TODO: ideally also called, but results in some endless loop, yet to be investigated
        // addChildWidget(widget);
        mWidgetList.append(new QWidgetItem(widget));
        invalidate();
    }
}

#if 0
void StatusBarLayout::updateMarginAndSpacing()
{
    Statusbar* statusBar = qobject_cast<Statusbar*>(parentWidget());
    if (!statusBar) {
        return;
    }

    QStyle* style = statusBar->style();
    QStyleOptionToolBar opt;
    statusBar->initStyleOption(&opt);
    setMargin(style->pixelMetric(QStyle::PM_ToolBarItemMargin, &opt, statusBar)
              + style->pixelMetric(QStyle::PM_ToolBarFrameWidth, &opt, statusBar));
    setSpacing(style->pixelMetric(QStyle::PM_ToolBarItemSpacing, &opt, statusBar));
}
#endif

void StatusBarLayout::setGeometry(const QRect& _rect)
{
    QLayout::setGeometry(_rect);

    if (mIsDirty) {
        updateLayoutStructs();
    }
    const QPoint baseWidgetOffset = parentWidget()->pos();
    QRect rect(baseWidgetOffset, _rect.size());

    const int margin = 0;// this->margin();
    const int spacing = this->spacing();

    int availableWidth =        rect.size().width()  - 2 * margin;
    const int availableHeight = rect.size().height() - 2 * margin;

    int usedWidth = 0;
    int visibleCount = 0;
    int i;
    for (i = 0; i < mWidgetList.count(); ++i) {
        QWidgetItem* item = mWidgetList.at(i);
        QWidget* widget = item->widget();

        // TODO: is there really no way to get to the geometry data if a widget is hidden?
        if (widget->isHidden()) {
            widget->show();
        }

        const int itemWidth = item->sizeHint().width();
        const int itemSpacing = (visibleCount == 0) ? 0 : spacing;
        const int newUsedWidth = usedWidth + itemSpacing + itemWidth;
// qCDebug(LOG_KASTEN_GUI)<<widget<<<<availableWidth<<usedWidth<<itemWidth<<itemSpacing<<newUsedWidth;

        const bool isTooWide = (newUsedWidth > availableWidth);

        if (isTooWide) {
            break;
        }

        const QPoint pos(baseWidgetOffset.x() + margin + usedWidth + itemSpacing, baseWidgetOffset.y() + margin);
        const QSize size(itemWidth, availableHeight);
        QRect r(pos, size);

        r = QStyle::visualRect(parentWidget()->layoutDirection(), rect, r);

        item->setGeometry(r);

        usedWidth = newUsedWidth;

        ++visibleCount;
    }

    // hide the rest if needed
    for (; i < mWidgetList.count(); ++i) {
        QWidgetItem* item = mWidgetList.at(i);
        QWidget* widget = item->widget();

        if (!widget->isHidden()) {
            widget->hide();
        }
    }
}

void StatusBarLayout::updateLayoutStructs() const
{
    auto* that = const_cast<StatusBarLayout*>(this);

//     const int margin = this->margin();
    const int spacing = this->spacing();

    QSize sizeHint(0, 0);

    int visibleCount = 0;
    for (int i = 0; i < mWidgetList.count(); ++i) {
        QWidgetItem* item = mWidgetList.at(i);

        if (!item->isEmpty()) {
            const QSize itemSizeHint = item->sizeHint();

            sizeHint.rwidth() += (visibleCount == 0 ? 0 : spacing) + itemSizeHint.width();
            sizeHint.rheight() = qMax(sizeHint.height(), itemSizeHint.height());
            ++visibleCount;
        }
    }

//     sizeHint += QSize( 2*margin, 2*margin );

    that->mIsEmpty =         (visibleCount == 0);
    that->mSizeHint =        sizeHint;
    that->mIsDirty =         false;
}

}
