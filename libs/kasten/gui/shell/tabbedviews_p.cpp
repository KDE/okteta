/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2007, 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "tabbedviews_p.hpp"

// lib
#include "toolinlineviewwidget.hpp"
#include <abstracttoolinlineview.hpp>
// Kasten core
#include <Kasten/AbstractDocument>
// Qt
#include <QDragMoveEvent>
#include <QDropEvent>
#include <QApplication>
#include <QClipboard>

namespace Kasten {

TabbedViewsPrivate::TabbedViewsPrivate(TabbedViews* parent)
    : AbstractGroupedViewsPrivate(parent)
{
}

TabbedViewsPrivate::~TabbedViewsPrivate()
{
    delete mViewAreaBox;
}

void TabbedViewsPrivate::init()
{
    Q_Q(TabbedViews);

    mTabWidget = new TabWidget();

    mViewAreaBox = new ViewAreaBox(mTabWidget);

    QObject::connect(mTabWidget, &QTabWidget::tabCloseRequested,
                     q, [&](int index) { onTabCloseRequest(index); });
    QObject::connect(mTabWidget, &QTabWidget::currentChanged,
                     q, [&](int index) { onCurrentChanged(index); });

    QObject::connect(mTabWidget, &TabWidget::testCanDecode,
                     q, [&](const QDragMoveEvent* event, bool& accept) { onDragMoveEvent(event, accept); });
    QObject::connect(mTabWidget, &TabWidget::receivedDropEvent,
                     q, [&](QDropEvent* event) { onDropEvent(event); });
    QObject::connect(mTabWidget, &TabWidget::mouseMiddleClick,
                     q, [&]() { onMouseMiddleClick(); });
}

QVector<AbstractView*> TabbedViewsPrivate::viewList() const
{
    QVector<AbstractView*> result;

    const int count = mTabWidget->count();
    result.reserve(count);
    for (int i = 0; i < count; ++i) {
        const auto* viewBox = static_cast<const ViewBox*>(mTabWidget->widget(i));
        AbstractView* view = viewBox->view();
        result.append(view);
    }

    return result;
}

int TabbedViewsPrivate::indexOf(AbstractView* view) const
{
    int result = -1;

    const int tabCount = mTabWidget->count();
    for (int i = 0; i < tabCount; ++i) {
        const auto* viewBox = static_cast<const ViewBox*>(mTabWidget->widget(i));
        if (view == viewBox->view()) {
            result = i;
            break;
        }
    }

    return result;
}

void TabbedViewsPrivate::addViews(const QVector<AbstractView*>& views)
{
    Q_Q(TabbedViews);

    if (views.count() == 0) {
        return;
    }

    int insertIndex = mTabWidget->currentIndex() + 1;
    for (AbstractView* view : views) {
        QObject::connect(view, &AbstractModel::titleChanged,
                         q, [&](const QString& title) { onTitleChanged(title); });

        auto* viewBox = new ViewBox(view, mTabWidget);
        mTabWidget->insertTab(insertIndex, viewBox, view->title());
        ++insertIndex;
    }

    mTabWidget->setCurrentIndex(insertIndex - 1);

    // fix for Qt bug:
    if (mTabWidget->count() == 1) {
        // simulate signal reaction
        onCurrentChanged(0);
    }

    emit q->added(views);
}

void TabbedViewsPrivate::removeViews(const QVector<AbstractView*>& views)
{
    Q_Q(TabbedViews);

    int index = -1;

    // TODO: check if contained
    for (AbstractView* view : views) {
        view->disconnect(q);

        index = indexOf(view);
        if (index != -1) {
            auto* viewBox = static_cast<ViewBox*>(mTabWidget->widget(index));

            mTabWidget->removeTab(index);
            delete viewBox;
        }
    }

    // fix for Qt bug:
    const int currentIndex = mTabWidget->currentIndex();
    // last removed or no change in index (not bound to widget)?
    if (currentIndex == -1 || currentIndex == index) {
        // simulate signal reaction
        onCurrentChanged(currentIndex);
    }

    emit q->removing(views);
}

void TabbedViewsPrivate::setCurrentToolInlineView(AbstractToolInlineView* view)
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

void TabbedViewsPrivate::onCurrentChanged(int index)
{
    Q_Q(TabbedViews);

    const auto* viewBox = static_cast<const ViewBox*>(mTabWidget->widget(index));
    AbstractView* view = viewBox ? viewBox->view() : nullptr;

    if (view == mCurrentView) {
        return;
    }

    setCurrentToolInlineView(nullptr);

    if (mCurrentView) {
        mCurrentView->disconnect(q);
    }

    mCurrentView = view;

    if (view) {
        QObject::connect(view, &AbstractView::focusChanged,
                         q, [&](bool hasFocus) { onViewFocusChanged(hasFocus); });
        view->widget()->setFocus();
    }

    emit q->viewFocusChanged(view);
}

void TabbedViewsPrivate::onTabCloseRequest(int tabIndex)
{
    Q_Q(TabbedViews);

    const QWidget* widget = mTabWidget->widget(tabIndex);
    const auto* viewBox = static_cast<const ViewBox*>(widget);
    AbstractView* view = viewBox->view();

    const QVector<Kasten::AbstractView*> views { view };
    emit q->closeRequest(views);
}

void TabbedViewsPrivate::onTitleChanged(const QString& newTitle)
{
    Q_Q(TabbedViews);

    auto* view = qobject_cast<AbstractView*>(q->sender());
    if (view) {
        const int index = indexOf(view);
        if (index != -1) {
            mTabWidget->setTabText(index, newTitle);
        }
    }
}

#if 0
void TabbedViewsPrivate::onModifiedChanged(AbstractDocument::SyncStates newStates)
{
    Q_UNUSED(newStates)
    AbstractView * view = qobject_cast<AbstractView*>(sender());
    if (view) {
        const int index = indexOf(view);
        if (index != -1) {
//             mViewsTab->setIcon( index, newTitle ); //modificationSymbol
            if (index == mViewsTab->currentIndex()) {
                setCaption(view->title(), view->document()->hasLocalChanges());
            }
        }
    }

}
#endif

// TODO: could be just a signal forwarder
void TabbedViewsPrivate::onViewFocusChanged(bool hasFocus)
{
    Q_Q(TabbedViews);

//     AbstractView* view = qobject_cast<AbstractView *>( q->sender() );
// qCDebug(LOG_KASTEN_GUI)<<view<<view->title()<<hasFocus;

    emit q->focusChanged(hasFocus);
}

void TabbedViewsPrivate::onMouseMiddleClick()
{
    Q_Q(TabbedViews);

    const QMimeData* mimeData = QApplication::clipboard()->mimeData(QClipboard::Selection);

    emit q->dataDropped(mimeData);
}

void TabbedViewsPrivate::onDragMoveEvent(const QDragMoveEvent* event, bool& accepted)
{
    Q_Q(TabbedViews);

    const QMimeData* mimeData = event->mimeData();

    emit q->dataOffered(mimeData, accepted);
}

void TabbedViewsPrivate::onDropEvent(QDropEvent* event)
{
    Q_Q(TabbedViews);

    const QMimeData* mimeData = event->mimeData();

    emit q->dataDropped(mimeData);
}

}
