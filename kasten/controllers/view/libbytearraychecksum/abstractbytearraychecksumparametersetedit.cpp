/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "abstractbytearraychecksumparametersetedit.hpp"

class AbstractByteArrayChecksumParameterSetEditPrivate
{
};

AbstractByteArrayChecksumParameterSetEdit::AbstractByteArrayChecksumParameterSetEdit(QWidget* parent)
    : QWidget(parent)
    , d(new AbstractByteArrayChecksumParameterSetEditPrivate())
{}

AbstractByteArrayChecksumParameterSetEdit::~AbstractByteArrayChecksumParameterSetEdit() = default;

bool AbstractByteArrayChecksumParameterSetEdit::isValid() const { return true; }
