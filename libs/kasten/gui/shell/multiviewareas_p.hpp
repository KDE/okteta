/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_MULTIVIEWAREAS_P_HPP
#define KASTEN_MULTIVIEWAREAS_P_HPP

// lib
#include "abstractgroupedviews_p.hpp"
#include "multiviewareas.hpp"
#include "tabbedviews.hpp"
// Qt
#include <QSplitter>

class QDragMoveEvent;
class QDropEvent;

namespace Kasten {

class TabbedViews;

class MultiViewAreasPrivate : public AbstractGroupedViewsPrivate
{
public:
    explicit MultiViewAreasPrivate(MultiViewAreas* parent);

    ~MultiViewAreasPrivate() override;

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
    AbstractToolInlineView* currentToolInlineView() const;

public: // If::ViewAreaSplitable API
    AbstractViewArea* splitViewArea(AbstractViewArea* viewArea, Qt::Orientation orientation);
    void closeViewArea(AbstractViewArea* viewArea);
    void setViewAreaFocus(AbstractViewArea* viewArea);
    AbstractViewArea* viewAreaFocus() const;
    int viewAreasCount() const;

private:
    int indexOf(AbstractView* view) const;

private:  // slots
    void onViewsRemoved();
    void onViewAreaFocusChanged(bool hasFocus);

private:
    Q_DECLARE_PUBLIC(MultiViewAreas)

private:
    QVector<TabbedViews*> mViewAreaList;
    QSplitter* mMainSplitter;

    TabbedViews* mCurrentViewArea = nullptr;
    TabbedViews* mCurrentInlineToolViewArea = nullptr;
};

inline QWidget* MultiViewAreasPrivate::widget()                 const { return mMainSplitter; }
inline bool MultiViewAreasPrivate::hasFocus()                   const { return mCurrentViewArea->hasFocus(); }
inline AbstractView* MultiViewAreasPrivate::viewFocus()         const { return mCurrentViewArea->viewFocus(); }
inline AbstractViewArea* MultiViewAreasPrivate::viewAreaFocus() const { return mCurrentViewArea; }
inline int MultiViewAreasPrivate::viewAreasCount()              const { return mViewAreaList.count(); }

inline QVector<AbstractView*> MultiViewAreasPrivate::viewList() const
{
    QVector<AbstractView*> result;

    result.reserve(mViewAreaList.size());
    for (const TabbedViews* viewArea : mViewAreaList) {
        result.append(viewArea->viewList());
    }

    return result;
}

inline int MultiViewAreasPrivate::viewCount() const
{
    int result = 0;

    for (const TabbedViews* viewArea : mViewAreaList) {
        result += viewArea->viewCount();
    }

    return result;
}

inline int MultiViewAreasPrivate::indexOf(AbstractView* view) const
{
    int result = -1;

    int globalBaseIndex = 0;
    for (const TabbedViews* viewArea : mViewAreaList) {
        const int localIndexOf = viewArea->indexOf(view);
        if (localIndexOf != -1) {
            result = globalBaseIndex + localIndexOf;
            break;
        }
        globalBaseIndex += viewArea->viewCount();
    }

    return result;
}

inline void MultiViewAreasPrivate::setFocus()
{
    mCurrentViewArea->setFocus();
}

inline void MultiViewAreasPrivate::addViews(const QVector<AbstractView*>& views)
{
    Q_Q(MultiViewAreas);

    mCurrentViewArea->addViews(views);

    emit q->added(views);
}

inline void MultiViewAreasPrivate::removeViews(const QVector<AbstractView*>& views)
{
    Q_Q(MultiViewAreas);

    // TODO: possible to just send the views of the given area?
    for (TabbedViews* viewArea : qAsConst(mViewAreaList)) {
        viewArea->removeViews(views);
    }

    // TODO: above might trigger removal of areas before, is this a problem?
    emit q->removing(views);
}

inline void MultiViewAreasPrivate::setCurrentToolInlineView(AbstractToolInlineView* view)
{
    if (mCurrentInlineToolViewArea && mCurrentInlineToolViewArea != mCurrentViewArea) {
        mCurrentInlineToolViewArea->setCurrentToolInlineView(nullptr);
    }

    mCurrentInlineToolViewArea = mCurrentViewArea;

    mCurrentViewArea->setCurrentToolInlineView(view);
}

inline AbstractToolInlineView* MultiViewAreasPrivate::currentToolInlineView() const
{
    return mCurrentInlineToolViewArea->currentToolInlineView();
}

inline void MultiViewAreasPrivate::setViewFocus(AbstractView* view)
{
    // TODO: makes this more efficient!
    for (TabbedViews* viewArea : qAsConst(mViewAreaList)) {
        const int localIndex = viewArea->indexOf(view);
        if (localIndex != -1) {
            viewArea->setViewFocus(view);
            break;
        }
    }
}

// TODO: this method could be removed, as it is the same as _viewArea->setFocus(), or?
inline void MultiViewAreasPrivate::setViewAreaFocus(AbstractViewArea* _viewArea)
{
    auto* viewArea = static_cast<TabbedViews*>(_viewArea);
    if (viewArea == mCurrentViewArea) {
        return;
    }

    if (mViewAreaList.contains(viewArea)) {
        viewArea->setFocus();
    }
}

inline void MultiViewAreasPrivate::closeViewArea(AbstractViewArea* _viewArea)
{
    Q_Q(MultiViewAreas);

    auto* viewArea = static_cast<TabbedViews*>(_viewArea);

    const QVector<AbstractView*> views = viewArea->viewList();

    emit q->closeRequest(views);
}

}

#endif
