/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "nobytearrayfilterparametersetedit.hpp"

const char NoByteArrayFilterParameterSetEdit::Id[] = "None";

NoByteArrayFilterParameterSetEdit::NoByteArrayFilterParameterSetEdit(QWidget* parent)
    : AbstractByteArrayFilterParameterSetEdit(parent)
{}

NoByteArrayFilterParameterSetEdit::~NoByteArrayFilterParameterSetEdit() = default;

void NoByteArrayFilterParameterSetEdit::setValues(const AbstractByteArrayFilterParameterSet* parameterSet)
{
    Q_UNUSED(parameterSet);
}

void NoByteArrayFilterParameterSetEdit::getParameterSet(AbstractByteArrayFilterParameterSet* parameterSet) const
{
    Q_UNUSED(parameterSet);
}
