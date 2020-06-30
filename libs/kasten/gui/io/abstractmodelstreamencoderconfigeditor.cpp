/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "abstractmodelstreamencoderconfigeditor.hpp"

namespace Kasten {

class AbstractModelStreamEncoderConfigEditorPrivate
{
};


AbstractModelStreamEncoderConfigEditor::AbstractModelStreamEncoderConfigEditor(QWidget* parent)
    : QWidget(parent)
{}

AbstractModelStreamEncoderConfigEditor::~AbstractModelStreamEncoderConfigEditor() = default;

bool AbstractModelStreamEncoderConfigEditor::isValid() const
{
    return true;
}

AbstractSelectionView* AbstractModelStreamEncoderConfigEditor::createPreviewView() const
{
    return nullptr;
}

}
