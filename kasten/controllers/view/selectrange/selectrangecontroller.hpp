/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_SELECTRANGECONTROLLER_HPP
#define KASTEN_SELECTRANGECONTROLLER_HPP

// Kasten gui
#include <Kasten/AbstractXmlGuiController>

class QAction;
class KXMLGUIClient;

namespace Kasten {

class SelectRangeToolView;
class SelectRangeTool;
namespace If {
class ToolInlineViewable;
}

class SelectRangeController : public AbstractXmlGuiController
{
    Q_OBJECT

public:
    explicit SelectRangeController(If::ToolInlineViewable* toolInlineViewable, KXMLGUIClient* guiClient);
    ~SelectRangeController() override;

public: // AbstractXmlGuiController API
    void setTargetModel(AbstractModel* model) override;

private Q_SLOTS: // action slots
    void select();

private:
    QAction* mSelectAction;

    If::ToolInlineViewable* mToolInlineViewable;

    SelectRangeToolView* mView;
    SelectRangeTool* mTool;
};

}

#endif
