/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2007, 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_ABSTRACTGROUPEDVIEWS_HPP
#define KASTEN_ABSTRACTGROUPEDVIEWS_HPP

// lib
#include "abstractviewarea.hpp"

template <class T> class QVector;

namespace Kasten {

class AbstractView;
class AbstractGroupedViewsPrivate;

class KASTENGUI_EXPORT AbstractGroupedViews : public AbstractViewArea
{
    Q_OBJECT

protected:
    AbstractGroupedViews();
    explicit AbstractGroupedViews(AbstractGroupedViewsPrivate* d);

public:
    ~AbstractGroupedViews() override;

public Q_SLOTS: // set/action API to be implemented
    virtual void addViews(const QVector<Kasten::AbstractView*>& views) = 0;
    virtual void removeViews(const QVector<Kasten::AbstractView*>& views) = 0;
    virtual void setViewFocus(Kasten::AbstractView* view) = 0;

public: // get API to be implemented
    // returns the list in the order of display
    virtual QVector<AbstractView*> viewList() const = 0;
    virtual int viewCount() const = 0;
    virtual AbstractView* viewFocus() const = 0;

Q_SIGNALS:
    // view was created and already added to the list
    void added(const QVector<Kasten::AbstractView*>& views);
    // view will be removed, already removed from list
    void removing(const QVector<Kasten::AbstractView*>& views);
    // closing the view is requested
    void closeRequest(const QVector<Kasten::AbstractView*>& views);
    void viewFocusChanged(Kasten::AbstractView* view);

private:
    Q_DECLARE_PRIVATE(AbstractGroupedViews)
};

}

#endif
