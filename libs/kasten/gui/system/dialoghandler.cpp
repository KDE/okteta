/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2009, 2011 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "dialoghandler.hpp"

// Kasten core
#include <Kasten/AbstractDocument>
// KF
#include <KMessageBox>
#include <KLocalizedString>
// Qt
#include <QUrl>

namespace Kasten {

DialogHandler::DialogHandler(QWidget* widget)
    : mWidget(widget)
{}

DialogHandler::~DialogHandler() = default;

void DialogHandler::setWidget(QWidget* widget) { mWidget = widget; }

Answer DialogHandler::queryOverwrite(const QUrl& url, const QString& title) const
{
    const QString message =
        xi18nc("@info",
               "There is already a file at<nl/><filename>%1</filename>.<nl/>"
               "Overwrite?", url.url());
    const int answer = KMessageBox::warningYesNoCancel(mWidget, message, title,
                                                       KStandardGuiItem::overwrite(),
                                                       KStandardGuiItem::back());

    return (answer == KMessageBox::Yes) ? Overwrite :
           (answer == KMessageBox::No) ?  PreviousQuestion :
                                          Cancel;
}

Answer DialogHandler::queryDiscardOnReload(const AbstractDocument* document, const QString& title) const
{
    const QString message = xi18nc("@info \"%title\" has been modified.",
                                   "There are unsaved modifications to <filename>%1</filename>. "
                                   "They will be lost if you reload the document.<nl/>"
                                   "Do you want to discard them?", document->title());

    const int answer = KMessageBox::warningContinueCancel(mWidget, message, title,
                                                          KStandardGuiItem::discard());

    return (answer == KMessageBox::Cancel) ? Cancel : Discard;
}

Answer DialogHandler::querySaveDiscard(const AbstractDocument* document, const QString& title) const
{
    const QString message = xi18nc("@info \"%title\" has been modified.",
                                   "<filename>%1</filename> has been modified.<nl/>"
                                   "Do you want to save your changes or discard them?", document->title());

    const int answer = KMessageBox::warningYesNoCancel(mWidget, message, title,
                                                       KStandardGuiItem::save(), KStandardGuiItem::discard());

    return (answer == KMessageBox::Yes) ? Save :
           (answer == KMessageBox::No) ?  Discard :
                                          Cancel;
}

Answer DialogHandler::queryDiscard(const AbstractDocument* document, const QString& title) const
{
    const QString message = xi18nc("@info \"%title\" has been modified.",
                                   "<filename>%1</filename> has been modified.<nl/>"
                                   "Do you want to discard your changes?", document->title());

    const int answer = KMessageBox::warningContinueCancel(mWidget, message, title,
                                                          KStandardGuiItem::discard());

    return (answer == KMessageBox::Cancel) ? Cancel : Discard;
}

}
