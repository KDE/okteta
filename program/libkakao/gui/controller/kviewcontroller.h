/***************************************************************************
                          kviewcontroller.h  -  description
                             -------------------
    begin                : Mon Oct 15 2007
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


#ifndef KVIEWCONTROLLER_H
#define KVIEWCONTROLLER_H


// lib
#include "kcontroller.h"

class KAbstractView;

// TODO: rename to Controllet?
class KViewController : public KController
{
  public: // API to be implemented
    // TODO: rename to setFocus or create FocusSwitchEvent system
    virtual void setView( KAbstractView * ) = 0;
};

#endif
