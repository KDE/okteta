/*
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
// Std
#include <memory>

class KXMLGUIClient;
class QAction;
class QWidget;

namespace Kasten {

class AbstractUserMessagesHandler;
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
    ReplaceController(KXMLGUIClient* guiClient, AbstractUserMessagesHandler* userMessagesHandler, QWidget* parentWidget);

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
    AbstractUserMessagesHandler* const m_userMessagesHandler;
    QWidget* const mParentWidget;

    QAction* mReplaceAction;

    // in (reverse) order of destruction
    std::unique_ptr<ReplaceTool> mTool;
    mutable std::unique_ptr<ReplacePrompt> mReplacePrompt;
    std::unique_ptr<ReplaceDialog> mReplaceDialog;
};

}

#endif
