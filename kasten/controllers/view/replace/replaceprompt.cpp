/*
    SPDX-FileCopyrightText: 2006-2007, 2009, 2013 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "replaceprompt.hpp"

// KF
#include <KLocalizedString>
// Qt
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <QPushButton>

namespace Kasten {

ReplacePrompt::ReplacePrompt(QWidget* parent)
    : QDialog(parent)
{
    setModal(true);
    setWindowTitle(i18nc("@title:window prompt for iterative replacement", "Replace"));

    // dialog buttons
    auto* const dialogButtonBox = new QDialogButtonBox;
    QPushButton* const replaceAllButton = dialogButtonBox->addButton(i18nc("@action:button", "Replace &All"),
                                                                     QDialogButtonBox::ApplyRole);
    connect(replaceAllButton, &QAbstractButton::clicked, this, &ReplacePrompt::onReplaceAllButton);

    QPushButton* const skipButton = dialogButtonBox->addButton(i18nc("@action:button", "&Skip"),
                                                               QDialogButtonBox::ApplyRole);
    connect(skipButton, &QAbstractButton::clicked, this, &ReplacePrompt::onSkipButton);

    QPushButton* const replaceButton = dialogButtonBox->addButton(i18nc("@action:button", "Replace"),
                                                                  QDialogButtonBox::ApplyRole);
    connect(replaceButton, &QAbstractButton::clicked, this, &ReplacePrompt::onReplaceButton);

    auto* const closeButton = dialogButtonBox->addButton(QDialogButtonBox::Close);
    connect(closeButton, &QAbstractButton::clicked, this, &ReplacePrompt::onCloseButton);

    // main layout
    auto* const layout = new QVBoxLayout;
    layout->addWidget(dialogButtonBox);

    setLayout(layout);
    resize(minimumSize());

    replaceButton->setDefault(true);
}

void ReplacePrompt::onReplaceAllButton()
{
    Q_EMIT finished(ReplaceAll);
}

void ReplacePrompt::onSkipButton()
{
    Q_EMIT finished(SkipCurrent);
}

void ReplacePrompt::onReplaceButton()
{
    Q_EMIT finished(ReplaceCurrent);
}

void ReplacePrompt::onCloseButton()
{
    Q_EMIT finished(CancelReplacing);
}

}

#include "moc_replaceprompt.cpp"
