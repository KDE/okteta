/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_IF_TOOLINLINEVIEWABLE_HPP
#define KASTEN_IF_TOOLINLINEVIEWABLE_HPP

// Qt
#include <QtPlugin>

namespace Kasten {

class AbstractToolInlineView;

namespace If {

class ToolInlineViewable
{
public:
    virtual ~ToolInlineViewable();

public: // set/action
    virtual void setCurrentToolInlineView(AbstractToolInlineView* view) = 0;

public: // get
    virtual AbstractToolInlineView* currentToolInlineView() const = 0;

public: // signal
};

inline ToolInlineViewable::~ToolInlineViewable() = default;

}
}

Q_DECLARE_INTERFACE(Kasten::If::ToolInlineViewable, "org.kde.kasten.if.toolinlineviewable/1.0")

#endif
