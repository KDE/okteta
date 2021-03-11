/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_MULTIVIEWAREAS_HPP
#define KASTEN_MULTIVIEWAREAS_HPP

// lib
#include "abstractgroupedviews.hpp"
#include "viewareasplitable.hpp"
#include "toolinlineviewable.hpp"

class QMimeData;

namespace Kasten {

class MultiViewAreasPrivate;

class KASTENGUI_EXPORT MultiViewAreas : public AbstractGroupedViews
                                      , public If::ToolInlineViewable
                                      , public If::ViewAreaSplitable
{
    Q_OBJECT
    Q_INTERFACES(
        Kasten::If::ToolInlineViewable
        Kasten::If::ViewAreaSplitable
    )

public:
    explicit MultiViewAreas();

    ~MultiViewAreas() override;

public: // AbstractViewArea API
    void setFocus() override;
    QWidget* widget() const override;
    bool hasFocus() const override;

public: // AbstractGroupedViews API
    void addViews(const QVector<AbstractView*>& views) override;
    void removeViews(const QVector<AbstractView*>& views) override;
    void setViewFocus(AbstractView* view) override;

    QVector<AbstractView*> viewList() const override;
    int viewCount() const override;
    AbstractView* viewFocus() const override;

public: // If::ToolInlineViewable API
    void setCurrentToolInlineView(AbstractToolInlineView* view) override;
    AbstractToolInlineView* currentToolInlineView() const override;

public: // If::ViewAreaSplitable API
    AbstractViewArea* splitViewArea(AbstractViewArea* viewArea, Qt::Orientation orientation) override;
    void closeViewArea(AbstractViewArea* viewArea) override;
    void setViewAreaFocus(AbstractViewArea* viewArea) override;
    AbstractViewArea* viewAreaFocus() const override;
    int viewAreasCount() const override;

Q_SIGNALS: // If::ViewAreaSplitable API
    void viewAreasAdded(const QVector<Kasten::AbstractViewArea*>& viewAreas) override;
    void viewAreasRemoved(const QVector<Kasten::AbstractViewArea*>& viewAreas) override;
    void viewAreaFocusChanged(Kasten::AbstractViewArea* viewArea) override;

Q_SIGNALS:
    void dataOffered(const QMimeData* mimeData, bool& accepted);
    void dataDropped(const QMimeData* mimeData);

private:
    int indexOf(AbstractView* view) const;

private:
    Q_DECLARE_PRIVATE(MultiViewAreas)
};

}

#endif
