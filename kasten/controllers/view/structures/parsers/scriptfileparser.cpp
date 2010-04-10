/*
 *   This file is part of the Okteta Kasten Framework, part of the KDE project.
 *
 *   Copyright 2010 Alex Richardson <alex.richardson@gmx.de>
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

#include "scriptfileparser.h"

#include "../structuredefinitionfile.h"
#include "../datatypes/topleveldatainformation.h"
#include "../datatypes/int32datainformation.h"

ScriptFileParser::ScriptFileParser(const Kasten::StructureDefinitionFile* const def) :
    AbstractStructureParser(def), mParsedCompletely(false)
{
}

ScriptFileParser::~ScriptFileParser()
{
}

QStringList ScriptFileParser::parseStructureNames()
{
    return QStringList() << mDef->pluginInfo().pluginName();
}

QList<const TopLevelDataInformation*> ScriptFileParser::parseStructures()
{
    QList<const TopLevelDataInformation*> ret;
    Int32DataInformation* dummyDataInf = new Int32DataInformation("dummy", Type_Int32);
    TopLevelDataInformation* topData = new TopLevelDataInformation(*dummyDataInf, QFileInfo(
            mDef->dir().absoluteFilePath("main.js")), true,
            mDef->pluginInfo().pluginName());
    delete dummyDataInf;
    if (topData->wasAbleToParse())
        ret << topData;
    else
    {
        kWarning() << "could not parse file " << mDef->absolutePath();
        delete topData;
    }
    mParsedCompletely = true;

    return ret;
}

bool ScriptFileParser::isFullyParsed()
{
    return mParsedCompletely;
}

QList<EnumDefinition::Ptr> ScriptFileParser::parseEnums()
{
    //FIXME stub, no better implementation useful now
    return QList<EnumDefinition::Ptr> ();
}
