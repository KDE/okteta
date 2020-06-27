/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2009, 2011 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_BYTEARRAYCOMBOBOX_P_HPP
#define KASTEN_BYTEARRAYCOMBOBOX_P_HPP

// lib
#include "bytearraycombobox.hpp"
#include "bytearrayvalidator.hpp"
// KF
#include <KComboBox>
// Qt
#include <QByteArray>

namespace Okteta {

class ByteArrayComboBoxPrivate
{
public:
    explicit ByteArrayComboBoxPrivate(ByteArrayComboBox* parent);

public:
    QByteArray byteArray() const;
    int format() const;
    int maxLength() const;
    int minLength() const;

public:
    void init();
    void setByteArray(const QByteArray& byteArray);
    void setCharCodec(const QString& charCodecName);
    void setMaxLength(int maxLength);
    void setMinLength(int minLength);
    void rememberCurrentByteArray();

    void onFormatChanged(int index);
    void onValueEdited(const QString& value);
    void onValueActivated(int index);

private:
    ByteArrayComboBox* const q_ptr;
    Q_DECLARE_PUBLIC(ByteArrayComboBox)

    KComboBox * mFormatComboBox;
    KComboBox* mValueComboBox;

    ByteArrayValidator* mValidator;
};

inline ByteArrayComboBoxPrivate::ByteArrayComboBoxPrivate(ByteArrayComboBox* parent)
    : q_ptr(parent)
{}

inline QByteArray ByteArrayComboBoxPrivate::byteArray() const
{
    return mValidator->toByteArray(mValueComboBox->currentText());
}
inline int ByteArrayComboBoxPrivate::format() const
{
    return mFormatComboBox->currentIndex();
}

}

#endif
