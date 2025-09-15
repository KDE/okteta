/*
    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "char8editor.hpp"

// lib
#include "char8stringvalidator.hpp"
// Okteta core
#include <Okteta/Character>
#include <Okteta/CharCodec>


namespace Okteta {

Char8Editor::Char8Editor(const CharCodec* charCodec, QWidget* parent)
    : QLineEdit(parent)
    , m_parser(charCodec)
{
    setValidator(new Char8StringValidator(&m_parser, this));
    setClearButtonEnabled(true);
}

Char8Editor::~Char8Editor() = default;

void Char8Editor::setByte(quint8 byte)
{
    const Okteta::Character decodedChar = m_parser.charCodec()->decode(byte);
    setData(decodedChar);
}

void Char8Editor::setData(Char8 data)
{
    setText(data.character.isUndefined() ? QString() : data.toString());
}

std::optional<quint8> Char8Editor::byte() const
{
    const Okteta::Character character = data().character;

    bool success = (!character.isUndefined());
    Okteta::Byte byte;

    if (success) {
        success = m_parser.charCodec()->encode(&byte, character);
    }
    if (success) {
        return {byte};
    }
    return {};
}

Char8 Char8Editor::data() const
{
    QChar c;
    const Char8StringParser::CodeState evalResult = m_parser.evaluate(&c, text());
    if (evalResult != Char8StringParser::CodeAcceptable) {
        return {QChar(0)};
    }

    return {c};
}

}

#include "moc_char8editor.cpp"
