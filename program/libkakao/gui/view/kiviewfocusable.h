/***************************************************************************
                          kiviewfocusable.h  -  description
                             -------------------
    begin                : Wed Oct 10 2007
    copyright            : 2007 by Friedrich W. H. Kossebau
    email                : kossebau@kde.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU General Public License           *
 *   version 2 as published by the Free Software Foundation.               *
 *                                                                         *
 ***************************************************************************/


#ifndef KDE_IF_VIEWFOCUSABLE_H
#define KDE_IF_VIEWFOCUSABLE_H

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
