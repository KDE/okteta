/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "octal8editor.hpp"

Octal8Editor::Octal8Editor(QWidget* parent)
    : AbstractValue8Editor(Okteta::OctalCoding, parent)
{
}

Octal8Editor::~Octal8Editor() = default;

void Octal8Editor::setData(Octal8 data)
{
    setValue(data.value);
}

Octal8 Octal8Editor::data() const
{
    return value();
}
