/*
    This file is part of the Kakao Framework, part of the KDE project.

    Copyright 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) version 3, or any
    later version accepted by the membership of KDE e.V. (or its
    successor approved by the membership of KDE e.V.), which shall
    act as a proxy defined in Section 6 of version 3 of the license.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef OFFERCONTROLLER_H
#define OFFERCONTROLLER_H


// lib
#include <abstractxmlguicontroller.h>

class KDocumentSyncManager;
class AbstractModelSynchronizer;
class KAbstractDocument;
class KToggleAction;
class KXMLGUIClient;


class OfferController : public AbstractXmlGuiController
{
  Q_OBJECT

  public:
    OfferController( KDocumentSyncManager* syncManager, KXMLGUIClient* guiClient );

  public: // AbstractXmlGuiController API
    virtual void setTargetModel( AbstractModel* model );

  protected Q_SLOTS:
    void setOffering( bool offering );
    void onSynchronizerChanged( AbstractModelSynchronizer* synchronizer );

  protected:
    KDocumentSyncManager* mSyncManager;
    KAbstractDocument* mDocument;

    KToggleAction* mSetOfferingAction;
};

#endif
