/*
    SPDX-FileCopyrightText: 2009, 2023 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef OKTETA_CHAR8STRINGVALIDATOR_HPP
#define OKTETA_CHAR8STRINGVALIDATOR_HPP

// Qt
#include <QValidator>

namespace Okteta {

class Char8StringParser;

class Char8StringValidator : public QValidator
{
    Q_OBJECT

public:
    explicit Char8StringValidator(const Char8StringParser* parser, QObject* parent = nullptr);

    ~Char8StringValidator() override;

public: // QValidator API
    [[nodiscard]]
    QValidator::State validate(QString& input, int& pos) const override;

private:
    const Char8StringParser* const m_parser;
};


inline Char8StringValidator::Char8StringValidator(const Char8StringParser* parser, QObject* parent)
    : QValidator(parent)
    , m_parser(parser)
{}

inline Char8StringValidator::~Char8StringValidator() = default;

}

#endif
