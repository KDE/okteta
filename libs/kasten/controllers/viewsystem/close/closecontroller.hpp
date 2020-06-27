/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2006-2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_CLOSECONTROLLER_HPP
#define KASTEN_CLOSECONTROLLER_HPP

// Kasten gui
#include <Kasten/AbstractXmlGuiController>

class KAction;
class KXMLGUIClient;

namespace Kasten {

class ViewManager;

class CloseController : public AbstractXmlGuiController
{
    Q_OBJECT

public:
    CloseController(ViewManager* viewManager, KXMLGUIClient* guiClient);

public: // AbstractXmlGuiController API
    virtual void setTargetModel(AbstractModel* model);

private Q_SLOTS:
    void close();

private:
    ViewManager* mViewManager;

    AbstractView* mView;
    KAction* mCloseAction;
};

}

#endif
