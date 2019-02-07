/*
    This file is part of the Kasten Framework, made within the KDE community.

    Copyright 2007,2009 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#ifndef TABBEDVIEWS_P_HPP
#define TABBEDVIEWS_P_HPP

// lib
#include "abstractgroupedviews_p.hpp"
#include "tabbedviews.hpp"
#include "viewareabox.hpp"
#include "viewbox.hpp"
#include <abstractview.hpp>
#include "tabwidget.hpp"

namespace Kasten {

class TabbedViewsPrivate : public AbstractGroupedViewsPrivate
{
public:
    explicit TabbedViewsPrivate(TabbedViews* parent);

    ~TabbedViewsPrivate() override;

public:
    void init();

public: // AbstractViewArea API
    void setFocus();
    QWidget* widget() const;
    bool hasFocus() const;

public: // AbstractGroupedViews API
    void addViews(const QVector<AbstractView*>& views);
    void removeViews(const QVector<AbstractView*>& views);
    void setViewFocus(AbstractView* view);

    QVector<AbstractView*> viewList() const;
    int viewCount() const;
    AbstractView* viewFocus() const;

public: // If::ToolInlineViewable API
    void setCurrentToolInlineView(AbstractToolInlineView* view);

public:
    int indexOf(AbstractView* view) const;

public: // Q_SLOTS:
    void onCurrentChanged(int index);
    void onTabCloseRequest(int tabIndex);
    void onTitleChanged(const QString& newTitle);
//     void onModifiedChanged( Kasten::AbstractDocument::SyncStates newStates );
    void onViewFocusChanged(bool focusChanged);
    void onMouseMiddleClick();
    void onDragMoveEvent(const QDragMoveEvent* event, bool& accept);
    void onDropEvent(QDropEvent* event);

protected:
    Q_DECLARE_PUBLIC(TabbedViews)

protected:
    ViewAreaBox* mViewAreaBox;
    TabWidget* mTabWidget;

    AbstractView* mCurrentView = nullptr;
};

inline QWidget* TabbedViewsPrivate::widget() const { return mViewAreaBox; }
inline int TabbedViewsPrivate::viewCount()   const { return mTabWidget->count(); }

inline AbstractView* TabbedViewsPrivate::viewFocus() const
{
    const ViewBox* viewBox = static_cast<const ViewBox*>(mTabWidget->currentWidget());
    return viewBox ? viewBox->view() : nullptr;
}

inline bool TabbedViewsPrivate::hasFocus() const
{
    return mCurrentView ? mCurrentView->hasFocus() : false;
}

inline void TabbedViewsPrivate::setViewFocus(AbstractView* view)
{
    const int index = indexOf(view);
    mTabWidget->setCurrentIndex(index);
}

inline void TabbedViewsPrivate::setFocus()
{
    if (mCurrentView) {
        mCurrentView->setFocus();
    }
    // TODO: would ensure the bottomwidget gets focus if there is one. Just, it didn't work at all
//     mViewAreaBox->setFocus();
}

}

#endif
