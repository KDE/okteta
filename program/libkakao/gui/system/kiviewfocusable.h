/*
    This file is part of the Kakao Framework, part of the KDE project.

    Copyright 2007 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#ifndef KDE_IF_VIEWFOCUSABLE_H
#define KDE_IF_VIEWFOCUSABLE_H

// Qt
#include <QtCore/QtPlugin>

class KAbstractView;

namespace KDE
{
namespace If
{

class ViewFocusable
{
  public:
    virtual ~ViewFocusable();

  public: // set/action TODO: FocussedView instead of ViewFocus?
    virtual void setViewFocus( KAbstractView *view ) = 0;

  public: // get
    virtual KAbstractView *viewFocus() const = 0;

  public: // signal
    virtual void viewFocusChanged( KAbstractView *view ) = 0;
};

inline ViewFocusable::~ViewFocusable() {}

}
}

Q_DECLARE_INTERFACE( KDE::If::ViewFocusable, "org.kde.if.viewfocusable/1.0" )

#endif
