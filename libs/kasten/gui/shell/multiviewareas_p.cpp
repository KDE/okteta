/*
    This file is part of the Kasten Framework, made within the KDE community.

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

namespace Kasten {
// TODO: catch area focus change!
MultiViewAreasPrivate::MultiViewAreasPrivate(MultiViewAreas* parent)
    : AbstractGroupedViewsPrivate(parent)
{
}

MultiViewAreasPrivate::~MultiViewAreasPrivate()
{
    qDeleteAll(mViewAreaList);
    delete mMainSplitter;
}

TabbedViews* MultiViewAreasPrivate::createViewArea(QSplitter* splitter)
{
    Q_Q(MultiViewAreas);

    auto* viewArea = new TabbedViews();

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

    mViewAreaList.append(viewArea);
    mCurrentViewArea = viewArea;

    splitter->addWidget(viewArea->widget());

    return viewArea;
}

void MultiViewAreasPrivate::init()
{
    mMainSplitter = new QSplitter();

    // create start view area
    createViewArea(mMainSplitter);
}

AbstractViewArea* MultiViewAreasPrivate::splitViewArea(AbstractViewArea* _viewArea, Qt::Orientation orientation)
{
    Q_Q(MultiViewAreas);

    auto* firstViewArea = static_cast<TabbedViews*>(_viewArea);
    QWidget* firstViewAreaWidget = firstViewArea->widget();
    auto* baseSplitter = static_cast<QSplitter*>(firstViewAreaWidget->parentWidget());

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

    TabbedViews* secondViewArea = createViewArea(splitter);

    // set to equal sizes
    QList<int> splitterSizes = splitter->sizes();
    // TODO: check if there are more, style-dependent spaces
    const int equalSize = (splitterSizes[0] + splitterSizes[1] - splitter->handleWidth()) / 2;
    splitterSizes[1] = splitterSizes[0] = equalSize;
    splitter->setSizes(splitterSizes);

    const QVector<AbstractViewArea*> viewAreas { secondViewArea };
    emit q->viewAreasAdded(viewAreas);
    emit q->viewAreaFocusChanged(secondViewArea);

    return secondViewArea;
}

void MultiViewAreasPrivate::onViewsRemoved()
{
    Q_Q(MultiViewAreas);

    // keep a minimum of one area
    if (mViewAreaList.count() < 2) {
        return;
    }

    auto* const removingViewArea = qobject_cast<TabbedViews*>(q->sender());

    if (removingViewArea->viewCount() == 0) {
        QWidget* const viewAreaWidget = removingViewArea->widget();
        auto* baseSplitter = static_cast<QSplitter*>(viewAreaWidget->parentWidget());

        const int index = baseSplitter->indexOf(viewAreaWidget);
        const int otherIndex = 1 - index;

        QWidget* otherWidget = baseSplitter->widget(otherIndex);
        // do not delete the main splitter
        if (baseSplitter != mMainSplitter) {
            auto* baseOfBaseSplitter = static_cast<QSplitter*>(baseSplitter->parentWidget());

            const QList<int> baseOfBaseSplitterSizes = baseOfBaseSplitter->sizes();
            const int indexOfBaseSplitter = baseOfBaseSplitter->indexOf(baseSplitter);
            baseOfBaseSplitter->insertWidget(indexOfBaseSplitter, otherWidget);
            viewAreaWidget->setParent(nullptr);
            delete baseSplitter;
            baseOfBaseSplitter->setSizes(baseOfBaseSplitterSizes);
        }

        mViewAreaList.removeOne(removingViewArea);

        if (mCurrentInlineToolViewArea == removingViewArea) {
            mCurrentInlineToolViewArea = nullptr;
        }

        if (mCurrentViewArea == removingViewArea) {
            // search for the previous widget which is the next or the previous, using index
            while (true) {
                auto* splitter = qobject_cast<QSplitter*>(otherWidget);
                if (splitter) {
                    otherWidget = splitter->widget(index);
                } else {
                    break;
                }
            }

            for (TabbedViews* viewArea : qAsConst(mViewAreaList)) {
                if (viewArea->widget() == otherWidget) {
                    viewArea->setFocus();
                    break;
                }
            }
        }

        const QVector<AbstractViewArea*> viewAreas { removingViewArea };
        emit q->viewAreasRemoved(viewAreas);

        delete removingViewArea;
    }
}

void MultiViewAreasPrivate::onViewAreaFocusChanged(bool hasFocus)
{
    Q_Q(MultiViewAreas);

    auto* viewArea = qobject_cast<TabbedViews*>(q->sender());

    if (mCurrentViewArea == viewArea) {
        return;
    }

    if (mCurrentInlineToolViewArea && mCurrentInlineToolViewArea == mCurrentViewArea) {
        mCurrentInlineToolViewArea->setCurrentToolInlineView(nullptr);
    }

    // TODO: care for ! hasFocus?
    if (hasFocus) {
        mCurrentViewArea = viewArea;

        emit q->viewAreaFocusChanged(viewArea);
        emit q->viewFocusChanged(viewArea->viewFocus());
    }
}

void MultiViewAreasPrivate::onContextMenuRequested(AbstractView* view, QPoint pos)
{
    Q_Q(MultiViewAreas);

    auto* viewArea = qobject_cast<TabbedViews*>(q->sender());

    auto* viewAreaWidget = viewArea->widget();
    emit q->contextMenuRequested(viewArea, view, viewAreaWidget->mapTo(mMainSplitter, pos));
}

#if 0
void MultiViewAreasPrivate::onModifiedChanged(AbstractDocument::SyncStates newStates)
{
    Q_UNUSED(newStates)
    auto* view = qobject_cast<AbstractView*>(sender());
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
