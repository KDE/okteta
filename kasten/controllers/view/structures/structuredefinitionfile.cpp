/*
 *   This file is part of the Okteta Kasten Framework, made within the KDE community.
 *
 *   Copyright 2009, 2010, 2012 Alex Richardson <alex.richardson@gmx.de>
 *
 *   This library is free software; you can redistribute it and/or
 *   modify it under the terms of the GNU Lesser General Public
 *   License as published by the Free Software Foundation; either
 *   version 2.1 of the License, or (at your option) version 3, or any
 *   later version accepted by the membership of KDE e.V. (or its
 *   successor approved by the membership of KDE e.V.), which shall
 *   act as a proxy defined in Section 6 of version 3 of the license.
 *
 *   This library is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *   Lesser General Public License for more details.
 *
 *   You should have received a copy of the GNU Lesser General Public
 *   License along with this library. If not, see <http://www.gnu.org/licenses/>.
 */
#include "structuredefinitionfile.h"

#include <QFile>
#include <QDir>
#include <QStringList>

#include <KDebug>
#include "datatypes/topleveldatainformation.h"

#include "parsers/abstractstructureparser.h"
#include "parsers/osdparser.h"
#include "parsers/scriptfileparser.h"

namespace Kasten2
{
StructureDefinitionFile::StructureDefinitionFile(KPluginInfo info)
        : mPluginInfo(info)
{
    QFileInfo tmp(info.entryPath());
    mDir = tmp.dir();

    QString category = info.category();
    if (category == QLatin1String("structure/js"))
        mParser.reset(new ScriptFileParser(mPluginInfo.pluginName(),
                absolutePath() + QLatin1String("/main.js")));
    else if (category == QLatin1String("structure"))
        mParser.reset(new OsdParser(mPluginInfo.pluginName(),
                absolutePath() + QLatin1Char('/') + mPluginInfo.pluginName()
                        + QLatin1String(".osd")));
    else
        kWarning() << "no valid parser found for plugin category '" << category << "'";
}

StructureDefinitionFile::~StructureDefinitionFile()
{
}

QVector<TopLevelDataInformation*> StructureDefinitionFile::structures() const
{
    Q_CHECK_PTR(mParser);
    return mParser->parseStructures();
}

TopLevelDataInformation* StructureDefinitionFile::structure(const QString& name) const
{
    Q_CHECK_PTR(mParser);
    QVector<TopLevelDataInformation*> list = mParser->parseStructures();
    TopLevelDataInformation* ret = 0;
    for (int i = 0; i < list.size(); ++i)
    {
        if (list.at(i)->actualDataInformation()->name() == name)
            ret = list.at(i);
        else
            delete list.at(i); //we have no use for this element
    }
    if (!ret)
        kWarning() << "could not find structure with name=" << name;
    return ret; // not found
}

QStringList StructureDefinitionFile::structureNames() const
{
    Q_CHECK_PTR(mParser);
    return mParser->parseStructureNames();
}

QString StructureDefinitionFile::absolutePath() const
{
    return mDir.absolutePath();
}

}
