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
#include <QStringList>
#include <QFileInfo>

class DataInformation;
class QDomElement;
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
    StructureDefinitionFile(const QFileInfo& path);
    //FIXME copy constructor
    StructureDefinitionFile(StructureDefinitionFile& f);
    StructureDefinitionFile(QString title, QString description, QString author,
            QList<DataInformation*> topLevelStructures, QString absPath);
    virtual ~StructureDefinitionFile();
    void parse();
    //    const QList<const DataInformation*> structures() const
    //    {
    //        return mTopLevelStructures;
    //    }
    QList<DataInformation*> structures() const;
    DataInformation* getStructure(QString& name) const;
    QString title() const;
    QString description() const;
    QString author() const;
    QFileInfo path() const
    {
        return mPath;
    }
    /** a QList with the absolute paths to the included files */
    QStringList includedFiles() const;

private:
    void parseIncludeNode(QDomElement& elem);

    QFileInfo mPath;
    QString mTitle;
    QString mDescription;
    QString mAuthor;
    QList<const DataInformation*> mTopLevelStructures;
    QStringList mIncludedFiles;
    bool mValidAndLoaded :1;
    //TODO enum definitions
};
}
#endif /* STRUCTUREDEFINITIONFILE_H_ */
