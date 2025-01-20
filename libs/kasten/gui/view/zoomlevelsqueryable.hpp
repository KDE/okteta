/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2024 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_IF_ZOOMLEVELSQUERYABLE_HPP
#define KASTEN_IF_ZOOMLEVELSQUERYABLE_HPP

// Qt
#include <QtPlugin>

namespace Kasten {

namespace If {

class ZoomLevelsQueryable
{
public:
    virtual ~ZoomLevelsQueryable();

public: // get
    [[nodiscard]]
    virtual int zoomInLevelsSize() const = 0;
    [[nodiscard]]
    virtual int zoomOutLevelsSize() const = 0;
    [[nodiscard]]
    virtual double zoomScaleForLevel(int zoomLevel) const = 0;
    [[nodiscard]]
    virtual int zoomLevelForScale(double zoomScale) const = 0;

public: // signal
    virtual void zoomLevelsChanged() = 0;
};

inline ZoomLevelsQueryable::~ZoomLevelsQueryable() = default;

}
}

Q_DECLARE_INTERFACE(Kasten::If::ZoomLevelsQueryable, "org.kde.kasten.if.zoomlevelsqueryable/1.0")

#endif
