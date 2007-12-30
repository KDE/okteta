/***************************************************************************
                          stringsextractcontroller.h  -  description
                             -------------------
    begin                : Wed Dec 26 2007
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


#ifndef STRINGSEXTRACTCONTROLLER_H
#define STRINGSEXTRACTCONTROLLER_H

// kakao
#include <kviewcontroller.h>

class QAction;
class KXmlGuiWindow;
class KByteArrayDocument;

class StringsExtractDialog;
class StringsExtractTool;

namespace KHEUI {
class KByteArrayView;
}

class StringsExtractController : public KViewController
{
  Q_OBJECT

  public:
    explicit StringsExtractController( KXmlGuiWindow *window );
    virtual ~StringsExtractController();

  public: // KViewController API
    virtual void setView( KAbstractView *view );

  protected Q_SLOTS:
    void showTool();

  protected:
    KXmlGuiWindow *mWindow;

    KHEUI::KByteArrayView *mViewWidget;
    KByteArrayDocument *mDocument;

    QAction *mStringsExtractAction;

    StringsExtractTool *mStringsExtractTool;
    StringsExtractDialog *mStringsExtractDialog;
};

#endif
