/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2024 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef OKTETA_ABSTRACTBYTEARRAYSTRINGVALIDATOR_HPP
#define OKTETA_ABSTRACTBYTEARRAYSTRINGVALIDATOR_HPP

// Qt
#include <QValidator>

class QString;
class QByteArray;

namespace Okteta {

class AbstractByteArrayStringValidator
{
public:
    virtual ~AbstractByteArrayStringValidator() = default;

public: // API to implement
    virtual  QValidator::State validate(QString& input, int& pos) const = 0;

    virtual  QByteArray toByteArray(const QString& string) const = 0;
    virtual  QString toString(const QByteArray& byteArray) const = 0;

public:
    /**
     * Sets the maximal length of the edited bytearray to @p maxLength.
     * If @p maxLength is negative, the behaviour is undefined. Default is @c 32767.
     */
    void setMaxLength(int maxLength);
    /**
     * Sets the maximal length of the edited bytearray to @p minLength.
     * If @p minLength is negative, the behaviour is undefined. Default is @c 0.
     */
    void setMinLength(int minLength);

    [[nodiscard]]
    int maxLength() const;
    [[nodiscard]]
    int minLength() const;

private:
    int m_minLength = 0;
    int m_maxLength = 32767;
};

inline int AbstractByteArrayStringValidator::maxLength() const { return m_maxLength; }
inline int AbstractByteArrayStringValidator::minLength() const { return m_minLength; }

inline void AbstractByteArrayStringValidator::setMaxLength(int maxLength)
{
    m_maxLength = maxLength;

    if (maxLength < m_minLength) {
        m_minLength = maxLength;
    }
}

inline void AbstractByteArrayStringValidator::setMinLength(int minLength)
{
    m_minLength = minLength;

    if (minLength > m_maxLength) {
        m_maxLength = minLength;
    }
}


}

#endif
