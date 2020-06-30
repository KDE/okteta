/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2006-2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_GOTOOFFSETCONTROLLER_HPP
#define KASTEN_GOTOOFFSETCONTROLLER_HPP

// Kasten gui
#include <Kasten/AbstractXmlGuiController>

class QAction;
class KXMLGUIClient;

namespace Kasten {

class GotoOffsetToolView;
class GotoOffsetTool;
namespace If {
class ToolInlineViewable;
}

class GotoOffsetController : public AbstractXmlGuiController
{
    Q_OBJECT

public:
    explicit GotoOffsetController(If::ToolInlineViewable* toolInlineViewable, KXMLGUIClient* guiClient);
    ~GotoOffsetController() override;

public: // AbstractXmlGuiController API
    void setTargetModel(AbstractModel* model) override;

private Q_SLOTS: // action slots
    void gotoOffset();

private:
    QAction* mGotoOffsetAction;

    If::ToolInlineViewable* mToolInlineViewable;

    GotoOffsetToolView* mView;
    GotoOffsetTool* mTool;
};

}

#endif
