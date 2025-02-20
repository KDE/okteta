/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2006, 2009, 2011 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "bytearrayvalidator.hpp"

// lib
#include "bytearraychar8stringvalidator.hpp"
#include "bytearrayutf8stringvalidator.hpp"
#include "bytearrayvaluestringvalidator.hpp"

namespace Okteta {

ByteArrayValidator::ByteArrayValidator(QObject* parent)
    : QValidator(parent)
    , m_stringValidator(new ByteArrayValueStringValidator(ByteArrayValueStringValidator::HexadecimalCoding))
    , mCodecId(HexadecimalCoding)
{
}

ByteArrayValidator::~ByteArrayValidator()
{
    delete m_stringValidator;
}

void ByteArrayValidator::setCharCodec(const QString& charCodecName)
{
    if (m_charCodecName == charCodecName) {
        return;
    }

    m_charCodecName = charCodecName;

    if (mCodecId == CharCoding) {
        static_cast<ByteArrayChar8StringValidator*>(m_stringValidator)->setCharCodec(m_charCodecName);
        emit changed();
    }
}

void ByteArrayValidator::setCodec(Coding codecId)
{
    if (codecId == mCodecId) {
        return;
    }

    const int oldMinLength = m_stringValidator->minLength();
    const int oldMaxLength = m_stringValidator->maxLength();

    const bool isAnotherValueCoding =
        (mCodecId != CharCoding) && (mCodecId != Utf8Coding) &&
        (codecId != CharCoding) && (codecId != Utf8Coding);

    if (!isAnotherValueCoding) {
        delete m_stringValidator;
    }

    mCodecId = codecId;

    if (mCodecId == CharCoding) {
        auto* char8StringValidator = new ByteArrayChar8StringValidator();
        char8StringValidator->setCharCodec(m_charCodecName);
        m_stringValidator = char8StringValidator;
    } else if (mCodecId == Utf8Coding) {
        m_stringValidator = new ByteArrayUtf8StringValidator();
    } else {
        if (isAnotherValueCoding) {
            static_cast<ByteArrayValueStringValidator*>(m_stringValidator)->setCoding(static_cast<ByteArrayValueStringValidator::Coding>(codecId));
        } else {
            auto* valueStringValidator = new ByteArrayValueStringValidator(static_cast<ByteArrayValueStringValidator::Coding>(codecId));
            m_stringValidator = valueStringValidator;
        }
    }
    if (!isAnotherValueCoding) {
        m_stringValidator->setMinLength(oldMinLength);
        m_stringValidator->setMaxLength(oldMaxLength);
    }
}

int ByteArrayValidator::maxLength() const
{
    return m_stringValidator->maxLength();
}

int ByteArrayValidator::minLength() const
{
    return m_stringValidator->minLength();
}

void ByteArrayValidator::setMaxLength(int maxLength)
{
    if (m_stringValidator->maxLength() == maxLength) {
        return;
    }

    m_stringValidator->setMaxLength(maxLength);

    emit changed();
}

void ByteArrayValidator::setMinLength(int minLength)
{
    if (m_stringValidator->minLength() == minLength) {
        return;
    }

    m_stringValidator->setMinLength(minLength);

    emit changed();
}

QValidator::State ByteArrayValidator::validate(QString& string, int& pos) const
{
    const State result = m_stringValidator->validate(string, pos);

    return result;
}

QByteArray ByteArrayValidator::toByteArray(const QString& string) const
{
    const QByteArray result = m_stringValidator->toByteArray(string);

    return result;
}

QString ByteArrayValidator::toString(const QByteArray& byteArray) const
{
    const QString result = m_stringValidator->toString(byteArray);

    return result;
}

}

#include "moc_bytearrayvalidator.cpp"
