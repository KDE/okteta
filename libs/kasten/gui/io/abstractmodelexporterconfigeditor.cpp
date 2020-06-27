/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "abstractmodelexporterconfigeditor.hpp"

namespace Kasten {

class AbstractModelExporterConfigEditorPrivate
{
};


AbstractModelExporterConfigEditor::AbstractModelExporterConfigEditor(QWidget* parent)
    : QWidget(parent)
{}

AbstractModelExporterConfigEditor::~AbstractModelExporterConfigEditor() = default;

bool AbstractModelExporterConfigEditor::isValid() const
{
    return true;
}

AbstractSelectionView* AbstractModelExporterConfigEditor::createPreviewView() const
{
    return nullptr;
}

}
