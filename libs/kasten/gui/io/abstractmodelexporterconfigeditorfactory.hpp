/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_ABSTRACTMODELEXPORTERCONFIGEDITORFACTORY_HPP
#define KASTEN_ABSTRACTMODELEXPORTERCONFIGEDITORFACTORY_HPP

// lib
#include "kastengui_export.hpp"

namespace Kasten {

class AbstractModelExporterConfigEditor;
class AbstractModelExporter;

class KASTENGUI_EXPORT AbstractModelExporterConfigEditorFactory
{
public:
    virtual ~AbstractModelExporterConfigEditorFactory();

public:
    virtual AbstractModelExporterConfigEditor* tryCreateConfigEditor(AbstractModelExporter* exporter) const = 0;
};

inline AbstractModelExporterConfigEditorFactory::~AbstractModelExporterConfigEditorFactory() = default;

}

#endif
