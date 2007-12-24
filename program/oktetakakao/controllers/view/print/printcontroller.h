/***************************************************************************
                          printcontroller.h  -  description
                             -------------------
    begin                : Thu Dec 6 2007
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


#ifndef PRINTCONTROLLER_H
#define PRINTCONTROLLER_H

// kakao
#include <kviewcontroller.h>

class KAction;
class KXmlGuiWindow;
class KByteArrayDocument;


class PrintTool;


class PrintController : public KViewController
{
  Q_OBJECT

  public:
    explicit PrintController( KXmlGuiWindow *window );
    virtual ~PrintController();

  public: // KViewController API
    virtual void setView( KAbstractView *view );

  protected Q_SLOTS: // action slots
    void print();

  protected:
    KXmlGuiWindow *mWindow;
    KByteArrayDocument *mDocument;

    KAction *mPrintAction;

    PrintTool *mPrintTool;
};
#endif
