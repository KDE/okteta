/*
    This file is part of the Okteta Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2009, 2010, 2012 Alex Richardson <alex.richardson@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "structuredefinitionfile.hpp"

#include "structlogging.hpp"

#include <QFile>
#include <QDir>
#include <QStringList>

#include "datatypes/topleveldatainformation.hpp"

#include "parsers/abstractstructureparser.hpp"
#include "parsers/osdparser.hpp"
#include "parsers/scriptfileparser.hpp"

namespace Kasten {

StructureDefinitionFile::StructureDefinitionFile(const KPluginInfo& info)
    : mPluginInfo(info)
{
    const QFileInfo tmp(info.entryPath());
    const QString absoluteDir = tmp.absolutePath();

    QString category = info.category();
    if (category == QLatin1String("structure/js")) {
        const QString filename = absoluteDir + QLatin1String("/main.js");
        mParser.reset(new ScriptFileParser(mPluginInfo.pluginName(), filename));
    } else if (category == QLatin1String("structure")) {
        // by default use main.osd, only if it doesn't exist fall back to old behaviour
        QString filename = absoluteDir + QLatin1String("/main.osd");
        if (!QFile::exists(filename)) {
            filename = absoluteDir + QLatin1Char('/') + mPluginInfo.pluginName() + QLatin1String(".osd");
        }
        mParser.reset(new OsdParser(mPluginInfo.pluginName(), filename));
    } else {
        qCWarning(LOG_KASTEN_OKTETA_CONTROLLERS_STRUCTURES) << "no valid parser found for plugin category '" << category << "'";
    }
}

StructureDefinitionFile::~StructureDefinitionFile() = default;

KPluginInfo StructureDefinitionFile::pluginInfo() const
{
    return mPluginInfo;
}

bool StructureDefinitionFile::isValid() const
{
    return !mParser.isNull();
}

QVector<TopLevelDataInformation*> StructureDefinitionFile::structures() const
{
    Q_CHECK_PTR(mParser);
    return mParser->parseStructures();
}

TopLevelDataInformation* StructureDefinitionFile::structure(const QString& name) const
{
    Q_CHECK_PTR(mParser);
    const QVector<TopLevelDataInformation*> list = mParser->parseStructures();
    TopLevelDataInformation* ret = nullptr;
    for (auto* info : list) {
        if (info->actualDataInformation()->name() == name) {
            ret = info;
        } else {
            delete info; // we have no use for this element
        }
    }

    if (!ret) {
        qCWarning(LOG_KASTEN_OKTETA_CONTROLLERS_STRUCTURES) << "could not find structure with name=" << name;
    }
    return ret; // not found
}

QStringList StructureDefinitionFile::structureNames() const
{
    Q_CHECK_PTR(mParser);
    return mParser->parseStructureNames();
}

}
