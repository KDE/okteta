/*
 This file is part of the Okteta Kasten Framework, made within the KDE community.

 Copyright 2009 Friedrich W. H. Kossebau <kossebau@kde.org>
 Copyright 2009, 2012 Alex Richardson <alex.richardson@gmx.de>

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

#include "structuresselectiondialog.hpp"

// controller
#include "structureaddremovewidget.hpp"
// KF
#include <KLocalizedString>
// Qt
#include <QDialogButtonBox>
#include <QVBoxLayout>

StructuresSelectionDialog::StructuresSelectionDialog(const QStringList& selected, Kasten::StructuresTool* tool,
                                                     QWidget* parent)
    : QDialog(parent)
{
    setAttribute(Qt::WA_DeleteOnClose, true);

    setWindowTitle(i18nc("@title:window", "Advanced Structures Selection"));

    auto* layout = new QVBoxLayout;

    m_structureAddRemoveWidget = new StructureAddRemoveWidget(selected, tool, this);

    auto* dialogButtonBox = new QDialogButtonBox;
    dialogButtonBox->addButton(QDialogButtonBox::Ok);
    connect(dialogButtonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    dialogButtonBox->addButton(QDialogButtonBox::Cancel);
    connect(dialogButtonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    layout->addWidget(m_structureAddRemoveWidget);
    layout->addWidget(dialogButtonBox);

    setLayout(layout);

    connect(this, &QDialog::finished, this, &StructuresSelectionDialog::onFinished);
}

void StructuresSelectionDialog::onFinished(int result)
{
    if (result != QDialog::Accepted) {
        return;
    }

    emit structuresAccepted(m_structureAddRemoveWidget->values());
}
