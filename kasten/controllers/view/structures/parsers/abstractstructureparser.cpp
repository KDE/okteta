/*
    SPDX-FileCopyrightText: 2010, 2011, 2012 Alex Richardson <alex.richardson@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "abstractstructureparser.hpp"

// lib
#include <structuredefinitionfile.hpp>
#include <scriptlogger.hpp>
#include <structureslogging.hpp>
// Qt
#include <QFile>

AbstractStructureParser::AbstractStructureParser(const QString& pluginName, const QString& absolutePath)
    : mPluginName(pluginName)
    , mAbsolutePath(absolutePath)
{
    if (!QFile::exists(absolutePath)) {
        qCWarning(LOG_KASTEN_OKTETA_CONTROLLERS_STRUCTURES) << "File" << absolutePath << "does not exist, parsing structure will fail!";
    }
}

AbstractStructureParser::~AbstractStructureParser() = default;
