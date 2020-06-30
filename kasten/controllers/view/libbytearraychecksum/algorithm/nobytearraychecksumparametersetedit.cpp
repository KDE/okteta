/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "nobytearraychecksumparametersetedit.hpp"

const char NoByteArrayChecksumParameterSetEdit::Id[] = "None";

NoByteArrayChecksumParameterSetEdit::NoByteArrayChecksumParameterSetEdit(QWidget* parent)
    : AbstractByteArrayChecksumParameterSetEdit(parent)
{}

NoByteArrayChecksumParameterSetEdit::~NoByteArrayChecksumParameterSetEdit() = default;

void NoByteArrayChecksumParameterSetEdit::setParameterSet(const AbstractByteArrayChecksumParameterSet* parameterSet)
{
    Q_UNUSED(parameterSet);
}

void NoByteArrayChecksumParameterSetEdit::getParameterSet(AbstractByteArrayChecksumParameterSet* parameterSet) const
{
    Q_UNUSED(parameterSet);
}
