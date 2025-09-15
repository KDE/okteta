/*
    SPDX-FileCopyrightText: 2006, 2008-2009, 2019 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_VIEWMANAGER_P_HPP
#define KASTEN_VIEWMANAGER_P_HPP

// lib
#include "viewmanager.hpp"
// Std
#include <memory>

namespace Kasten {

class ViewManagerPrivate
{
public:
    explicit ViewManagerPrivate(ViewManager* q);
    ViewManagerPrivate(const ViewManagerPrivate&) = delete;
    ViewManagerPrivate(ViewManagerPrivate&&) = delete;

    ~ViewManagerPrivate();

    ViewManagerPrivate& operator=(const ViewManagerPrivate&) = delete;
    ViewManagerPrivate& operator=(ViewManagerPrivate&&) = delete;

public:
    void setViewFactory(std::unique_ptr<AbstractViewFactory>&& factory);

    void createCopyOfView(AbstractView* view, Qt::Alignment alignment = {});
    void removeViews(const QVector<AbstractView*>& views);

public:
    [[nodiscard]]
    QVector<AbstractView*> views() const;
    [[nodiscard]]
    AbstractView* viewByWidget(QWidget* widget) const;

public:
    [[nodiscard]]
    ModelCodecViewManager* codecViewManager() const;

public:
    void createViewsFor(const QVector<Kasten::AbstractDocument*>& documents);
    void removeViewsFor(const QVector<Kasten::AbstractDocument*>& documents);

private:
    ViewManager* const q_ptr;

    // TODO: remove into own singleton
    const std::unique_ptr<ModelCodecViewManager> mCodecViewManager;

    QVector<AbstractView*> mViewList;
    std::unique_ptr<AbstractViewFactory> mFactory;

    Q_DECLARE_PUBLIC(ViewManager)
};

inline ModelCodecViewManager* ViewManagerPrivate::codecViewManager() const { return mCodecViewManager.get(); }

}

#endif
