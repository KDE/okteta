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
#ifndef STRUCTUREDEFINITIONFILE_H_
#define STRUCTUREDEFINITIONFILE_H_

#include <QString>
#include <QList>
#include <QDir>
#include <QStringList>
#include <QDomNodeList>
#include <QFileInfo>
#include <KPluginInfo>

#include "datatypes/primitive/enumdefinition.h"

class TopLevelDataInformation;
class AbstractStructureParser;

namespace Kasten2
{
/**
 *  This class takes care of all the XML parsing and stores the result.
 */
class StructureDefinitionFile
{
public:
    /**
     * This class uses lazy parsing
     * @param info the information about this structure definition
     *      (passed by value so nothing bad can happen)
     */
    StructureDefinitionFile(KPluginInfo info);
    StructureDefinitionFile(StructureDefinitionFile& f);
    virtual ~StructureDefinitionFile();

    QList<TopLevelDataInformation*> structures();
    /** this is all that is needed for the StructureAddRemoveWidget,
     *  should allow improving performance compared to before.
     *  This method is not const, since it may cause parsing when called the first time
     */
    const QStringList structureNames();
    uint structuresCount();
    TopLevelDataInformation* structure(QString& name);

    const KPluginInfo& pluginInfo() const;
    const QDir dir() const;
    /** @return the absolute path to the directory containing the structure definition */
    QString absolutePath() const;

    bool isParsed() const;
    bool isParsedCompletely() const;
    bool isValid() const;
private:
    KPluginInfo mPluginInfo;
    /** the directory the plugin is saved in */
    QDir mDir;
    QList<const TopLevelDataInformation*> mTopLevelStructures;
    QStringList mStructureNames;

    AbstractStructureParser* mParser;
    bool mValid :1;
    /** when true basic parsing finished (names of structures found)
     * , i.e. mStructureNames has been filled */
    bool mStructureNamesParsed :1;
    /** when true complete parsing finished, i.e. mTopLevelStructures has been filled */
    bool mStructuresParsedCompletely :1;
};

inline const KPluginInfo& StructureDefinitionFile::pluginInfo() const
{
    return mPluginInfo;
}

inline const QDir StructureDefinitionFile::dir() const
{
    return mDir;
}

inline bool StructureDefinitionFile::isValid() const
{
    return mValid;
}

inline bool StructureDefinitionFile::isParsed() const
{
    return mStructureNamesParsed;
}

inline bool StructureDefinitionFile::isParsedCompletely() const
{
    return mStructuresParsedCompletely;
}

} //namespace Kasten2

#endif /* STRUCTUREDEFINITIONFILE_H_ */
