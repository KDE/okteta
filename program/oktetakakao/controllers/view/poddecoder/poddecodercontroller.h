/***************************************************************************
                          poddecodercontroller.h  -  description
                             -------------------
    begin                : Fri Nov 3 2006
    copyright            : 2006 by Friedrich W. H. Kossebau
    email                : kossebau@kde.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU General Public License           *
 *   version 2 as published by the Free Software Foundation.               *
 *                                                                         *
 ***************************************************************************/


#ifndef PODDECODERCONTROLLER_H
#define PODDECODERCONTROLLER_H


// kakao
#include <kviewcontroller.h>

class PODDecoderTool;
class KXmlGuiWindow;
class KPrimitiveTypesView;

class PODDecoderController : public KViewController
{
  public:
    PODDecoderController( KXmlGuiWindow *window );
    virtual ~PODDecoderController();

  public: // KViewController API
    virtual void setView( KAbstractView *View );

  protected:
    KXmlGuiWindow *mWindow;

    PODDecoderTool *mTool;
    KPrimitiveTypesView *mPrimitiveTypesView;
};

#endif
