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
// Std
#include <memory>

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

public Q_SLOTS:
    void createViewsFor(const QVector<Kasten::AbstractDocument*>& documents);
    void removeViewsFor(const QVector<Kasten::AbstractDocument*>& documents);

Q_SIGNALS:
    // view was created and already added to the list
    void opened(const QVector<Kasten::AbstractView*>& views);
    // view will be closed, already removed from list
    void closing(const QVector<Kasten::AbstractView*>& views);

private:
    const std::unique_ptr<class ViewManagerPrivate> d_ptr;
    Q_DECLARE_PRIVATE(ViewManager)
};

}

#endif
