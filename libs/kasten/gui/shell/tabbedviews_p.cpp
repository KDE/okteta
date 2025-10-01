/*
    SPDX-FileCopyrightText: 2007, 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "tabbedviews_p.hpp"

// lib
#include <abstracttoolinlineview.hpp>
#include <logging.hpp>
// Qt
#include <QDragMoveEvent>
#include <QDropEvent>
#include <QTabBar>
#include <QApplication>
#include <QClipboard>

namespace Kasten {

TabbedViewsPrivate::TabbedViewsPrivate(TabbedViews* parent)
    : AbstractGroupedViewsPrivate(parent)
{
}

TabbedViewsPrivate::~TabbedViewsPrivate() = default;

void TabbedViewsPrivate::init()
{
    Q_Q(TabbedViews);

    mTabWidget = std::make_unique<TabWidget>();
    mTabWidget->setMovable(true);
    mTabWidget->setContextMenuPolicy(Qt::CustomContextMenu);

    mViewAreaBox = std::make_unique<ViewAreaBox>(mTabWidget.get());

    QObject::connect(mTabWidget.get(), &QTabWidget::tabCloseRequested,
                     q, [this](int index) { onTabCloseRequest(index); });
    QObject::connect(mTabWidget.get(), &QTabWidget::currentChanged,
                     q, [this](int index) { onCurrentChanged(index); });

    QObject::connect(mTabWidget.get(), &QWidget::customContextMenuRequested,
                     q, [this](const QPoint& pos) { onContextMenuRequested(pos); });

    QObject::connect(mTabWidget.get(), &TabWidget::testCanDecode,
                     q, [this](const QDragMoveEvent* event, bool& accept) { onDragMoveEvent(event, accept); });
    QObject::connect(mTabWidget.get(), &TabWidget::receivedDropEvent,
                     q, [this](QDropEvent* event) { onDropEvent(event); });
    QObject::connect(mTabWidget.get(), &TabWidget::mouseMiddleClick,
                     q, [this]() { onMouseMiddleClick(); });
    QObject::connect(mTabWidget.get(), &TabWidget::emptySpaceMouseDoubleClicked,
                     q, [this]() { onEmptySpaceMouseDoubleClicked(); });
}

QVector<AbstractView*> TabbedViewsPrivate::viewList() const
{
    QVector<AbstractView*> result;

    const int count = mTabWidget->count();
    result.reserve(count);
    for (int i = 0; i < count; ++i) {
        const auto* const viewBox = static_cast<const ViewBox*>(mTabWidget->widget(i));
        AbstractView* const view = viewBox->view();
        result.append(view);
    }

    return result;
}

int TabbedViewsPrivate::indexOf(AbstractView* view) const
{
    int result = -1;

    const int tabCount = mTabWidget->count();
    for (int i = 0; i < tabCount; ++i) {
        const auto* const viewBox = static_cast<const ViewBox*>(mTabWidget->widget(i));
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

    if (views.isEmpty()) {
        return;
    }

    int insertIndex = mTabWidget->currentIndex() + 1;
    for (AbstractView* view : views) {
        QObject::connect(view, &AbstractModel::titleChanged,
                         q, [this](const QString& title) { onTitleChanged(title); });

        auto* const viewBox = new ViewBox(view, mTabWidget.get());
        mTabWidget->insertTab(insertIndex, viewBox, view->title());
        ++insertIndex;
    }

    mTabWidget->setCurrentIndex(insertIndex - 1);

    // fix for Qt bug:
    if (mTabWidget->count() == 1) {
        // simulate signal reaction
        onCurrentChanged(0);
    }

    Q_EMIT q->added(views);
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
            auto* const viewBox = static_cast<ViewBox*>(mTabWidget->widget(index));

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

    Q_EMIT q->removing(views);
}

void TabbedViewsPrivate::setCurrentToolInlineView(AbstractToolInlineView* view)
{
    if (mCurrentToolInlineView != view) {
        mCurrentToolInlineView = view;
        QWidget* const toolInlineViewWidget = mCurrentToolInlineView ? mCurrentToolInlineView->widget() : nullptr;
        mViewAreaBox->setBottomToolWidget(toolInlineViewWidget);
    }

    if (mCurrentToolInlineView) {
        mViewAreaBox->showBottomToolWidget();
    }
}

void TabbedViewsPrivate::onCurrentChanged(int index)
{
    Q_Q(TabbedViews);

    const auto* const viewBox = static_cast<const ViewBox*>(mTabWidget->widget(index));
    AbstractView* const view = viewBox ? viewBox->view() : nullptr;

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
                         q, [this](bool hasFocus) { onViewFocusChanged(hasFocus); });
        view->widget()->setFocus();
    }

    Q_EMIT q->viewFocusChanged(view);
}

void TabbedViewsPrivate::onTabCloseRequest(int tabIndex)
{
    Q_Q(TabbedViews);

    const QWidget* const widget = mTabWidget->widget(tabIndex);
    const auto* const viewBox = static_cast<const ViewBox*>(widget);
    AbstractView* const view = viewBox->view();

    const QVector<Kasten::AbstractView*> views { view };
    Q_EMIT q->closeRequest(views);
}

void TabbedViewsPrivate::onTitleChanged(const QString& newTitle)
{
    Q_Q(TabbedViews);

    auto* const view = qobject_cast<AbstractView*>(q->sender());
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
    auto* const view = qobject_cast<AbstractView*>(sender());
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

//     auto* const view = qobject_cast<AbstractView*>( q->sender() );
// qCDebug(LOG_KASTEN_GUI)<<view<<view->title()<<hasFocus;

    Q_EMIT q->focusChanged(hasFocus);
}

void TabbedViewsPrivate::onContextMenuRequested(QPoint pos)
{
    Q_Q(TabbedViews);

    AbstractView* view = nullptr;
    QTabBar* const tabBar = mTabWidget->tabBar();
    const int tabIndex = tabBar->tabAt(tabBar->mapFrom(mTabWidget.get(), pos));
    if (tabIndex != -1) {
        const QWidget* const widget = mTabWidget->widget(tabIndex);
        const auto* const viewBox = static_cast<const ViewBox*>(widget);
        view = viewBox->view();
    }
    Q_EMIT q->contextMenuRequested(view, mTabWidget->mapTo(mViewAreaBox.get(), pos));
}

void TabbedViewsPrivate::onMouseMiddleClick()
{
    Q_Q(TabbedViews);

    const QMimeData* const mimeData = QApplication::clipboard()->mimeData(QClipboard::Selection);

    Q_EMIT q->dataDropped(mimeData);
}

void TabbedViewsPrivate::onEmptySpaceMouseDoubleClicked()
{
    Q_Q(TabbedViews);

    Q_EMIT q->newDocumentRequested();
}

void TabbedViewsPrivate::onDragMoveEvent(const QDragMoveEvent* event, bool& accepted)
{
    Q_Q(TabbedViews);

    const QMimeData* const mimeData = event->mimeData();

    Q_EMIT q->dataOffered(mimeData, accepted);
}

void TabbedViewsPrivate::onDropEvent(QDropEvent* event)
{
    Q_Q(TabbedViews);

    const QMimeData* const mimeData = event->mimeData();

    Q_EMIT q->dataDropped(mimeData);
}

}
