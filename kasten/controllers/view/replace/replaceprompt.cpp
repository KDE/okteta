/*
    This file is part of the Okteta Kasten module, made within the KDE community.

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
    auto* dialogButtonBox = new QDialogButtonBox;
    QPushButton* button = dialogButtonBox->addButton(i18nc("@action:button", "Replace &All"),
                                                     QDialogButtonBox::ApplyRole);
    connect(button, &QAbstractButton::clicked, this, &ReplacePrompt::onReplaceAllButton);
    button = dialogButtonBox->addButton(i18nc("@action:button", "&Skip"),
                                        QDialogButtonBox::ApplyRole);
    connect(button, &QAbstractButton::clicked, this, &ReplacePrompt::onSkipButton);
    QPushButton* replaceButton = dialogButtonBox->addButton(i18nc("@action:button", "Replace"),
                                                            QDialogButtonBox::ApplyRole);
    connect(replaceButton, &QAbstractButton::clicked, this, &ReplacePrompt::onReplaceButton);
    button = dialogButtonBox->addButton(QDialogButtonBox::Close);
    connect(button, &QAbstractButton::clicked, this, &ReplacePrompt::onCloseButton);

    // main layout
    auto* layout = new QVBoxLayout;
    layout->addWidget(dialogButtonBox);

    setLayout(layout);
    resize(minimumSize());

    replaceButton->setDefault(true);
}

void ReplacePrompt::onReplaceAllButton()
{
    emit finished(ReplaceAll);
}

void ReplacePrompt::onSkipButton()
{
    emit finished(SkipCurrent);
}

void ReplacePrompt::onReplaceButton()
{
    emit finished(ReplaceCurrent);
}

void ReplacePrompt::onCloseButton()
{
    emit finished(CancelReplacing);
}

}
