/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    Copyright 2006-2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) version 3, or any
    later version accepted by the membership of KDE e.V. (or its
    successor approved by the membership of KDE e.V.), which shall
    act as a proxy defined in Section 6 of version 3 of the license.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef KASTEN_REPLACECONTROLLER_HPP
#define KASTEN_REPLACECONTROLLER_HPP

// controller
#include "replaceuserqueryable.hpp"
// lib
#include <kasten/okteta/finddirection.hpp>
// Kasten gui
#include <Kasten/AbstractXmlGuiController>

class KXMLGUIClient;
class QAction;
class QWidget;

namespace Kasten {

class ReplaceDialog;
class ReplacePrompt;
class ReplaceTool;

class ReplaceController : public AbstractXmlGuiController
                        , public If::ReplaceUserQueryable
{
    Q_OBJECT
    Q_INTERFACES(
        Kasten::If::ReplaceUserQueryable
    )

public:
    ReplaceController(KXMLGUIClient* guiClient, QWidget* parentWidget);

    ~ReplaceController() override;

public: // AbstractXmlGuiController API
    void setTargetModel(AbstractModel* model) override;

public: // If::ReplaceUserQueryable API
    void queryContinue(FindDirection direction, int noOfReplacements) override;
    void queryReplaceCurrent() override;

Q_SIGNALS: // If::ReplaceUserQueryable API
    void queryContinueFinished(bool result) override;
    void queryReplaceCurrentFinished(Kasten::ReplaceBehaviour result) override;

private Q_SLOTS: // action slots
    void replace();

    void onPromptReply(ReplaceBehaviour behaviour);
    void onFinished(bool previousFound, int noOfReplacements);

private:
    QWidget* mParentWidget;

    QAction* mReplaceAction;

    ReplaceDialog* mReplaceDialog = nullptr;
    mutable ReplacePrompt* mReplacePrompt = nullptr;
    ReplaceTool* mTool;
};

}

#endif
