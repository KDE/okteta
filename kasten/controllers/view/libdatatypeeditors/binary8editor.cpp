/*
    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "binary8editor.hpp"

namespace Okteta {

Binary8Editor::Binary8Editor(QWidget* parent)
    : AbstractValue8Editor(Okteta::BinaryCoding, parent)
{
}

Binary8Editor::~Binary8Editor() = default;

void Binary8Editor::setData(Binary8 data)
{
    setValue(data.value);
}

Binary8 Binary8Editor::data() const
{
    return value();
}

}

#include "moc_binary8editor.cpp"
