/*
 *   This file is part of the Okteta Kasten Framework, made within the KDE community.
 *
 *   Copyright 2009, 2010 Alex Richardson <alex.richardson@gmx.de>
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
#include <QDomNode>
#include <QStringList>

#include <KDebug>
#include "datatypes/topleveldatainformation.h"

#include "parsers/abstractstructureparser.h"
#include "parsers/osdparser.h"
#include "parsers/scriptfileparser.h"

namespace Kasten2
{
StructureDefinitionFile::StructureDefinitionFile(KPluginInfo info) :
    mPluginInfo(info), mValid(true), mStructureNamesParsed(false),
            mStructuresParsedCompletely(false)
{
    QFileInfo tmp(info.entryPath());
    mDir = tmp.dir();

    QString category = info.category();
    if (category == QLatin1String("structure/js"))
        mParser = new ScriptFileParser(this);
    else if (category == QLatin1String("structure"))
        mParser = new OsdParser(this);
    else
    {
        //no valid parser found:
        mValid = false;

        mStructuresParsedCompletely = true;
        mStructureNamesParsed = true;
        //now all methods will just return an empty list
        kWarning() << "no valid parser found for plugin category '" << category
                << "'";
    }

}

StructureDefinitionFile::~StructureDefinitionFile()
{
    qDeleteAll(mTopLevelStructures);
    delete mParser;
}

StructureDefinitionFile::StructureDefinitionFile(StructureDefinitionFile& f) :
    mPluginInfo(f.mPluginInfo), mDir(f.mDir), mStructureNames(f.mStructureNames),
            mValid(f.mValid), mStructureNamesParsed(f.mStructureNamesParsed),
            mStructuresParsedCompletely(f.mStructuresParsedCompletely)
{
    int len = f.mTopLevelStructures.length();
    for (int i = 0; i < len; ++i)
    {
        mTopLevelStructures.append(f.mTopLevelStructures.at(i)->clone());
    }
}

QList<TopLevelDataInformation*> StructureDefinitionFile::structures()
{
    if (!mStructuresParsedCompletely && mParser)
    {
        mTopLevelStructures = mParser->parseStructures();
        if (!mStructureNamesParsed) //also parse the names, they should be there already anyway, so there is no overhead
            mStructureNames = mParser->parseStructureNames();
        //is parsed completely now -> delete parser
        delete mParser;
        mParser = 0;
        mStructureNamesParsed = true;
        mStructuresParsedCompletely = true;
    }
    //return copy
    QList<TopLevelDataInformation*> ret;
    foreach(const TopLevelDataInformation* data, mTopLevelStructures)
        {
            ret.append(data->clone());
        }
    return ret;
}

//TODO QSharedDataPointer instead?
TopLevelDataInformation* StructureDefinitionFile::structure(QString& name)
{
    if (!mValid)
        kError() << "invalid file -> can't find structure";
    else if (!mStructuresParsedCompletely)
    {
        if (mParser)
        {
            mTopLevelStructures = mParser->parseStructures();
            if (!mStructureNamesParsed)
            {
                foreach(const TopLevelDataInformation* data, mTopLevelStructures)
                    {
                        mStructureNames << data->actualDataInformation()->name();
                    }
            }
            mStructureNamesParsed = true;
            mStructuresParsedCompletely = true;
            delete mParser;
            mParser = NULL;
        }
    }
    foreach(const TopLevelDataInformation* data,mTopLevelStructures)
        {
            if (data->actualDataInformation()->name() == name)
            {
                return data->clone();
            }
        }
    kWarning() << "could not find structure with name " << name;
    return NULL; // not found
}

const QStringList StructureDefinitionFile::structureNames()
{
    if (!mStructureNamesParsed)
    {
        //should be kept in sync with structuresCount()
        if (mParser)
        {
            mStructureNames = mParser->parseStructureNames();
            if (mParser->isFullyParsed()) //was it necessary to completely parse to get enums?

            {
                //no point retaining in memory, has been parsed, assign all values now
                if (!mStructuresParsedCompletely)
                    mTopLevelStructures = mParser->parseStructures();
                mStructuresParsedCompletely = true;
                delete mParser;
                mParser = NULL; //so it can be safely deleted again later
            }
        }
        mStructureNamesParsed = true;
    }
    return mStructureNames;
}

uint StructureDefinitionFile::structuresCount()
{
    //get structure names and then return name cound
    if (!mStructureNamesParsed)
    {
        //should be kept in sync with structureNames()
        if (mParser)
        {
            mStructureNames = mParser->parseStructureNames();
            if (mParser->isFullyParsed()) //was it necessary to completely parse to get enums?

            {
                //no point retaining in memory, has been parsed, assign all values now
                if (!mStructuresParsedCompletely)
                    mTopLevelStructures = mParser->parseStructures();
                mStructuresParsedCompletely = true;
                delete mParser;
                mParser = NULL; //so it can be safely deleted again later
            }
        }
        mStructureNamesParsed = true;
    }
    return mStructureNames.length();
}

QString StructureDefinitionFile::absolutePath() const
{
    return mDir.absolutePath();
}

}
