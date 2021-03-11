/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2011 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_SINGLEVIEWAREA_HPP
#define KASTEN_SINGLEVIEWAREA_HPP

// lib
#include "abstractviewarea.hpp"
#include "toolinlineviewable.hpp"

class QMimeData;

namespace Kasten {

class SingleViewAreaPrivate;
class AbstractView;

class KASTENGUI_EXPORT SingleViewArea : public AbstractViewArea
                                      , public If::ToolInlineViewable
{
    Q_OBJECT
    Q_INTERFACES(
        Kasten::If::ToolInlineViewable
    )

public:
    explicit SingleViewArea();

    ~SingleViewArea() override;

public: // AbstractViewArea API
    void setFocus() override;
    QWidget* widget() const override;
    bool hasFocus() const override;

public: // If::ToolInlineViewable API
    void setCurrentToolInlineView(AbstractToolInlineView* view) override;
    AbstractToolInlineView* currentToolInlineView() const override;

public:
    void setView(AbstractView* view);

Q_SIGNALS:
    void dataOffered(const QMimeData* mimeData, bool& accepted);
    void dataDropped(const QMimeData* mimeData);

private:
    Q_DECLARE_PRIVATE(SingleViewArea)
};

}

#endif
