/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "abstractmodeldatageneratorconfigeditor.hpp"

namespace Kasten {

class AbstractModelDataGeneratorConfigEditorPrivate
{
};


AbstractModelDataGeneratorConfigEditor::AbstractModelDataGeneratorConfigEditor(QWidget* parent)
    : QWidget(parent)
{}

AbstractModelDataGeneratorConfigEditor::~AbstractModelDataGeneratorConfigEditor() = default;

bool AbstractModelDataGeneratorConfigEditor::isValid() const
{
    return true;
}

}
