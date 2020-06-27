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
// KF
#include <KXmlGuiWindow>
#include <KLocalizedString>
#include <KActionCollection>
#include <KStandardAction>
#include <KMessageBox>
// Qt
#include <QAction>

namespace Kasten {

// TODO: for docked widgets signal widgets if embedded or floating, if horizontal/vertical
ReplaceController::ReplaceController(KXMLGUIClient* guiClient, QWidget* parentWidget)
    : mParentWidget(parentWidget)
{
    mReplaceAction = KStandardAction::replace(this, &ReplaceController::replace, this);

    guiClient->actionCollection()->addAction(mReplaceAction->objectName(), mReplaceAction);

    mTool = new ReplaceTool();
    mTool->setUserQueryAgent(this);

    connect(mTool, &ReplaceTool::isApplyableChanged,
            mReplaceAction, &QAction::setEnabled);

    connect(mTool, &ReplaceTool::finished, this, &ReplaceController::onFinished);

    mReplaceAction->setEnabled(false);
}

ReplaceController::~ReplaceController()
{
    delete mReplaceDialog;
    delete mReplacePrompt;
    delete mTool;
}

void ReplaceController::setTargetModel(AbstractModel* model)
{
    mTool->setTargetModel(model);
}

void ReplaceController::replace()
{
    // ensure dialog
    if (!mReplaceDialog) {
        mReplaceDialog = new ReplaceDialog(mTool, mParentWidget);
    }

    mReplaceDialog->show();
}

void ReplaceController::onFinished(bool previousFound, int noOfReplacements)
{
    if (mReplacePrompt) {
        mReplacePrompt->hide();
    }

    const QString messageBoxTitle = i18nc("@title:window", "Replace");
    const QString replacementReport = (noOfReplacements == 0) ?
                                      i18nc("@info", "No replacements made.") :
                                      i18ncp("@info", "1 replacement made.", "%1 replacements made.", noOfReplacements);

    if (!previousFound) {
        KMessageBox::sorry(mParentWidget, i18nc("@info", "Replace pattern not found in byte array."), messageBoxTitle);
    } else {
        KMessageBox::information(mParentWidget, replacementReport, messageBoxTitle);
    }
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
    const int answer = KMessageBox::questionYesNo(mParentWidget, message, messageBoxTitle,
                                                  KStandardGuiItem::cont(), KStandardGuiItem::cancel());

    const bool result = (answer != KMessageBox::No);

    emit queryContinueFinished(result);
}

void ReplaceController::queryReplaceCurrent()
{
    if (!mReplacePrompt) {
        mReplacePrompt = new ReplacePrompt(mParentWidget);
        connect(mReplacePrompt, &ReplacePrompt::finished,
                this, &ReplaceController::onPromptReply);
    }
    mReplacePrompt->show();
}

void ReplaceController::onPromptReply(ReplaceBehaviour behaviour)
{
    if (behaviour == ReplaceAll || behaviour == CancelReplacing) {
        mReplacePrompt->hide();
    }

    emit queryReplaceCurrentFinished(behaviour);
}

}
