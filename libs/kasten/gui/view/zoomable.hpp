/*
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
// perhaps zoomScaleChanged needs a flag automatically
class Zoomable
{
public:
    virtual ~Zoomable();

public: // set/action
    virtual void setZoomScale(double zoomScale) = 0;

public: // get
    [[nodiscard]]
    virtual double zoomScale() const = 0;

public: // signal
    virtual void zoomScaleChanged(double zoomScale) = 0;
};

inline Zoomable::~Zoomable() = default;

}
}

Q_DECLARE_INTERFACE(Kasten::If::Zoomable, "org.kde.kasten.if.zoomable/1.0")

#endif
