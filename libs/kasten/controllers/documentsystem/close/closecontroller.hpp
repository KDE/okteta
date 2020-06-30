/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2006-2009, 2011 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_CLOSECONTROLLER_HPP
#define KASTEN_CLOSECONTROLLER_HPP

// Kasten gui
#include <Kasten/AbstractXmlGuiController>

class QAction;
class KXMLGUIClient;

namespace Kasten {

class AbstractDocumentStrategy;
class AbstractDocument;

class CloseController : public AbstractXmlGuiController
{
    Q_OBJECT

public:
    CloseController(AbstractDocumentStrategy* documentStrategy,
                    KXMLGUIClient* guiClient,
                    bool supportMultiple = true);
    ~CloseController() override;

public: // AbstractXmlGuiController API
    void setTargetModel(AbstractModel* model) override;

private Q_SLOTS:
    void close();
    void closeAll();
    void closeAllOther();

    void onDocumentsChanged();

private:
    AbstractDocumentStrategy* mDocumentStrategy;

    AbstractDocument* mDocument = nullptr;
    QAction* mCloseAction;
    QAction* mCloseAllAction;
    QAction* mCloseAllOtherAction;
};

}

#endif
