/*
    This file is part of the Kasten Framework, made within the KDE community.

    Copyright 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "multiviewareas_p.h"

// lib
#include "viewbox.h"
#include "toolinlineviewwidget.h"
#include <abstracttoolinlineview.h>
#include <abstractview.h>
// Kasten core
#include <kasten/abstractdocument.h>

// Qt
#include <QMimeData>

namespace Kasten {
// TODO: catch area focues change!
MultiViewAreasPrivate::MultiViewAreasPrivate(MultiViewAreas* parent)
    : AbstractGroupedViewsPrivate(parent)
    , mCurrentInlineToolViewArea(nullptr)
{
}

MultiViewAreasPrivate::~MultiViewAreasPrivate()
{
    qDeleteAll(mViewAreaList);
    delete mMainSplitter;
}

void MultiViewAreasPrivate::init()
{
    Q_Q(MultiViewAreas);

    mMainSplitter = new QSplitter();

    // create start view area
    TabbedViews* viewArea = new TabbedViews();
    QObject::connect(viewArea, &AbstractViewArea::focusChanged,
                     q, [&](bool hasFocus) { onViewAreaFocusChanged(hasFocus); });
    QObject::connect(viewArea, &AbstractGroupedViews::viewFocusChanged,
                     q, &AbstractGroupedViews::viewFocusChanged);
    QObject::connect(viewArea, &AbstractGroupedViews::closeRequest,
                     q, &AbstractGroupedViews::closeRequest);
    QObject::connect(viewArea, &AbstractGroupedViews::removing,
                     q, [&]() { onViewsRemoved(); });
    QObject::connect(viewArea, &TabbedViews::dataOffered,
                     q, &MultiViewAreas::dataOffered);
    QObject::connect(viewArea, &TabbedViews::dataDropped,
                     q, &MultiViewAreas::dataDropped);

    mViewAreaList.append(viewArea);
    mCurrentViewArea = viewArea;

    mMainSplitter->addWidget(viewArea->widget());
}

AbstractViewArea* MultiViewAreasPrivate::splitViewArea(AbstractViewArea* _viewArea, Qt::Orientation orientation)
{
    Q_Q(MultiViewAreas);

    TabbedViews* firstViewArea = static_cast<TabbedViews*>(_viewArea);
    QWidget* firstViewAreaWidget = firstViewArea->widget();
    QSplitter* baseSplitter = static_cast<QSplitter*>(firstViewAreaWidget->parentWidget());

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

    TabbedViews* secondViewArea = new TabbedViews();
    QObject::connect(secondViewArea, &AbstractViewArea::focusChanged,
                     q, [&](bool hasFocus) { onViewAreaFocusChanged(hasFocus); });
    QObject::connect(secondViewArea, &AbstractGroupedViews::viewFocusChanged,
                     q, &AbstractGroupedViews::viewFocusChanged);
    QObject::connect(secondViewArea, &AbstractGroupedViews::closeRequest,
                     q, &AbstractGroupedViews::closeRequest);
    QObject::connect(secondViewArea, &AbstractGroupedViews::removing,
                     q, [&]() { onViewsRemoved(); });
    QObject::connect(secondViewArea, &TabbedViews::dataOffered,
                     q, &MultiViewAreas::dataOffered);
    QObject::connect(secondViewArea, &TabbedViews::dataDropped,
                     q, &MultiViewAreas::dataDropped);

    mViewAreaList.append(secondViewArea);
    mCurrentViewArea = secondViewArea;

    splitter->setOrientation(orientation == Qt::Horizontal ? Qt::Vertical : Qt::Horizontal);
    splitter->addWidget(secondViewArea->widget());
    // set to equal sizes
    QList<int> splitterSizes = splitter->sizes();
    // TODO: check if there are more, style-dependent spaces
    const int equalSize = (splitterSizes[0] + splitterSizes[1] - splitter->handleWidth()) / 2;
    splitterSizes[1] = splitterSizes[0] = equalSize;
    splitter->setSizes(splitterSizes);

    const QList<AbstractViewArea*> viewAreas { secondViewArea };
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

    TabbedViews* viewArea = qobject_cast<TabbedViews*>(q->sender());

    if (viewArea->viewCount() == 0) {
        QWidget* viewAreaWidget = viewArea->widget();
        QSplitter* baseSplitter = static_cast<QSplitter*>(viewAreaWidget->parentWidget());

        const int index = baseSplitter->indexOf(viewAreaWidget);
        const int otherIndex = 1 - index;

        QWidget* otherWidget = baseSplitter->widget(otherIndex);
        // do not delete the main splitter
        if (baseSplitter != mMainSplitter) {
            QSplitter* baseOfBaseSplitter = static_cast<QSplitter*>(baseSplitter->parentWidget());

            const QList<int> baseOfBaseSplitterSizes = baseOfBaseSplitter->sizes();
            const int indexOfBaseSplitter = baseOfBaseSplitter->indexOf(baseSplitter);
            baseOfBaseSplitter->insertWidget(indexOfBaseSplitter, otherWidget);
            viewAreaWidget->setParent(nullptr);
            delete baseSplitter;
            baseOfBaseSplitter->setSizes(baseOfBaseSplitterSizes);
        }

        mViewAreaList.removeOne(viewArea);

        if (mCurrentInlineToolViewArea == viewArea) {
            mCurrentInlineToolViewArea = nullptr;
        }

        if (mCurrentViewArea == viewArea) {
            // search for the previous widget which is the next or the previous, using index
            while (true) {
                QSplitter* splitter = qobject_cast<QSplitter*>(otherWidget);
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

        const QList<AbstractViewArea*> viewAreas { viewArea };
        emit q->viewAreasRemoved(viewAreas);

        delete viewArea;
    }
}

void MultiViewAreasPrivate::onViewAreaFocusChanged(bool hasFocus)
{
    Q_Q(MultiViewAreas);

    TabbedViews* viewArea = qobject_cast<TabbedViews*>(q->sender());

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

#if 0
void MultiViewAreasPrivate::onModifiedChanged(AbstractDocument::SyncStates newStates)
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

}
