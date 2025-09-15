/*
    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "abstractmodelstreamencoderconfigeditor.hpp"

// Kasten
#include <Kasten/AbstractSelectionView>

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

std::unique_ptr<AbstractSelectionView> AbstractModelStreamEncoderConfigEditor::createPreviewView() const
{
    return {};
}

}

#include "moc_abstractmodelstreamencoderconfigeditor.cpp"
