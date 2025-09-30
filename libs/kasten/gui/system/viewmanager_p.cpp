/*
    SPDX-FileCopyrightText: 2006, 2008-2009, 2019 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "viewmanager_p.hpp"

// lib
#include "abstractviewfactory.hpp"
#include "dummyview.hpp"
#include <logging.hpp>
// Qt
#include <QMutableVectorIterator>
// Std
#include <utility>

// temporary
#include "modelcodecviewmanager.hpp"

namespace Kasten {

ViewManagerPrivate::ViewManagerPrivate(ViewManager* q)
    : q_ptr(q)
    , mCodecViewManager(std::make_unique<ModelCodecViewManager>())
{
}

ViewManagerPrivate::~ViewManagerPrivate()
{
    // TODO: signal closing here, too?
    qDeleteAll(mViewList);
}

void ViewManagerPrivate::setViewFactory(std::unique_ptr<AbstractViewFactory>&& factory)
{
    mFactory = std::move(factory);
}

QVector<AbstractView*> ViewManagerPrivate::views() const
{
    return mViewList;
}

AbstractView* ViewManagerPrivate::viewByWidget(QWidget* widget) const
{
    const auto it = std::find_if(mViewList.constBegin(), mViewList.constEnd(), [widget](AbstractView* view) {
        return (view->widget() == widget);
    });

    return (it != mViewList.constEnd()) ? *it : nullptr;
}

void ViewManagerPrivate::createCopyOfView(AbstractView* view, Qt::Alignment alignment)
{
    Q_Q(ViewManager);

    auto viewCopy = mFactory->createCopyOfView(view, alignment);
    if (!viewCopy) {
        auto* documentOfView = view->findBaseModel<AbstractDocument*>();
        viewCopy = std::make_unique<DummyView>(documentOfView);
    }

    auto* rawViewCopy = viewCopy.release();

    mViewList.append(rawViewCopy);

    const QVector<Kasten::AbstractView*> views { rawViewCopy };
    Q_EMIT q->opened(views);
}

void ViewManagerPrivate::createViewsFor(const QVector<Kasten::AbstractDocument*>& documents)
{
    Q_Q(ViewManager);

    QVector<Kasten::AbstractView*> openedViews;

    openedViews.reserve(documents.size());
    mViewList.reserve(mViewList.size() + documents.size());
    for (AbstractDocument* document : documents) {
        auto view = mFactory->createViewFor(document);
        if (!view) {
            view = std::make_unique<DummyView>(document);
        }

        auto* rawView = view.release();
        mViewList.append(rawView);
        openedViews.append(rawView);
    }

    if (!openedViews.isEmpty()) {
        Q_EMIT q->opened(openedViews);
    }
}

void ViewManagerPrivate::removeViewsFor(const QVector<Kasten::AbstractDocument*>& documents)
{
    Q_Q(ViewManager);

    QVector<Kasten::AbstractView*> closedViews;

    QMutableVectorIterator<AbstractView*> it(mViewList);
    for (AbstractDocument* document : documents) {
        while (it.hasNext()) {
            AbstractView* view = it.next();
            auto* documentOfView = view->findBaseModel<AbstractDocument*>();
            if (documentOfView == document) {
                it.remove();
                closedViews.append(view);
            }
        }
        it.toFront();
    }

    Q_EMIT q->closing(closedViews);

    for (AbstractView* view : std::as_const(closedViews)) {
//         qCDebug(LOG_KASTEN_GUI) << view->title();
        delete view;
    }
}

void ViewManagerPrivate::removeViews(const QVector<AbstractView*>& views)
{
    Q_Q(ViewManager);

    for (AbstractView* view : views) {
        mViewList.removeOne(view);
    }

    Q_EMIT q->closing(views);

    for (AbstractView* view : views) {
//         qCDebug(LOG_KASTEN_GUI)<<view->title();
        delete view;
    }
}

}
