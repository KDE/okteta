/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    Copyright 2010,2012-2013 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "viewprofileeditdialog.hpp"

// lib
#include "viewprofileedit.hpp"
// Okteta Gui Kasten
#include <kasten/okteta/bytearrayviewprofile.hpp>
// Qt
#include <QVBoxLayout>
#include <QPushButton>
#include <QDialogButtonBox>

namespace Kasten {

ViewProfileEditDialog::ViewProfileEditDialog(QWidget* parent)
    : QDialog(parent)
{
    mViewProfileEdit = new ViewProfileEdit(this);

    // dialog buttons
    QDialogButtonBox* dialogButtonBox = new QDialogButtonBox(QDialogButtonBox::Ok
                                                             | QDialogButtonBox::Cancel);
    connect(dialogButtonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(dialogButtonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    // main layout
    QVBoxLayout* layout = new QVBoxLayout;
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

}
