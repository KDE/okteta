/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2006 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_IF_ZOOMABLE_HPP
#define KASTEN_IF_ZOOMABLE_HPP

// Qt
#include <QtPlugin>

namespace Kasten {
namespace If {

// TODO: add support for automatic zoom
// enum { Manual, Width, Height, FullSize };
// perhaps zoomLevelChanged needs a flag automatically
class Zoomable
{
public:
    virtual ~Zoomable();

public: // set/action
    virtual void setZoomLevel(double Level) = 0;

public: // get
    virtual double zoomLevel() const = 0;

public: // signal
    virtual void zoomLevelChanged(double Level) = 0;
};

inline Zoomable::~Zoomable() = default;

}
}

Q_DECLARE_INTERFACE(Kasten::If::Zoomable, "org.kde.kasten.if.zoomable/1.0")

#endif
