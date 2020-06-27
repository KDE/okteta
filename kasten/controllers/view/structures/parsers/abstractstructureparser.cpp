/*
    This file is part of the Okteta Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2010, 2011, 2012 Alex Richardson <alex.richardson@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "abstractstructureparser.hpp"

#include "../structuredefinitionfile.hpp"
#include "../script/scriptlogger.hpp"
#include "../structlogging.hpp"

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
