/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2007-2009, 2012 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_SYNCHRONIZECONTROLLER_HPP
#define KASTEN_SYNCHRONIZECONTROLLER_HPP

// Kasten gui
#include <Kasten/AbstractXmlGuiController>

class QAction;
class KXMLGUIClient;

namespace Kasten {

class AbstractModelSynchronizer;
class AbstractModelFileSystemSynchronizer;
class DocumentSyncManager;
class AbstractDocument;

class SynchronizeController : public AbstractXmlGuiController
{
    Q_OBJECT

public:
    SynchronizeController(DocumentSyncManager* syncManager, KXMLGUIClient* guiClient);

public: // AbstractXmlGuiController API
    void setTargetModel(AbstractModel* model) override;

private Q_SLOTS:
    void save();
    void reload();

private Q_SLOTS:
    void onSynchronizerChanged(Kasten::AbstractModelSynchronizer* newSynchronizer);
    void onSynchronizerDeleted(QObject* synchronizer);
    void onSyncStateChanged();

private:
    DocumentSyncManager* mSyncManager;
    AbstractDocument* mDocument = nullptr;
    AbstractModelFileSystemSynchronizer* mSynchronizer = nullptr;

    QAction* mSaveAction;
    QAction* mReloadAction;
};

}

#endif
