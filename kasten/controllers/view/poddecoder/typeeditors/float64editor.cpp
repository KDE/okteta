/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "float64editor.hpp"

// Qt
#include <QDoubleValidator>

Float64Editor::Float64Editor(QWidget* parent)
    : QLineEdit(parent)
{
    auto* validator = new QDoubleValidator(this);
    validator->setNotation(QDoubleValidator::ScientificNotation);

    setValidator(validator);
    setClearButtonEnabled(true);
}

Float64Editor::~Float64Editor() = default;

void Float64Editor::setData(Float64 data)
{
    setText(data.toString());
}

Float64 Float64Editor::data() const
{
//     interpretText();
    return text().toDouble();
}
