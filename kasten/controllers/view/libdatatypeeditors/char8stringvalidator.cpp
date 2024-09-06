/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2009, 2023 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "char8stringvalidator.hpp"

// lib
#include "char8stringparser.hpp"

namespace Okteta {

QValidator::State Char8StringValidator::validate(QString& input, int& pos) const
{
    Q_UNUSED(pos)

    const Char8StringParser::CodeState evalResult = m_parser->evaluate(nullptr, input);

    return
        (evalResult == Char8StringParser::CodeAcceptable) ?   QValidator::Acceptable :
        (evalResult == Char8StringParser::CodeIntermediate) ? QValidator::Intermediate :
        /* else */                                            QValidator::Invalid;


}

}

#include "moc_char8stringvalidator.cpp"
