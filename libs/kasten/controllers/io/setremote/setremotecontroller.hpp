/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2007-2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_SETREMOTECONTROLLER_HPP
#define KASTEN_SETREMOTECONTROLLER_HPP

// Kasten gui
#include <Kasten/AbstractXmlGuiController>

class QAction;
class KXMLGUIClient;

namespace Kasten {

class AbstractDocument;
class DocumentSyncManager;

class SetRemoteController : public AbstractXmlGuiController
{
    Q_OBJECT

public:
    SetRemoteController(DocumentSyncManager* syncManager, KXMLGUIClient* guiClient);

public: // AbstractXmlGuiController API
    void setTargetModel(AbstractModel* model) override;

private Q_SLOTS:
    void saveAs();

private:
    QAction* mSaveAsAction;

    DocumentSyncManager* mSyncManager;
    AbstractDocument* mDocument;
};

}

#endif
