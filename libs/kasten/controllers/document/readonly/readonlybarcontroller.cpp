/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "readonlybarcontroller.hpp"

// Kasten core
#include <Kasten/AbstractDocument>
// Kasten ui
#include <Kasten/ToggleButton>
#include <Kasten/StatusBar>
// KF
#include <KLocalizedString>

namespace Kasten {

ReadOnlyBarController::ReadOnlyBarController(StatusBar* statusBar)
{
    const QString readWriteText = i18nc("@option:check the document is read-write", "Read-write");
    const QString readOnlyText = i18nc("@option:check the document is read-only", "Read-only");
    mReadOnlyButton = new ToggleButton(QIcon::fromTheme(QStringLiteral("object-unlocked")), QString(), readWriteText, statusBar);
    mReadOnlyButton->setCheckedState(QIcon::fromTheme(QStringLiteral("object-locked")), QString(), readOnlyText);
    statusBar->addWidget(mReadOnlyButton);
    connect(mReadOnlyButton, &QAbstractButton::clicked,
            this, &ReadOnlyBarController::setReadOnly);

    setTargetModel(nullptr);
}

void ReadOnlyBarController::setTargetModel(AbstractModel* model)
{
    if (mDocument) {
        mDocument->disconnect(mReadOnlyButton);
    }

    mDocument = model ? model->findBaseModel<AbstractDocument*>() : nullptr;

    if (mDocument) {
        mReadOnlyButton->setChecked(mDocument->isReadOnly());

        connect(mDocument, &AbstractModel::readOnlyChanged,
                mReadOnlyButton, &QAbstractButton::setChecked);
        connect(mDocument, &AbstractModel::modifiableChanged,
                mReadOnlyButton, &QWidget::setEnabled);
    } else {
        mReadOnlyButton->setChecked(false);
    }

    mReadOnlyButton->setEnabled(mDocument ? mDocument->isModifiable() : false);
}

void ReadOnlyBarController::setReadOnly(bool isReadOnly)
{
    mDocument->setReadOnly(isReadOnly);
}

}
