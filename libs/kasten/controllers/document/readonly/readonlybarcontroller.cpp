/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    Copyright 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) version 3, or any
    later version accepted by the membership of KDE e.V. (or its
    successor approved by the membership of KDE e.V.), which shall
    act as a proxy defined in Section 6 of version 3 of the license.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library. If not, see <http://www.gnu.org/licenses/>.
*/

#include "readonlybarcontroller.hpp"

// lib
#include <kasten/abstractdocument.hpp>
// Kasten ui
#include <kasten/togglebutton.hpp>
#include <kasten/statusbar.hpp>
// KF5
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
