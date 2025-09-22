/*
    SPDX-FileCopyrightText: 2009, 2010, 2012 Alex Richardson <alex.richardson@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "structuredefinitionfile.hpp"

// lib
#include <topleveldatainformation.hpp>
#include <abstractstructureparser.hpp>
#include <osdparser.hpp>
#include <scriptfileparser.hpp>
#include <structureslogging.hpp>
// Qt
#include <QFile>
#include <QDir>
#include <QStringList>

namespace Kasten {

StructureDefinitionFile::StructureDefinitionFile(const StructureMetaData& metaData)
    : mMetaData(metaData)
{
    const QFileInfo tmp(mMetaData.entryPath());
    const QString absoluteDir = tmp.absolutePath();

    const QString categoryId = mMetaData.categoryId();
    if (categoryId == QLatin1String("structure/js")) {
        const QString filename = absoluteDir + QLatin1String("/main.js");
        mParser = std::make_unique<ScriptFileParser>(mMetaData.id(), filename);
    } else if (categoryId == QLatin1String("structure")) {
        // by default use main.osd, only if it doesn't exist fall back to old behaviour
        QString filename = absoluteDir + QLatin1String("/main.osd");
        if (!QFile::exists(filename)) {
            filename = absoluteDir + QLatin1Char('/') + mMetaData.id() + QLatin1String(".osd");
        }
        mParser = std::make_unique<OsdParser>(mMetaData.id(), filename);
    } else {
        qCWarning(LOG_KASTEN_OKTETA_CONTROLLERS_STRUCTURES) << "no valid parser found for plugin category '" << categoryId << "'";
    }
}

StructureDefinitionFile::~StructureDefinitionFile() = default;

StructureMetaData StructureDefinitionFile::metaData() const
{
    return mMetaData;
}

bool StructureDefinitionFile::isValid() const
{
    return (mParser.get() != nullptr);
}

std::vector<std::unique_ptr<TopLevelDataInformation>> StructureDefinitionFile::structures() const
{
    Q_CHECK_PTR(mParser);
    return mParser->parseStructures();
}

std::unique_ptr<TopLevelDataInformation> StructureDefinitionFile::structure(const QString& name) const
{
    Q_CHECK_PTR(mParser);
    std::vector<std::unique_ptr<TopLevelDataInformation>> list = mParser->parseStructures();
    std::unique_ptr<TopLevelDataInformation> ret;
    for (auto& info : list) {
        if (info->actualDataInformation()->name() == name) {
            ret = std::move(info);
            break;
        }
    }

    if (!ret) {
        qCWarning(LOG_KASTEN_OKTETA_CONTROLLERS_STRUCTURES) << "could not find structure with name=" << name;
    }
    return ret;
}

QStringList StructureDefinitionFile::structureNames() const
{
    Q_CHECK_PTR(mParser);
    return mParser->parseStructureNames();
}

}
