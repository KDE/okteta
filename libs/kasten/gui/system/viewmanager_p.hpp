/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2006, 2008-2009, 2019 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_VIEWMANAGER_P_HPP
#define KASTEN_VIEWMANAGER_P_HPP

// lib
#include "viewmanager.hpp"

namespace Kasten {

class ViewManagerPrivate
{
public:
    explicit ViewManagerPrivate(ViewManager* q);

    ~ViewManagerPrivate();

public:
    void setViewFactory(AbstractViewFactory* factory);

    void createCopyOfView(AbstractView* view, Qt::Alignment alignment = {});
    void removeViews(const QVector<AbstractView*>& views);

public:
    QVector<AbstractView*> views() const;
    AbstractView* viewByWidget(QWidget* widget) const;

public:
    ModelCodecViewManager* codecViewManager() const;

public:
    void createViewsFor(const QVector<Kasten::AbstractDocument*>& documents);
    void removeViewsFor(const QVector<Kasten::AbstractDocument*>& documents);

private:
    ViewManager* const q_ptr;

    QVector<AbstractView*> mViewList;
    AbstractViewFactory* mFactory = nullptr;

    // TODO: remove into own singleton
    ModelCodecViewManager* mCodecViewManager;

    Q_DECLARE_PUBLIC(ViewManager)
};

inline ModelCodecViewManager* ViewManagerPrivate::codecViewManager() const { return mCodecViewManager; }

}

#endif
