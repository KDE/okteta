/***************************************************************************
                          overwriteonlycontroller.h  -  description
                             -------------------
    begin                : Sun Jan 27 2008
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


#ifndef OVERWRITEONLYCONTROLLER_H
#define OVERWRITEONLYCONTROLLER_H


// kakao
#include <kviewcontroller.h>

namespace KHECore {
class KAbstractByteArrayModel;
}
class KXmlGuiWindow;
class KToggleAction;

class OverwriteOnlyController : public KViewController
{
  Q_OBJECT

  public:
    OverwriteOnlyController( KXmlGuiWindow *window );

  public: // KViewController API
    virtual void setView( KAbstractView *view );

  protected Q_SLOTS: // action slots
    void setOverwriteOnly( bool isOverwriteOnly );

  protected:
    KHECore::KAbstractByteArrayModel *mByteArrayModel;

    KToggleAction *mSetOverwriteOnlyAction;
};

#endif
