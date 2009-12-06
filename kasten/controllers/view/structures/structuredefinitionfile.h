/*
 *   This file is part of the Okteta Kasten module, part of the KDE project.
 *
 *   Copyright 2009 Alex Richardson <alex.richardson@gmx.de>
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
#include "datatypes/enumdefinition.h"
class DataInformation;
class QDomElement;

class AbstractArrayDataInformation;
class PrimitiveDataInformation;
class UnionDataInformation;
class StructureDataInformation;
class EnumDataInformation;

namespace Kasten
{
/**
 *  This class takes care of all the XML parsing and stores the result.
 */
class StructureDefinitionFile
{
public:
    /**
     * When using this constructor the method parse() must be called
     * before this class is usable (to allow lazy initialisation).
     */
    StructureDefinitionFile(QFileInfo file, KPluginInfo info);
    StructureDefinitionFile(StructureDefinitionFile& f);
    virtual ~StructureDefinitionFile();

    void parse();
    QList<DataInformation*> structures() const;
    DataInformation* getStructure(QString& name) const;
    const KPluginInfo& info() const
    {
        return mPluginInfo;
    }
    const QDir dir() const
    {
        return mDir;
    }
    const QString absPath() const
    {
        return mFileInfo.absoluteFilePath();
    }
    /** a QList with the relative paths to the included files */
    QStringList includedFiles() const;
    const QList<EnumDefinition::Ptr> enums() const
    {
        return mEnums;
    }
    bool isLoaded()
    {
        return mLoaded;
    }
    bool isValid()
    {
        return mValid;
    }
private:
    AbstractArrayDataInformation* arrayFromXML(const QDomElement& node) const;
    PrimitiveDataInformation* primitiveFromXML(const QDomElement& node) const;
    UnionDataInformation* unionFromXML(const QDomElement& node) const;
    StructureDataInformation* structFromXML(const QDomElement& node) const;
    EnumDataInformation* enumFromXML(const QDomElement& node) const;
    DataInformation* parseNode(const QDomNode& node) const;
    EnumDefinition::Ptr findEnum(const QString& defName) const;

    void parseIncludeNodes(QDomNodeList& elems);
    void parseEnumDefNodes(QDomNodeList& elems);

    KPluginInfo mPluginInfo;
    QFileInfo mFileInfo;
    QDir mDir;
    QList<const DataInformation*> mTopLevelStructures;
    QStringList mIncludedFiles; //TODO use QList<Structuredefinitionfile> instead, why bother about a few extra bytes of mem
    bool mValid :1;
    bool mLoaded :1;
    QList<EnumDefinition::Ptr> mEnums;
};
}
#endif /* STRUCTUREDEFINITIONFILE_H_ */
