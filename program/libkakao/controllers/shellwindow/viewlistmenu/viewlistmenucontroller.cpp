/***************************************************************************
                          viewlistmenucontroller.cpp  -  description
                             -------------------
    begin                : Tue Nov 30 2006
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


#include "viewlistmenucontroller.h"

// kakao
#include <kabstractdocument.h>
#include <kviewmanager.h>
#include <kiviewfocusable.h>
// KDE
#include <KXmlGuiWindow>
#include <kxmlguifactory.h>
#include <KActionCollection>
#include <KLocale>
#include <KStringHandler>
// Qt
#include <QtGui/QMenu>

static const int MaxEntryLength = 150;
static const char WindowsListActionListId[] = "windows_list";


ViewListMenuController::ViewListMenuController( KViewManager *viewManager, KDE::If::ViewFocusable *focusable,
                                      KXmlGuiWindow *mainWindow )
 : mViewManager( viewManager ), mFocusable( focusable ), mMainWindow( mainWindow )
{
    mWindowsActionGroup = new QActionGroup( this ); // TODO: do we use this only for the signal mapping?
//     mWindowsActionGroup->setExclusive( true );
    connect( mWindowsActionGroup, SIGNAL(triggered( QAction* )), SLOT(onActionTriggered( QAction* )) );

    connect( mViewManager, SIGNAL(opened( KAbstractView* )),  SLOT(updateActions()) );
    connect( mViewManager, SIGNAL(closing( KAbstractView* )), SLOT(updateActions()) );

    setView( 0 );
}

void ViewListMenuController::setView( KAbstractView *View )
{
Q_UNUSED(View)
}

 Q_DECLARE_METATYPE(KAbstractView*)

void ViewListMenuController::updateActions()
{
    mMainWindow->unplugActionList( WindowsListActionListId );

    qDeleteAll( mWindowsActionGroup->actions() );

    const QList<KAbstractView*> views = mViewManager->views();

    //TODO: sortieren nach namen und erste 10 mit Zahl, siehe unten
    for( int v = 0; v < views.size(); ++v )
    {
        KAbstractView *view = views.at( v );
        const QString title = KStringHandler::rsqueeze( view->title(), MaxEntryLength );
        QAction *action = new QAction( v<10 ? QString::fromLatin1("&%1 %2").arg(v).arg(title) : title, mWindowsActionGroup );
//         action->setCheckable( true );

//         if(m_viewManager->activeView() && doc == m_viewManager->activeView()->document())
//             action->setChecked(true);
        action->setData( QVariant::fromValue(view) );
        mWindowsActionGroup->addAction( action );
    }
    mMainWindow->plugActionList( WindowsListActionListId, mWindowsActionGroup->actions() );
}


void ViewListMenuController::onActionTriggered( QAction *action )
{
    KAbstractView *view = action->data().value<KAbstractView *>();
    mFocusable->setViewFocus( view );
}
