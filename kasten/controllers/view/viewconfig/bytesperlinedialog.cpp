/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2010, 2014 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "bytesperlinedialog.hpp"

// KF
#include <KLocalizedString>
// Qt
#include <QFormLayout>
#include <QVBoxLayout>
#include <QSpinBox>
#include <QDialogButtonBox>

namespace Kasten {

BytesPerLineDialog::BytesPerLineDialog(QWidget* parent)
    : QDialog(parent)
{
    setAttribute(Qt::WA_DeleteOnClose, true);

    auto* pageLayout = new QFormLayout();

    mBytesPerLineEdit = new QSpinBox(this);
    mBytesPerLineEdit->setRange(1, INT_MAX);
    const QString bytesPerLineLabel =
        i18nc("@label:spinbox number of bytes which are shown per line",
              "Bytes per Line:");
    pageLayout->addRow(bytesPerLineLabel, mBytesPerLineEdit);

    auto* dialogButtonBox = new QDialogButtonBox;
    dialogButtonBox->setStandardButtons(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(dialogButtonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(dialogButtonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    auto* layout = new QVBoxLayout;

    layout->addLayout(pageLayout);
    layout->addWidget(dialogButtonBox);
    setLayout(layout);

    const QString caption =
        i18nc("@title:window", "Bytes per Line");
    setWindowTitle(caption);

    connect(this, &QDialog::finished, this, &BytesPerLineDialog::onFinished);
}

BytesPerLineDialog::~BytesPerLineDialog() = default;

int BytesPerLineDialog::bytesPerLine()      const { return mBytesPerLineEdit->value(); }

void BytesPerLineDialog::setBytesPerLine(int bytesPerLine)
{
    mBytesPerLineEdit->setValue(bytesPerLine);
}

void BytesPerLineDialog::onFinished(int result)
{
    if (result != QDialog::Accepted) {
        return;
    }

    emit bytesPerLineAccepted(bytesPerLine());
}

}
