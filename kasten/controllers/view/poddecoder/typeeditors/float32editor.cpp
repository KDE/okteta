/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "float32editor.hpp"

// Qt
#include <QDoubleValidator>
// C++
#include <limits>

Float32Editor::Float32Editor(QWidget* parent)
    : QLineEdit(parent)
{
    const float floatMax = std::numeric_limits<float>::max();
    const float floatMin = std::numeric_limits<float>::min();

    auto* validator = new QDoubleValidator(this);
    validator->setNotation(QDoubleValidator::ScientificNotation);
    validator->setRange(floatMin, floatMax);

    setValidator(validator);
    setClearButtonEnabled(true);
}

Float32Editor::~Float32Editor() = default;

void Float32Editor::setData(Float32 data)
{
    setText(data.toString());
}

Float32 Float32Editor::data() const
{
//     interpretText();
    return text().toFloat();
}
