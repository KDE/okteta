/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2006, 2009, 2011 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_BYTEARRAYCOMBOBOX_HPP
#define KASTEN_BYTEARRAYCOMBOBOX_HPP

// lib
#include "oktetakastengui_export.hpp"
// Qt
#include <QWidget>

class QString;
class QByteArray;

namespace Okteta {
class ByteArrayComboBoxPrivate;

class OKTETAKASTENGUI_EXPORT ByteArrayComboBox : public QWidget
{
    Q_OBJECT

public:
    // TODO: same as in kbytearrayvalidator.h, we need a global enum containing both value and char codings...
    // matching Okteta::ValueCoding
    enum Coding
    {
        InvalidCoding = -1,
        HexadecimalCoding = 0,
        DecimalCoding = 1,
        OctalCoding = 2,
        BinaryCoding = 3,
        CharCoding = 4,
        Utf8Coding = 5
    };

public:
    explicit ByteArrayComboBox(QWidget* parent = nullptr);
    ~ByteArrayComboBox() override;

public: // set
    void setByteArray(const QByteArray& byteArray);
    void setCharCodec(const QString& charCodecName);
    void setMaxLength(int maxLength);
    void setMinLength(int minLength);
    void rememberCurrentByteArray();

public: // get
    QByteArray byteArray() const;
    int format() const;
    int maxLength() const;
    int minLength() const;

Q_SIGNALS:
    void byteArrayChanged(const QByteArray& byteArray);
    void formatChanged(int index);

protected:
    const QScopedPointer<ByteArrayComboBoxPrivate> d_ptr;

private:
    Q_DECLARE_PRIVATE(ByteArrayComboBox)
};

}

#endif
