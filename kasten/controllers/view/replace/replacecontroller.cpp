/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2006-2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "replacecontroller.hpp"

// controller
#include "replacedialog.hpp"
#include "replaceprompt.hpp"
#include "replacetool.hpp"
// Kasten core
#include <Kasten/AbstractUserMessagesHandler>
#include <Kasten/UserNotification>
#include <Kasten/UserResponseOption>
#include <Kasten/UserQuery>
// KF
#include <KXMLGUIClient>
#include <KLocalizedString>
#include <KActionCollection>
#include <KStandardAction>
#include <KStandardGuiItem>
// Qt
#include <QAction>

namespace Kasten {

// TODO: for docked widgets signal widgets if embedded or floating, if horizontal/vertical
ReplaceController::ReplaceController(KXMLGUIClient* guiClient, AbstractUserMessagesHandler* userMessagesHandler, QWidget* parentWidget)
    : m_userMessagesHandler(userMessagesHandler)
    , mParentWidget(parentWidget)
{
    mReplaceAction = KStandardAction::replace(this, &ReplaceController::replace, this);

    guiClient->actionCollection()->addAction(mReplaceAction->objectName(), mReplaceAction);

    mTool = std::make_unique<ReplaceTool>();
    mTool->setUserQueryAgent(this);

    connect(mTool.get(), &ReplaceTool::isApplyableChanged,
            mReplaceAction, &QAction::setEnabled);

    connect(mTool.get(), &ReplaceTool::finished, this, &ReplaceController::onFinished);

    mReplaceAction->setEnabled(false);
}

ReplaceController::~ReplaceController() = default;

void ReplaceController::setTargetModel(AbstractModel* model)
{
    mTool->setTargetModel(model);
}

void ReplaceController::replace()
{
    // ensure dialog
    if (!mReplaceDialog) {
        mReplaceDialog = std::make_unique<ReplaceDialog>(mTool.get(), mParentWidget);
    }

    mReplaceDialog->show();
}

void ReplaceController::onFinished(bool previousFound, int noOfReplacements)
{
    if (mReplacePrompt) {
        mReplacePrompt->hide();
    }

    const QString messageBoxTitle = i18nc("@title:window", "Replace");
    QString replacementReport;

    if (!previousFound) {
        replacementReport = i18nc("@info", "Replace pattern not found in byte array.");
    } else {
        replacementReport = (noOfReplacements == 0) ?
                            i18nc("@info", "No replacements made.") :
                            i18ncp("@info", "1 replacement made.", "%1 replacements made.", noOfReplacements);
    }
    auto message = std::make_unique<Kasten::UserNotification>(mTool->targetModel(), replacementReport, messageBoxTitle);
    m_userMessagesHandler->postNotification(std::move(message));
}

void ReplaceController::queryContinue(FindDirection direction, int noOfReplacements)
{
    const QString messageBoxTitle = i18nc("@title:window", "Replace");
    const QString replacementReport = (noOfReplacements == 0) ?
                                      i18nc("@info", "No replacements made.") :
                                      i18ncp("@info", "1 replacement made.", "%1 replacements made.", noOfReplacements);
    const QString question = (direction == FindForward) ?
                             xi18nc("@info", "End of byte array reached.<nl/>Continue from the beginning?") :
                             xi18nc("@info", "Beginning of byte array reached.<nl/>Continue from the end?");

    const QString message = replacementReport + QLatin1String("<br /><br />") + question;

    auto query = std::make_unique<Kasten::UserQuery>(mTool->targetModel(), message, messageBoxTitle);
    const QString continueResponseId = QStringLiteral("continue");
    query->addResponseOption(KStandardGuiItem::cont(), continueResponseId, Kasten::UserResponseDefaultHint);
    query->addResponseOption(KStandardGuiItem::cancel(), QStringLiteral("cancel"), Kasten::UserResponseCancelHint);

    const bool result = (m_userMessagesHandler->executeQuery(std::move(query)) == continueResponseId);

    Q_EMIT queryContinueFinished(result);
}

void ReplaceController::queryReplaceCurrent()
{
    if (!mReplacePrompt) {
        mReplacePrompt = std::make_unique<ReplacePrompt>(mParentWidget);
        connect(mReplacePrompt.get(), &ReplacePrompt::finished,
                this, &ReplaceController::onPromptReply);
    }
    mReplacePrompt->show();
}

void ReplaceController::onPromptReply(ReplaceBehaviour behaviour)
{
    if (behaviour == ReplaceAll || behaviour == CancelReplacing) {
        mReplacePrompt->hide();
    }

    Q_EMIT queryReplaceCurrentFinished(behaviour);
}

}

#include "moc_replacecontroller.cpp"
