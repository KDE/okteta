/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_TOOLLISTMENUCONTROLLER_HPP
#define KASTEN_TOOLLISTMENUCONTROLLER_HPP

// Kasten gui
#include <Kasten/AbstractXmlGuiController>
#include <Kasten/XmlGuiActionListsReplugRequiring>
// Qt
#include <QList>

class KXMLGUIClient;
class QAction;

namespace Kasten {

namespace If {
class WidgetsDockable;
}

class ToolListMenuController : public AbstractXmlGuiController
                             , public If::XmlGuiActionListsReplugRequiring
{
    Q_OBJECT
    Q_INTERFACES(
        Kasten::If::XmlGuiActionListsReplugRequiring
    )

public:
    ToolListMenuController(If::WidgetsDockable* widgetsDockable, KXMLGUIClient* guiClient);

public: // AbstractXmlGuiController API
    void setTargetModel(AbstractModel* model) override;

public: // If::XmlGuiActionListsReplugRequiring API
    void replugActionLists() override;

private:
    void updateActions();
    void plugToolListActionList();

private:
    If::WidgetsDockable* mWidgetsDockable;
    KXMLGUIClient* mGuiClient;

    QList<QAction*> mToolActionList;
};

}

#endif
