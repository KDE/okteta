/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "abstractbytearrayfilterparametersetedit.hpp"

class AbstractByteArrayFilterParameterSetEditPrivate
{
};

AbstractByteArrayFilterParameterSetEdit::AbstractByteArrayFilterParameterSetEdit(QWidget* parent)
    : QWidget(parent)
    , d(new AbstractByteArrayFilterParameterSetEditPrivate())
{}

AbstractByteArrayFilterParameterSetEdit::~AbstractByteArrayFilterParameterSetEdit() = default;

void AbstractByteArrayFilterParameterSetEdit::setCharCodec(const QString& charCodecName) { Q_UNUSED(charCodecName) }
bool AbstractByteArrayFilterParameterSetEdit::isValid() const { return true; }
void AbstractByteArrayFilterParameterSetEdit::rememberCurrentSettings() {}
