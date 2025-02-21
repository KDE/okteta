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
    , m_stringValidator(std::make_unique<ByteArrayValueStringValidator>(ByteArrayValueStringValidator::HexadecimalCoding))
{
}

ByteArrayValidator::~ByteArrayValidator() = default;

void ByteArrayValidator::setCharCodec(const QString& charCodecName)
{
    if (m_charCodecName == charCodecName) {
        return;
    }

    m_charCodecName = charCodecName;

    if (mCodecId == CharCoding) {
        static_cast<ByteArrayChar8StringValidator*>(m_stringValidator.get())->setCharCodec(m_charCodecName);
        Q_EMIT changed();
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

    mCodecId = codecId;

    if (mCodecId == CharCoding) {
        auto char8StringValidator = std::make_unique<ByteArrayChar8StringValidator>();
        char8StringValidator->setCharCodec(m_charCodecName);
        m_stringValidator = std::move(char8StringValidator);
    } else if (mCodecId == Utf8Coding) {
        m_stringValidator = std::make_unique<ByteArrayUtf8StringValidator>();
    } else {
        if (isAnotherValueCoding) {
            static_cast<ByteArrayValueStringValidator*>(m_stringValidator.get())->setCoding(static_cast<ByteArrayValueStringValidator::Coding>(codecId));
        } else {
            m_stringValidator = std::make_unique<ByteArrayValueStringValidator>(static_cast<ByteArrayValueStringValidator::Coding>(codecId));
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

    Q_EMIT changed();
}

void ByteArrayValidator::setMinLength(int minLength)
{
    if (m_stringValidator->minLength() == minLength) {
        return;
    }

    m_stringValidator->setMinLength(minLength);

    Q_EMIT changed();
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
