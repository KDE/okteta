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
#include "structuredefinitionfile.h"

#include <QFile>
#include <QDomNode>
#include <KDebug>
#include <QStringList>

#include "datatypes/structuredatainformation.h"
#include "datatypes/uniondatainformation.h"
#include "datatypes/staticlengtharraydatainformation.h"
#include "datatypes/primitivedatainformation.h"

namespace Kasten
{
StructureDefinitionFile::StructureDefinitionFile(const QFileInfo& path) :
    mPath(path), mValidAndLoaded(false)
{
    mTitle = path.baseName();
}

StructureDefinitionFile::~StructureDefinitionFile()
{
    qDeleteAll(mTopLevelStructures);
}

StructureDefinitionFile::StructureDefinitionFile(StructureDefinitionFile& f) :
    mPath(f.mPath), mTitle(f.mTitle), mDescription(f.mDescription), mAuthor(
            f.mAuthor), mIncludedFiles(f.mIncludedFiles), mValidAndLoaded(
            f.mValidAndLoaded)
{
    int len = f.mTopLevelStructures.length();
    for (int i = 0; i < len; ++i)
    {
        mTopLevelStructures.append(f.mTopLevelStructures.at(i)->clone());
    }
}

void StructureDefinitionFile::parseIncludeNode(QDomElement& elem)
{
    QString includeFile = elem.attribute("file", QString::null);
    if (!includeFile.isNull())
    {
        kDebug() << "including file: " << includeFile;
        //if contains this file, file was already included
        if (!mIncludedFiles.contains(includeFile))
        {
            mIncludedFiles.append(includeFile);
        }
        else
        {
            kDebug() << "included file is already loaded";
        }
    }
}
QList<DataInformation*> StructureDefinitionFile::structures() const
{
    QList<DataInformation*> list;
    foreach(const DataInformation* data,mTopLevelStructures)
        {
            list.append(data->clone());
        }
    return list;
}

void StructureDefinitionFile::parse()
{
    if (mValidAndLoaded)
    {
        kDebug() << "Already loaded -> will return";
        return;
    }
    QFile file(mPath.absoluteFilePath());
    QDomDocument doc;
    if (!file.open(QIODevice::ReadOnly))
    {
        kWarning() << "could not open file " << mPath.absoluteFilePath();
        return;
    }
    int errorLine, errorColumn;
    QString errorMsg;
    if (!doc.setContent(&file, false, &errorMsg, &errorLine, &errorColumn))
    {
        file.close();
        kWarning() << "DataInformation::loadFromXML(): error reading file:\n"
                << errorMsg << "\n error line=" << errorLine << " error column="
                << errorColumn;
        return;
    }
    file.close();
    QDomElement docElem = doc.documentElement();
    //    kDebug() << "root elem tag: " << docElem.tagName();
    QDomNode n = docElem.firstChild();
    QDomNodeList list = docElem.childNodes();
    while (!n.isNull())
    {
        QDomElement elem = n.toElement(); // try to convert the node to an element.
        DataInformation* data = NULL;
        if (!elem.isNull())
        {
            kDebug() << "element tag: " << elem.tagName();
            //e is element
            QString tag = elem.tagName();
            if (tag == "include")
            {
                parseIncludeNode(elem);
                n = n.nextSibling();
                continue;
            }
            else if (tag == "struct")
                data = StructureDataInformation::fromXML(elem);
            else if (tag == "array")
                data = StaticLengthArrayDataInformation::fromXML(elem);
            else if (tag == "primitive")
                data = PrimitiveDataInformation::fromXML(elem);
            else if (tag == "union")
                data = UnionDataInformation::fromXML(elem);
            else if (tag == "title")
                mTitle = elem.text();
            else if (tag == "description")
                mDescription = elem.text();
            else if (tag == "author")
                mAuthor = elem.text();
            //          kDebug() << "end of tag: " << tag;
        }

        if (data)
        {
            mTopLevelStructures.append(data);
        }
        else
        {
            kDebug() << "data == NULL -> could not parse node " << elem.tagName();
        }
        n = n.nextSibling();
    }
    mValidAndLoaded = true;
    if (mTitle.isEmpty())
    {
        mTitle = mPath.baseName();
    }
    return;
}
DataInformation* StructureDefinitionFile::getStructure(QString& name) const
{
    if (!mValidAndLoaded)
        kError() << "member accessed before file was parsed";
    foreach(const DataInformation* data,mTopLevelStructures)
        {
            if (data->getName() == name) {
                return data->clone();
            }
        }
    return NULL; // not found
}
QString StructureDefinitionFile::title() const
{
    if (!mValidAndLoaded)
        kError() << "member accessed before file was parsed";
    return mTitle;
}
QString StructureDefinitionFile::description() const
{
    if (!mValidAndLoaded)
        kError() << "member accessed before file was parsed";
    return mDescription;
}
QString StructureDefinitionFile::author() const
{
    if (!mValidAndLoaded)
        kError() << "member accessed before file was parsed";
    return mAuthor;
}
QStringList StructureDefinitionFile::includedFiles() const
{
    if (!mValidAndLoaded)
        kError() << "member accessed before file was parsed";
    return mIncludedFiles;
}
}
