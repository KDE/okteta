/***************************************************************************
                          overwritemodecontroller.h  -  description
                             -------------------
    begin                : Fri Feb 8 2008
    copyright            : 2008 by Friedrich W. H. Kossebau
    email                : kossebau@kde.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU General Public License           *
 *   version 2 as published by the Free Software Foundation.               *
 *                                                                         *
 ***************************************************************************/


#ifndef OVERWRITEMODECONTROLLER_H
#define OVERWRITEMODECONTROLLER_H


// kakao
#include <kviewcontroller.h>

namespace KHEUI {
class KByteArrayView;
}
class KXmlGuiWindow;
class KToggleAction;

class OverwriteModeController : public KViewController
{
  Q_OBJECT

  public:
    OverwriteModeController( KXmlGuiWindow *window );

  public: // KViewController API
    virtual void setView( KAbstractView *view );

  protected Q_SLOTS: // action slots
    void setOverWrite( bool isOverWrite );

  protected:
    KHEUI::KByteArrayView *mByteArrayView;

    KToggleAction *mSetOverWriteAction;
};

#endif
