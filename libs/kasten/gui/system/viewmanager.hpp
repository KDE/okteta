/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2006, 2008-2009, 2019 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_VIEWMANAGER_HPP
#define KASTEN_VIEWMANAGER_HPP

// lib
#include "abstractview.hpp"
// Qt
#include <QVector>
#include <QObject>

namespace Kasten {

class ModelCodecViewManager;
class AbstractViewFactory;

class ViewManagerPrivate;

class KASTENGUI_EXPORT ViewManager : public QObject
{
    Q_OBJECT

public:
    ViewManager();

    ~ViewManager() override;

public:
    void setViewFactory(AbstractViewFactory* factory);

    void createCopyOfView(AbstractView* view, Qt::Alignment alignment = {});
    void removeViews(const QVector<AbstractView*>& views);

public:
    QVector<AbstractView*> views() const;
    AbstractView* viewByWidget(QWidget* widget) const;

public:
    ModelCodecViewManager* codecViewManager() const;

public Q_SLOTS:
    void createViewsFor(const QVector<Kasten::AbstractDocument*>& documents);
    void removeViewsFor(const QVector<Kasten::AbstractDocument*>& documents);

Q_SIGNALS:
    // view was created and already added to the list
    void opened(const QVector<Kasten::AbstractView*>& views);
    // view will be closed, already removed from list
    void closing(const QVector<Kasten::AbstractView*>& views);

private:
    const QScopedPointer<class ViewManagerPrivate> d_ptr;
    Q_DECLARE_PRIVATE(ViewManager)
};

}

#endif
