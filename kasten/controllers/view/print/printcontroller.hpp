/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2007-2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_PRINTCONTROLLER_HPP
#define KASTEN_PRINTCONTROLLER_HPP

// Kasten gui
#include <Kasten/AbstractXmlGuiController>
// Std
#include <memory>

class QAction;
class KXMLGUIClient;

namespace Kasten {

class PrintTool;
class AbstractUserMessagesHandler;

class PrintController : public AbstractXmlGuiController
{
    Q_OBJECT

public:
    explicit PrintController(KXMLGUIClient* guiClient, AbstractUserMessagesHandler* userMessagesHandler);
    ~PrintController() override;

public: // AbstractXmlGuiController API
    void setTargetModel(AbstractModel* model) override;

public: // TODO: this is a hack to enable the browser extension of the KPart to execute printing, rethink it
    void print();

private:
    QAction* mPrintAction;
    QAction* mPrintPreviewAction;

    const std::unique_ptr<PrintTool> mPrintTool;
};

}

#endif
