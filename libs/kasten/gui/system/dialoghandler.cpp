/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2009, 2011 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "dialoghandler.hpp"

// Kasten core
#include <Kasten/AbstractUserMessagesHandler>
#include <Kasten/UserResponseOption>
#include <Kasten/UserQuery>
#include <Kasten/AbstractDocument>
// KF
#include <KStandardGuiItem>
#include <KLocalizedString>
// Qt
#include <QUrl>

namespace Kasten {

DialogHandler::DialogHandler(AbstractUserMessagesHandler* userMessagesHandler)
    : m_userMessagesHandler(userMessagesHandler)
{}

DialogHandler::~DialogHandler() = default;

Answer DialogHandler::queryOverwrite(const QUrl& url, const QString& title) const
{
    const QString message =
        xi18nc("@info",
               "There is already a file at<nl/><filename>%1</filename>.<nl/>"
               "Overwrite?", url.url());

    // TODO: get document model as context to use here
    auto query = std::make_unique<Kasten::UserQuery>(nullptr, message, title, Kasten::UserQueryWarningSeverity);
    const QString overwriteResponseId = QStringLiteral("overwrite");
    const QString backResponseId = QStringLiteral("back");
    query->addResponseOption(KStandardGuiItem::overwrite(), overwriteResponseId);
    query->addResponseOption(KStandardGuiItem::back(), backResponseId); // make default somehow?
    query->addResponseOption(KStandardGuiItem::cancel(), QStringLiteral("cancel"), UserResponseCancelHint | Kasten::UserResponseDefaultHint);

    const QString reponse = m_userMessagesHandler->executeQuery(std::move(query));
    return
        (reponse == overwriteResponseId) ? Overwrite :
        (reponse == backResponseId) ?      PreviousQuestion :
                                           Cancel;
}

Answer DialogHandler::queryDiscardOnReload(const AbstractDocument* document, const QString& title) const
{
    const QString message = xi18nc("@info \"%title\" has been modified.",
                                   "There are unsaved modifications to <filename>%1</filename>. "
                                   "They will be lost if you reload the document.<nl/>"
                                   "Do you want to discard them?", document->title());

    // TODO: reconsider constness of document
    auto query = std::make_unique<Kasten::UserQuery>(const_cast<AbstractDocument*>(document), message, title, Kasten::UserQueryWarningSeverity);
    const QString discardResponseId = QStringLiteral("discard");
    query->addResponseOption(KStandardGuiItem::discard(), discardResponseId);
    query->addResponseOption(KStandardGuiItem::cancel(), QStringLiteral("cancel"), UserResponseCancelHint | Kasten::UserResponseDefaultHint);

    const QString reponse = m_userMessagesHandler->executeQuery(std::move(query));
    return (reponse == discardResponseId) ? Discard : Cancel;
}

Answer DialogHandler::querySaveDiscard(const AbstractDocument* document, const QString& title) const
{
    const QString message = xi18nc("@info \"%title\" has been modified.",
                                   "<filename>%1</filename> has been modified.<nl/>"
                                   "Do you want to save your changes or discard them?", document->title());

    auto query = std::make_unique<Kasten::UserQuery>(const_cast<AbstractDocument*>(document), message, title, Kasten::UserQueryWarningSeverity);
    const QString saveResponseId = QStringLiteral("save");
    const QString discardResponseId = QStringLiteral("discard");
    query->addResponseOption(KStandardGuiItem::save(), saveResponseId);
    query->addResponseOption(KStandardGuiItem::discard(), discardResponseId);
    query->addResponseOption(KStandardGuiItem::cancel(), QStringLiteral("cancel"), UserResponseCancelHint | Kasten::UserResponseDefaultHint);

    const QString reponse = m_userMessagesHandler->executeQuery(std::move(query));
    return
        (reponse == saveResponseId) ?    Save :
        (reponse == discardResponseId) ? Discard :
                                         Cancel;
}

Answer DialogHandler::queryDiscard(const AbstractDocument* document, const QString& title) const
{
    const QString message = xi18nc("@info \"%title\" has been modified.",
                                   "<filename>%1</filename> has been modified.<nl/>"
                                   "Do you want to discard your changes?", document->title());

    auto query = std::make_unique<Kasten::UserQuery>(const_cast<AbstractDocument*>(document), message, title, Kasten::UserQueryWarningSeverity);
    const QString discardResponseId = QStringLiteral("discard");
    query->addResponseOption(KStandardGuiItem::discard(), discardResponseId);
    query->addResponseOption(KStandardGuiItem::cancel(), QStringLiteral("cancel"), UserResponseCancelHint | Kasten::UserResponseDefaultHint);

    const QString reponse = m_userMessagesHandler->executeQuery(std::move(query));
    return (reponse == discardResponseId) ? Discard : Cancel;
}

}
