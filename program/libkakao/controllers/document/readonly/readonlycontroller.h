/***************************************************************************
                          readonlycontroller.h  -  description
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


#ifndef READONLYCONTROLLER_H
#define READONLYCONTROLLER_H


// kakao
#include <kviewcontroller.h>

class KAbstractDocument;
class KXmlGuiWindow;
class KToggleAction;

class ReadOnlyController : public KViewController
{
  Q_OBJECT

  public:
    ReadOnlyController( KXmlGuiWindow *window );

  public: // KViewController API
    virtual void setView( KAbstractView *view );

  protected Q_SLOTS: // action slots
    void setReadOnly( bool isReadOnly );

  protected:
    KAbstractDocument *mDocument;

    KToggleAction *mSetReadOnlyAction;
};

#endif
