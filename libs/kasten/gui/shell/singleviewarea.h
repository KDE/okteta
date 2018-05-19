/*
    This file is part of the Kasten Framework, made within the KDE community.

    Copyright 2011 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#ifndef SINGLEVIEWAREA_H
#define SINGLEVIEWAREA_H

// lib
#include <kasten/abstractviewarea.h>
#include <kasten/toolinlineviewable.h>

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

public:
    void setView(AbstractView* view);

Q_SIGNALS:
    void dataOffered(const QMimeData* mimeData, bool& accepted);
    void dataDropped(const QMimeData* mimeData);

protected:
    Q_DECLARE_PRIVATE(SingleViewArea)
};

}

#endif
