/***************************************************************************
                          creatorcontroller.h  -  description
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


#ifndef CREATORCONTROLLER_H
#define CREATORCONTROLLER_H


// kakao
#include <kviewcontroller.h>
// KDE
#include <KUrl>

class KRecentFilesAction;
class KXmlGuiWindow;
class KDocumentCreator;


class CreatorController : public KViewController
{
  Q_OBJECT

  public:
    CreatorController( KDocumentCreator *creator, KXmlGuiWindow *window );
    ~CreatorController();

  public: // KToolet API
    virtual void setView( KAbstractView *view );

  public:
    void setWorkingUrl( const KUrl &url ); // TODO: should be the one from KDocumentCreator?

  public:
    const KUrl &workingUrl() const;

  public Q_SLOTS:
    void createNew();
    void open();
    void openRecent( const KUrl &url );

  protected:
    KDocumentCreator *mCreator;
    KXmlGuiWindow *mMainWindow;

    KUrl mWorkingUrl; // TODO: where to store the working url? in an environment object?
    KRecentFilesAction *mOpenRecentAction;
};

inline const KUrl &CreatorController::workingUrl() const { return mWorkingUrl; }

#endif
