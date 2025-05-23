/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2007, 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_TABBEDVIEWS_P_HPP
#define KASTEN_TABBEDVIEWS_P_HPP

// lib
#include "abstractgroupedviews_p.hpp"
#include "tabbedviews.hpp"
#include "viewareabox.hpp"
#include "viewbox.hpp"
#include <abstractview.hpp>
#include "tabwidget.hpp"
// Std
#include <memory>

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
    [[nodiscard]]
    QWidget* widget() const;
    [[nodiscard]]
    bool hasFocus() const;

public: // AbstractGroupedViews API
    void addViews(const QVector<AbstractView*>& views);
    void removeViews(const QVector<AbstractView*>& views);
    void setViewFocus(AbstractView* view);

    [[nodiscard]]
    QVector<AbstractView*> viewList() const;
    [[nodiscard]]
    int viewCount() const;
    [[nodiscard]]
    AbstractView* viewFocus() const;

public: // If::ToolInlineViewable API
    void setCurrentToolInlineView(AbstractToolInlineView* view);
    [[nodiscard]]
    AbstractToolInlineView* currentToolInlineView() const;

public:
    [[nodiscard]]
    int indexOf(AbstractView* view) const;

public: // Q_SLOTS:
    void onCurrentChanged(int index);
    void onTabCloseRequest(int tabIndex);
    void onTitleChanged(const QString& newTitle);
//     void onModifiedChanged( Kasten::AbstractDocument::SyncStates newStates );
    void onViewFocusChanged(bool hasFocus);
    void onContextMenuRequested(QPoint pos);
    void onMouseMiddleClick();
    void onEmptySpaceMouseDoubleClicked();
    void onDragMoveEvent(const QDragMoveEvent* event, bool& accept);
    void onDropEvent(QDropEvent* event);

private:
    Q_DECLARE_PUBLIC(TabbedViews)

private:
    // in (reverse) order of destruction
    std::unique_ptr<TabWidget> mTabWidget;
    std::unique_ptr<ViewAreaBox> mViewAreaBox;

    AbstractView* mCurrentView = nullptr;
    AbstractToolInlineView* mCurrentToolInlineView = nullptr;
};

inline QWidget* TabbedViewsPrivate::widget() const { return mViewAreaBox.get(); }
inline int TabbedViewsPrivate::viewCount()   const { return mTabWidget->count(); }

inline AbstractView* TabbedViewsPrivate::viewFocus() const
{
    const auto* viewBox = static_cast<const ViewBox*>(mTabWidget->currentWidget());
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

inline AbstractToolInlineView* TabbedViewsPrivate::currentToolInlineView() const
{
    return mCurrentToolInlineView;
}


}

#endif
