/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "nobytearraychecksumparametersetedit.hpp"

// KF
#include <KLocalizedString>
// Qt
#include <QLabel>
#include <QVBoxLayout>

const char NoByteArrayChecksumParameterSetEdit::Id[] = "None";

NoByteArrayChecksumParameterSetEdit::NoByteArrayChecksumParameterSetEdit(QWidget* parent)
    : AbstractByteArrayChecksumParameterSetEdit(parent)
{
    auto* baseLayout = new QVBoxLayout(this);
    baseLayout->setContentsMargins(0, 0, 0, 0);

    auto* label = new QLabel(i18nc("@info", "No parameters."), this);
    label->setEnabled(false);
    baseLayout->addWidget(label, 0, Qt::AlignCenter);
}

NoByteArrayChecksumParameterSetEdit::~NoByteArrayChecksumParameterSetEdit() = default;

void NoByteArrayChecksumParameterSetEdit::setParameterSet(const AbstractByteArrayChecksumParameterSet* parameterSet)
{
    Q_UNUSED(parameterSet)
}

void NoByteArrayChecksumParameterSetEdit::getParameterSet(AbstractByteArrayChecksumParameterSet* parameterSet) const
{
    Q_UNUSED(parameterSet)
}

#include "moc_nobytearraychecksumparametersetedit.cpp"
