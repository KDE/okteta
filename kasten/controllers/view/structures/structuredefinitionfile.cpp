/*
    This file is part of the Okteta Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2009, 2010, 2012 Alex Richardson <alex.richardson@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "structuredefinitionfile.hpp"

#include <structureslogging.hpp>

#include <QFile>
#include <QDir>
#include <QStringList>

#include "datatypes/topleveldatainformation.hpp"

#include "parsers/abstractstructureparser.hpp"
#include "parsers/osdparser.hpp"
#include "parsers/scriptfileparser.hpp"

namespace Kasten {

StructureDefinitionFile::StructureDefinitionFile(const StructureMetaData& metaData)
    : mMetaData(metaData)
{
    const QFileInfo tmp(mMetaData.entryPath());
    const QString absoluteDir = tmp.absolutePath();

    const QString categoryId = mMetaData.categoryId();
    if (categoryId == QLatin1String("structure/js")) {
        const QString filename = absoluteDir + QLatin1String("/main.js");
        mParser.reset(new ScriptFileParser(mMetaData.id(), filename));
    } else if (categoryId == QLatin1String("structure")) {
        // by default use main.osd, only if it doesn't exist fall back to old behaviour
        QString filename = absoluteDir + QLatin1String("/main.osd");
        if (!QFile::exists(filename)) {
            filename = absoluteDir + QLatin1Char('/') + mMetaData.id() + QLatin1String(".osd");
        }
        mParser.reset(new OsdParser(mMetaData.id(), filename));
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
