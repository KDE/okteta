/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2010 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_ABSTRACTTOOLVIEWFACTORY_HPP
#define KASTEN_ABSTRACTTOOLVIEWFACTORY_HPP

// lib
#include "kastengui_export.hpp"
#include "kastengui.hpp"

namespace Kasten {
class AbstractToolView;
class AbstractTool;

class KASTENGUI_EXPORT AbstractToolViewFactory
{
public:
    virtual ~AbstractToolViewFactory();

public: // API to be implemented
    virtual AbstractToolView* create(AbstractTool* tool) const = 0;

    // TODO: find out of iconName an id can be QByteArray
    virtual QString iconName() const = 0;
    virtual QString title() const = 0;
    virtual QString id() const = 0;
    virtual SidePosition defaultPosition() const = 0;
};

inline AbstractToolViewFactory::~AbstractToolViewFactory() = default;

}

#endif
