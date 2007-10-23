/***************************************************************************
                          mainwindow.h  -  description
                             -------------------
    begin                : Fri Jun 2 2006
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


#ifndef OKTETAMAINWINDOW_H
#define OKTETAMAINWINDOW_H


// kmvt
#include <kabstractdocument.h>
#include <kviewcontroller.h>
#include <kiviewfocusable.h>
// KDE
#include <KXmlGuiWindow>
// Qt
#include <QtCore/QList>

class QTabWidget;
class KAbstractView;
class OktetaProgram;


class OktetaMainWindow : public KXmlGuiWindow, public KDE::If::ViewFocusable
{
  Q_OBJECT
   Q_INTERFACES(KDE::If::ViewFocusable)

  public:
    explicit OktetaMainWindow( OktetaProgram *Program );
    virtual ~OktetaMainWindow();

  public:
    void updateControllers( KAbstractView *view );

  public: // KDE::If::ViewFocusable API
    virtual void setViewFocus( KAbstractView *view );
    virtual KAbstractView *viewFocus() const;

  public Q_SLOTS:
    void addView( KAbstractView* );
    void removeView( KAbstractView* View );
    void onTitleChanged( const QString &newTitle );
    void onModifiedChanged( KAbstractDocument::SynchronizationStates newStates );

  Q_SIGNALS:
    virtual void viewFocusChanged( KAbstractView *view );

  protected:
    void setupControllers();

  protected: // KMainWindow API
    virtual bool queryClose();

  private Q_SLOTS:
    void onCurrentChanged( int Index );

  protected:
   QTabWidget *mViewsTab;

   QList<KViewController*> mControllers;
   OktetaProgram* mProgram;
};

#endif
