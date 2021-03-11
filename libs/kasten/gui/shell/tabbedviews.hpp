/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2007, 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_TABBEDVIEWS_HPP
#define KASTEN_TABBEDVIEWS_HPP

// lib
#include "abstractgroupedviews.hpp"
#include "toolinlineviewable.hpp"

class QMimeData;

namespace Kasten {

class TabbedViewsPrivate;

class KASTENGUI_EXPORT TabbedViews : public AbstractGroupedViews
                                   , public If::ToolInlineViewable
{
    Q_OBJECT
    Q_INTERFACES(
        Kasten::If::ToolInlineViewable
    )

protected:
    explicit TabbedViews(TabbedViewsPrivate* d);

public:
    TabbedViews();

    ~TabbedViews() override;

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

public:
    int indexOf(AbstractView* view) const;

Q_SIGNALS:
    void dataOffered(const QMimeData* mimeData, bool& accepted);
    void dataDropped(const QMimeData* mimeData);

private:
    Q_DECLARE_PRIVATE(TabbedViews)
};

}
#endif
