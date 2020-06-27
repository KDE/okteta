/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2006-2009, 2011 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_CREATORCONTROLLER_HPP
#define KASTEN_CREATORCONTROLLER_HPP

// Kasten gui
#include <Kasten/AbstractXmlGuiController>

class KXMLGUIClient;

namespace Kasten {

class ModelCodecManager;
class AbstractDocumentStrategy;

class CreatorController : public AbstractXmlGuiController
{
    Q_OBJECT

public:
    CreatorController(ModelCodecManager* modelCodecManager,
                      AbstractDocumentStrategy* documentStrategy,
                      KXMLGUIClient* guiClient);
    ~CreatorController() override;

public: // AbstractXmlGuiController API
    void setTargetModel(AbstractModel* model) override;

private Q_SLOTS:
    void onNewActionTriggered();
    void onNewFromClipboardActionTriggered();
    void onNewFromGeneratorActionTriggered();

private:
    ModelCodecManager* mModelCodecManager;
    AbstractDocumentStrategy* mDocumentStrategy;
};

}

#endif
