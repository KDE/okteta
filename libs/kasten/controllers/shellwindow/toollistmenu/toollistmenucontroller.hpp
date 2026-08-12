/*
    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_TOOLLISTMENUCONTROLLER_HPP
#define KASTEN_TOOLLISTMENUCONTROLLER_HPP

// Kasten gui
#include <Kasten/AbstractXmlGuiController>
#include <Kasten/XmlGuiActionListsPlugRequiring>
// Qt
#include <QList>

class KXMLGUIClient;
class QAction;

namespace Kasten {

namespace If {
class WidgetsDockable;
}

class ToolListMenuController : public AbstractXmlGuiController
                             , public If::XmlGuiActionListsPlugRequiring
{
    Q_OBJECT
    Q_INTERFACES(
        Kasten::If::XmlGuiActionListsPlugRequiring
    )

public:
    ToolListMenuController(If::WidgetsDockable* widgetsDockable, KXMLGUIClient* guiClient);

public: // AbstractXmlGuiController API
    void setTargetModel(AbstractModel* model) override;

public: // If::XmlGuiActionListsPlugRequiring API
    void plugActionLists() override;

private:
    void updateActions();
    void plugToolListActionList();

private:
    If::WidgetsDockable* const mWidgetsDockable;
    KXMLGUIClient* const mGuiClient;

    QList<QAction*> mToolActionList;
};

}

#endif
