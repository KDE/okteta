/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2006, 2009, 2011 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "bytearraycombobox.hpp"
#include "bytearraycombobox_p.hpp"

namespace Okteta {

ByteArrayComboBox::ByteArrayComboBox(QWidget* parent)
    : QWidget(parent)
    , d_ptr(new ByteArrayComboBoxPrivate(this))
{
    Q_D(ByteArrayComboBox);

    d->init();
}

ByteArrayComboBox::~ByteArrayComboBox() = default;

void ByteArrayComboBox::setByteArray(const QByteArray& byteArray)
{
    Q_D(ByteArrayComboBox);

    d->setByteArray(byteArray);
}

void ByteArrayComboBox::setCharCodec(const QString& charCodecName)
{
    Q_D(ByteArrayComboBox);

    d->setCharCodec(charCodecName);
}

void ByteArrayComboBox::setMaxLength(int maxLength)
{
    Q_D(ByteArrayComboBox);

    d->setMaxLength(maxLength);
}

void ByteArrayComboBox::setMinLength(int minLength)
{
    Q_D(ByteArrayComboBox);

    d->setMinLength(minLength);
}

void ByteArrayComboBox::rememberCurrentByteArray()
{
    Q_D(ByteArrayComboBox);

    d->rememberCurrentByteArray();
}

QByteArray ByteArrayComboBox::byteArray() const
{
    Q_D(const ByteArrayComboBox);

    return d->byteArray();
}

int ByteArrayComboBox::format() const
{
    Q_D(const ByteArrayComboBox);

    return d->format();
}

int ByteArrayComboBox::maxLength() const
{
    Q_D(const ByteArrayComboBox);

    return d->maxLength();
}

int ByteArrayComboBox::minLength() const
{
    Q_D(const ByteArrayComboBox);

    return d->minLength();
}

}
