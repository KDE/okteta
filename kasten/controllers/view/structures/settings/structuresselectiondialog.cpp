/*
 This file is part of the Okteta Kasten Framework, made within the KDE community.

 SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>
 SPDX-FileCopyrightText: 2009, 2012 Alex Richardson <alex.richardson@gmx.de>

 SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "structuresselectiondialog.hpp"

// controller
#include "structureaddremovewidget.hpp"
#include <structureslogging.hpp>
// KF
#include <KLocalizedString>
// Qt
#include <QDialogButtonBox>
#include <QVBoxLayout>

StructuresSelectionDialog::StructuresSelectionDialog(const QMap<QString, Kasten::StructureDefinitionFile*>& structureDefs,
                                                     const StructureEnabledList& enabledList,
                                                     QWidget* parent)
    : QDialog(parent)
{
    setAttribute(Qt::WA_DeleteOnClose, true);

    setWindowTitle(i18nc("@title:window", "Advanced Structures Selection"));

    auto* layout = new QVBoxLayout;

    m_structureAddRemoveWidget = new StructureAddRemoveWidget(structureDefs, enabledList, this);

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

    const QStringList enabledStructures = m_structureAddRemoveWidget->values();
    qCDebug(LOG_KASTEN_OKTETA_CONTROLLERS_STRUCTURES) << "selected " << enabledStructures;

    Q_EMIT structuresAccepted(enabledStructures);
}
