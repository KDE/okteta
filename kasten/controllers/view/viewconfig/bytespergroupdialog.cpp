/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2010, 2014 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "bytespergroupdialog.hpp"

// KF
#include <KLocalizedString>
// Qt
#include <QFormLayout>
#include <QVBoxLayout>
#include <QSpinBox>
#include <QDialogButtonBox>

namespace Kasten {

BytesPerGroupDialog::BytesPerGroupDialog(QWidget* parent)
    : QDialog(parent)
{
    setAttribute(Qt::WA_DeleteOnClose, true);

    auto* pageLayout = new QFormLayout();

    mGroupedBytesCountEdit = new QSpinBox(this);
    mGroupedBytesCountEdit->setRange(0, INT_MAX);
    const QString noGroupingText = i18nc("@label",
                                         "No grouping.");
    mGroupedBytesCountEdit->setSpecialValueText(noGroupingText);
    const QString groupedBytesCountLabel =
        i18nc("@label:spinbox number of bytes which are grouped",
              "Bytes per Group:");
    pageLayout->addRow(groupedBytesCountLabel, mGroupedBytesCountEdit);

    auto* dialogButtonBox = new QDialogButtonBox;
    dialogButtonBox->setStandardButtons(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(dialogButtonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(dialogButtonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    auto* layout = new QVBoxLayout;

    layout->addLayout(pageLayout);
    layout->addWidget(dialogButtonBox);
    setLayout(layout);

    const QString caption =
        i18nc("@title:window", "Bytes per Group");
    setWindowTitle(caption);

    connect(this, &QDialog::finished, this, &BytesPerGroupDialog::onFinished);
}

BytesPerGroupDialog::~BytesPerGroupDialog() = default;

int BytesPerGroupDialog::groupedBytesCount() const { return mGroupedBytesCountEdit->value(); }

void BytesPerGroupDialog::setGroupedBytesCount(int groupedBytesCount)
{
    mGroupedBytesCountEdit->setValue(groupedBytesCount);
}

void BytesPerGroupDialog::onFinished(int result)
{
    if (result != QDialog::Accepted) {
        return;
    }

    emit bytesPerGroupAccepted(groupedBytesCount());
}

}
