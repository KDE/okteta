/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2025 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef OKTETA_ISO8859_1STRINGVALIDATOR_HPP
#define OKTETA_ISO8859_1STRINGVALIDATOR_HPP

// Qt
#include <QValidator>

class QTextCodec;

namespace Okteta {

class Iso8859_1StringValidator : public QValidator
{
    Q_OBJECT

public:
    explicit Iso8859_1StringValidator(QObject* parent = nullptr);

    ~Iso8859_1StringValidator() override;

public: // QValidator API
    [[nodiscard]]
    QValidator::State validate(QString& input, int& pos) const override;

private:
    QTextCodec* const m_codec;
};

inline Iso8859_1StringValidator::~Iso8859_1StringValidator() = default;

}

#endif
