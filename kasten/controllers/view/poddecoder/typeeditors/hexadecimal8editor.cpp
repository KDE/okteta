/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "hexadecimal8editor.hpp"

Hexadecimal8Editor::Hexadecimal8Editor(QWidget* parent)
    : AbstractValue8Editor(Okteta::HexadecimalCoding, parent)
{
}

Hexadecimal8Editor::~Hexadecimal8Editor() = default;

void Hexadecimal8Editor::setData(Hexadecimal8 data)
{
    setValue(data.value);
}

Hexadecimal8 Hexadecimal8Editor::data() const
{
    return value();
}
