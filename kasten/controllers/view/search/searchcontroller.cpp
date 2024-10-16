/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2006-2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "searchcontroller.hpp"

// controller
#include "searchdialog.hpp"
#include "searchtool.hpp"
// Kasten core
#include <Kasten/AbstractUserMessagesHandler>
#include <Kasten/UserNotification>
// KF
#include <KXMLGUIClient>
#include <KLocalizedString>
#include <KActionCollection>
#include <KStandardAction>
#include <KMessageBox>
// Qt
#include <QAction>

namespace Kasten {

// TODO: for docked widgets signal widgets if embedded or floating, if horizontal/vertical
SearchController::SearchController(KXMLGUIClient* guiClient,
                                   AbstractUserMessagesHandler* userMessagesHandler, QWidget* parentWidget)
    : m_userMessagesHandler(userMessagesHandler)
    , mParentWidget(parentWidget)
{
    mFindAction     = KStandardAction::find(    this, &SearchController::find,         this);
    mFindNextAction = KStandardAction::findNext(this, &SearchController::findNext,     this);
    mFindPrevAction = KStandardAction::findPrev(this, &SearchController::findPrevious, this);

    guiClient->actionCollection()->addActions({
        mFindAction,
        mFindNextAction,
        mFindPrevAction
    });


    mTool = std::make_unique<SearchTool>();
    mTool->setUserQueryAgent(this);

    connect(mTool.get(), &SearchTool::isApplyableChanged,
            mFindAction, &QAction::setEnabled);
    connect(mTool.get(), &SearchTool::isApplyableChanged,
            mFindNextAction, &QAction::setEnabled);
    connect(mTool.get(), &SearchTool::isApplyableChanged,
            mFindPrevAction, &QAction::setEnabled);

    connect(mTool.get(), &SearchTool::dataNotFound, this, &SearchController::onDataNotFound);

    mFindAction->setEnabled(false);
    mFindNextAction->setEnabled(false);
    mFindPrevAction->setEnabled(false);
}

SearchController::~SearchController() = default;

void SearchController::setTargetModel(AbstractModel* model)
{
    mTool->setTargetModel(model);
}

void SearchController::find()
{
    // ensure dialog
    if (!mSearchDialog) {
        mSearchDialog = std::make_unique<SearchDialog>(mTool.get(), mParentWidget);
    }

    mSearchDialog->show();
}

void SearchController::findNext()
{
    if (mTool->searchData().isEmpty()) {
        showDialog(FindForward);
    } else {
        mTool->search(FindForward, true, false);
    }
    ;
}

void SearchController::findPrevious()
{
    if (mTool->searchData().isEmpty()) {
        showDialog(FindBackward);
    } else {
        mTool->search(FindBackward, true, false);
    }
}

void SearchController::showDialog(FindDirection direction)
{
    // ensure dialog
    if (!mSearchDialog) {
        mSearchDialog = std::make_unique<SearchDialog>(mTool.get(), mParentWidget);
    }

    mSearchDialog->setDirection(direction);

    mSearchDialog->show();
}

void SearchController::onDataNotFound()
{
    const QString messageBoxTitle = i18nc("@title:window", "Find");
    const QString messageText = i18nc("@info", "Search key not found in byte array.");
    auto message = std::make_unique<Kasten::UserNotification>(mTool->targetModel(), messageText, messageBoxTitle);
    m_userMessagesHandler->postNotification(std::move(message));
}

bool SearchController::queryContinue(FindDirection direction) const
{
    const QString messageBoxTitle = i18nc("@title:window", "Find");
    const QString question = (direction == FindForward) ?
                             xi18nc("@info", "End of byte array reached.<nl/>Continue from the beginning?") :
                             xi18nc("@info", "Beginning of byte array reached.<nl/>Continue from the end?");

    const int answer = KMessageBox::questionTwoActions(mParentWidget,
                                                       question, messageBoxTitle,
                                                       KStandardGuiItem::cont(),
                                                       KStandardGuiItem::cancel());

    const bool result = (answer != KMessageBox::SecondaryAction);

    return result;
}

}

#include "moc_searchcontroller.cpp"
