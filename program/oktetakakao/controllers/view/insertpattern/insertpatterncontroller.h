/***************************************************************************
                          insertpatterncontroller.h  -  description
                             -------------------
    begin                : Thu Nov 23 2006
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


#ifndef INSERTPATTERNCONTROLLER_H
#define INSERTPATTERNCONTROLLER_H


// kakao
#include <kviewcontroller.h>

class KXmlGuiWindow;
class InsertPatternTool;
class InsertPatternDialog;
class QAction;


class InsertPatternController : public KViewController
{
  Q_OBJECT

  public:
    explicit InsertPatternController( KXmlGuiWindow *window );
    virtual ~InsertPatternController();

  public: // KViewController API
    virtual void setView( KAbstractView *view );

  protected Q_SLOTS:
    void onActionTriggered();
    void onViewChanged( bool hasView );

  private Q_SLOTS:
    void onOkClicked();

  protected:
    KXmlGuiWindow *mWindow;

    QAction *mInsertPatternAction;

    InsertPatternTool *mInsertPatternTool;
    InsertPatternDialog *mInsertPatternDialog;
};

#endif
