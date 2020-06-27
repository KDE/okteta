/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_IF_VIEWAREASPLITABLE_HPP
#define KASTEN_IF_VIEWAREASPLITABLE_HPP

// Qt
#include <QtPlugin>

template <typename C> class QVector;

namespace Kasten {

class AbstractViewArea;

namespace If {

// TODO: should split(Qt::Orientation) be a method of AbstractViewArea?
// TODO: split or add? difference in semantics?
// TODO: areas could be tabbed/stacked, too, also recursively. Support that?
// TODO: should views to part/sub-models be restricted to overview of document? No, or?
// TODO: where to decide what to do with the contained views of a area that is closed?
// TODO: where to decide what to do on a split, e.g. add a new view copy of the current one?
class ViewAreaSplitable
{
public:
    virtual ~ViewAreaSplitable();

public: // set/action
    /// returns the new view area
    virtual AbstractViewArea* splitViewArea(AbstractViewArea* viewArea, Qt::Orientation orientation) = 0;
    virtual void closeViewArea(AbstractViewArea* viewArea) = 0;   // TODO: or report success with bool?
    virtual void setViewAreaFocus(AbstractViewArea* viewArea) = 0;

public: // get
    virtual AbstractViewArea* viewAreaFocus() const = 0;
//     virtual QVector<Kasten::AbstractViewArea*> viewAreas() const = 0;
    virtual int viewAreasCount() const = 0;

public: // signal
    virtual void viewAreasAdded(const QVector<Kasten::AbstractViewArea*>& viewAreas) = 0;
    virtual void viewAreasRemoved(const QVector<Kasten::AbstractViewArea*>& viewAreas) = 0;
    virtual void viewAreaFocusChanged(Kasten::AbstractViewArea* viewArea) = 0;
};

inline ViewAreaSplitable::~ViewAreaSplitable() = default;

}
}

Q_DECLARE_INTERFACE(Kasten::If::ViewAreaSplitable, "org.kde.kasten.if.viewareasplitable/1.0")

#endif
