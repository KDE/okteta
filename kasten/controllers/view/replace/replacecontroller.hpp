/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2006-2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_REPLACECONTROLLER_HPP
#define KASTEN_REPLACECONTROLLER_HPP

// controller
#include "replaceuserqueryable.hpp"
// libfinddialog
#include "finddirection.hpp"
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

    void onPromptReply(Kasten::ReplaceBehaviour behaviour);
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
