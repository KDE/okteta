/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "char8editor.hpp"

// lib
#include "char8stringvalidator.hpp"
// Okteta core
#include <Okteta/Character>
#include <Okteta/CharCodec>


Char8Editor::Char8Editor(const Okteta::CharCodec* charCodec, QWidget* parent)
    : QLineEdit(parent)
    , m_parser(charCodec)
{
    setValidator(new Okteta::Char8StringValidator(&m_parser, this));
    setClearButtonEnabled(true);
}

Char8Editor::~Char8Editor() = default;

void Char8Editor::setData(Char8 data)
{
    setText(data.character.isUndefined() ? QString() : data.toString());
}

Char8 Char8Editor::data() const
{
    QChar c;
    const Okteta::Char8StringParser::SyntaxState evalResult = m_parser.evaluate(&c, text());
    if (evalResult != Okteta::Char8StringParser::SyntaxAcceptable) {
        return {QChar(0)};
    }

    return {c};
}

#include "moc_char8editor.cpp"
