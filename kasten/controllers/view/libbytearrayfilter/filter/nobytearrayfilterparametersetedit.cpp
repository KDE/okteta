/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "nobytearrayfilterparametersetedit.hpp"

// KF
#include <KLocalizedString>
// Qt
#include <QLabel>
#include <QVBoxLayout>

const char NoByteArrayFilterParameterSetEdit::Id[] = "None";

NoByteArrayFilterParameterSetEdit::NoByteArrayFilterParameterSetEdit(QWidget* parent)
    : AbstractByteArrayFilterParameterSetEdit(parent)
{
    auto* baseLayout = new QVBoxLayout(this);
    baseLayout->setContentsMargins(0, 0, 0, 0);

    auto* label = new QLabel(i18nc("@info", "No parameters."), this);
    label->setEnabled(false);
    baseLayout->addWidget(label, 0, Qt::AlignCenter);
}

NoByteArrayFilterParameterSetEdit::~NoByteArrayFilterParameterSetEdit() = default;

void NoByteArrayFilterParameterSetEdit::setValues(const AbstractByteArrayFilterParameterSet* parameterSet)
{
    Q_UNUSED(parameterSet)
}

void NoByteArrayFilterParameterSetEdit::getParameterSet(AbstractByteArrayFilterParameterSet* parameterSet) const
{
    Q_UNUSED(parameterSet)
}

#include "moc_nobytearrayfilterparametersetedit.cpp"
