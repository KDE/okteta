/*
    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "multiviewareas_p.hpp"

// lib
#include <abstracttoolinlineview.hpp>
#include <abstractview.hpp>
// Kasten core
#include <Kasten/AbstractDocument>

// Qt
#include <QMimeData>
// Std
#include <utility>

namespace Kasten {
// TODO: catch area focus change!
MultiViewAreasPrivate::MultiViewAreasPrivate(MultiViewAreas* parent)
    : AbstractGroupedViewsPrivate(parent)
{
}

MultiViewAreasPrivate::~MultiViewAreasPrivate() = default;

TabbedViews* MultiViewAreasPrivate::createViewArea(QSplitter* splitter)
{
    Q_Q(MultiViewAreas);

    const auto& managedviewArea = mViewAreaList.emplace_back(std::make_unique<TabbedViews>());

    auto* const viewArea = managedviewArea.get();

    QObject::connect(viewArea, &AbstractViewArea::focusChanged,
                     q, [this](bool hasFocus) { onViewAreaFocusChanged(hasFocus); });
    QObject::connect(viewArea, &AbstractGroupedViews::viewFocusChanged,
                     q, &AbstractGroupedViews::viewFocusChanged);
    QObject::connect(viewArea, &AbstractGroupedViews::closeRequest,
                     q, &AbstractGroupedViews::closeRequest);
    QObject::connect(viewArea, &AbstractGroupedViews::removing,
                     q, [this]() { onViewsRemoved(); });

    QObject::connect(viewArea, &TabbedViews::contextMenuRequested,
                     q, [this](AbstractView* view, QPoint pos) { onContextMenuRequested(view, pos); });
    QObject::connect(viewArea, &TabbedViews::dataOffered,
                     q, &MultiViewAreas::dataOffered);
    QObject::connect(viewArea, &TabbedViews::dataDropped,
                     q, &MultiViewAreas::dataDropped);
    QObject::connect(viewArea, &TabbedViews::newDocumentRequested,
                     q, &MultiViewAreas::newDocumentRequested);

    mCurrentViewArea = viewArea;

    splitter->addWidget(viewArea->widget());

    return viewArea;
}

void MultiViewAreasPrivate::init()
{
    mMainSplitter = std::make_unique<QSplitter>();

    // create start view area
    createViewArea(mMainSplitter.get());
}

AbstractViewArea* MultiViewAreasPrivate::splitViewArea(AbstractViewArea* _viewArea, Qt::Orientation orientation)
{
    Q_Q(MultiViewAreas);

    auto* const firstViewArea = static_cast<TabbedViews*>(_viewArea);
    QWidget* const firstViewAreaWidget = firstViewArea->widget();
    auto* const baseSplitter = static_cast<QSplitter*>(firstViewAreaWidget->parentWidget());

    QSplitter* splitter;
    if (baseSplitter->count() == 1) { // only valid with mMainSplitter
        splitter = baseSplitter;
    } else {
        const QList<int> baseSplitterSizes = baseSplitter->sizes();
        const int index = baseSplitter->indexOf(firstViewAreaWidget);
        splitter = new QSplitter(baseSplitter);
        baseSplitter->insertWidget(index, splitter);
        splitter->addWidget(firstViewAreaWidget);
        baseSplitter->setSizes(baseSplitterSizes);
    }

    splitter->setOrientation(orientation == Qt::Horizontal ? Qt::Vertical : Qt::Horizontal);

    TabbedViews* const secondViewArea = createViewArea(splitter);

    // set to equal sizes
    QList<int> splitterSizes = splitter->sizes();
    // TODO: check if there are more, style-dependent spaces
    const int equalSize = (splitterSizes[0] + splitterSizes[1] - splitter->handleWidth()) / 2;
    splitterSizes[1] = splitterSizes[0] = equalSize;
    splitter->setSizes(splitterSizes);

    const QVector<AbstractViewArea*> viewAreas { secondViewArea };
    Q_EMIT q->viewAreasAdded(viewAreas);
    Q_EMIT q->viewAreaFocusChanged(secondViewArea);

    return secondViewArea;
}

void MultiViewAreasPrivate::onViewsRemoved()
{
    Q_Q(MultiViewAreas);

    // keep a minimum of one area
    if (mViewAreaList.size() < 2) {
        return;
    }

    auto* const removingViewArea = qobject_cast<TabbedViews*>(q->sender());

    if (removingViewArea->viewCount() == 0) {
        QWidget* const viewAreaWidget = removingViewArea->widget();
        auto* const baseSplitter = static_cast<QSplitter*>(viewAreaWidget->parentWidget());

        const int index = baseSplitter->indexOf(viewAreaWidget);
        const int otherIndex = 1 - index;

        QWidget* otherWidget = baseSplitter->widget(otherIndex);
        // do not delete the main splitter
        if (baseSplitter != mMainSplitter.get()) {
            auto* const baseOfBaseSplitter = static_cast<QSplitter*>(baseSplitter->parentWidget());

            const QList<int> baseOfBaseSplitterSizes = baseOfBaseSplitter->sizes();
            const int indexOfBaseSplitter = baseOfBaseSplitter->indexOf(baseSplitter);
            baseOfBaseSplitter->insertWidget(indexOfBaseSplitter, otherWidget);
            viewAreaWidget->setParent(nullptr);
            delete baseSplitter;
            baseOfBaseSplitter->setSizes(baseOfBaseSplitterSizes);
        }

        const auto it = std::find_if(mViewAreaList.begin(), mViewAreaList.end(), [removingViewArea](const auto& area){
            return (area.get() == removingViewArea);
        });
        if (it != mViewAreaList.end()) {
            // remove from list, but keep alive a bit more, deleted nelow finally
            it->release();
            mViewAreaList.erase(it);
        }

        if (mCurrentInlineToolViewArea == removingViewArea) {
            mCurrentInlineToolViewArea = nullptr;
        }

        if (mCurrentViewArea == removingViewArea) {
            // search for the previous widget which is the next or the previous, using index
            while (true) {
                auto* const splitter = qobject_cast<QSplitter*>(otherWidget);
                if (splitter) {
                    otherWidget = splitter->widget(index);
                } else {
                    break;
                }
            }

            for (const auto& viewArea : mViewAreaList) {
                if (viewArea->widget() == otherWidget) {
                    viewArea->setFocus();
                    break;
                }
            }
        }

        const QVector<AbstractViewArea*> viewAreas { removingViewArea };
        Q_EMIT q->viewAreasRemoved(viewAreas);

        delete removingViewArea;
    }
}

void MultiViewAreasPrivate::onViewAreaFocusChanged(bool hasFocus)
{
    Q_Q(MultiViewAreas);

    auto* const viewArea = qobject_cast<TabbedViews*>(q->sender());

    if (mCurrentViewArea == viewArea) {
        return;
    }

    if (mCurrentInlineToolViewArea && mCurrentInlineToolViewArea == mCurrentViewArea) {
        mCurrentInlineToolViewArea->setCurrentToolInlineView(nullptr);
    }

    // TODO: care for ! hasFocus?
    if (hasFocus) {
        mCurrentViewArea = viewArea;

        Q_EMIT q->viewAreaFocusChanged(viewArea);
        Q_EMIT q->viewFocusChanged(viewArea->viewFocus());
    }
}

void MultiViewAreasPrivate::onContextMenuRequested(AbstractView* view, QPoint pos)
{
    Q_Q(MultiViewAreas);

    auto* const viewArea = qobject_cast<TabbedViews*>(q->sender());

    auto* const viewAreaWidget = viewArea->widget();
    Q_EMIT q->contextMenuRequested(viewArea, view, viewAreaWidget->mapTo(mMainSplitter.get(), pos));
}

#if 0
void MultiViewAreasPrivate::onModifiedChanged(AbstractDocument::SyncStates newStates)
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

}
