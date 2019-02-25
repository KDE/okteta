/*
    This file is part of the Kasten Framework, made within the KDE community.

    Copyright 2006,2008-2009,2019 Friedrich W. H. Kossebau <kossebau@kde.org>

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
