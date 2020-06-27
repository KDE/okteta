/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2010, 2012-2013 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "viewprofileeditdialog.hpp"

// lib
#include "viewprofileedit.hpp"
// Okteta Gui Kasten
#include <Kasten/Okteta/ByteArrayViewProfile>
// Qt
#include <QVBoxLayout>
#include <QPushButton>
#include <QDialogButtonBox>

namespace Kasten {

ViewProfileEditDialog::ViewProfileEditDialog(QWidget* parent)
    : QDialog(parent)
{
    setAttribute(Qt::WA_DeleteOnClose, true);

    mViewProfileEdit = new ViewProfileEdit(this);

    // dialog buttons
    QDialogButtonBox* dialogButtonBox = new QDialogButtonBox(QDialogButtonBox::Ok
                                                             | QDialogButtonBox::Cancel);
    connect(dialogButtonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(dialogButtonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    // main layout
    auto* layout = new QVBoxLayout;
    layout->addWidget(mViewProfileEdit);
    layout->addStretch();
    layout->addWidget(dialogButtonBox);
    setLayout(layout);

    dialogButtonBox->button(QDialogButtonBox::Cancel)->setDefault(true);

    connect(mViewProfileEdit, &ViewProfileEdit::profileTitleChanged,
            this, &ViewProfileEditDialog::onProfileTitleChanged);
    // Disable it by default
    mOkButton = dialogButtonBox->button(QDialogButtonBox::Ok);
    mOkButton->setEnabled(false);

    connect(this, &QDialog::finished, this, &ViewProfileEditDialog::onFinished);
}

ViewProfileEditDialog::~ViewProfileEditDialog() = default;

ByteArrayViewProfile ViewProfileEditDialog::viewProfile() const
{
    ByteArrayViewProfile result = mViewProfileEdit->viewProfile();
    result.setId(mId);
    return result;
}

void ViewProfileEditDialog::setViewProfile(const ByteArrayViewProfile& viewProfile)
{
    mViewProfileEdit->setViewProfile(viewProfile);
    mId = viewProfile.id();
}

void ViewProfileEditDialog::onProfileTitleChanged(const QString& title)
{
    mOkButton->setEnabled(!title.isEmpty());
}

void ViewProfileEditDialog::onFinished(int result)
{
    if (result != QDialog::Accepted) {
        return;
    }

    emit viewProfileAccepted(viewProfile());
}

}
