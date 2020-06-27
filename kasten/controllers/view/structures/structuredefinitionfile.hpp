/*
    This file is part of the Okteta Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2009, 2010, 2012 Alex Richardson <alex.richardson@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_STRUCTUREDEFINITIONFILE_HPP
#define KASTEN_STRUCTUREDEFINITIONFILE_HPP

#include <QString>
#include <QVector>
#include <QDir>
#include <QStringList>
#include <QDomNodeList>
#include <QFileInfo>
#include <KPluginInfo>

#include "datatypes/primitive/enumdefinition.hpp"

class TopLevelDataInformation;
class AbstractStructureParser;

namespace Kasten {

/**
 *  This class takes care of all the XML parsing and stores the result.
 */
class StructureDefinitionFile
{
    Q_DISABLE_COPY(StructureDefinitionFile)

public:
    /**
     * This class uses lazy parsing
     * @param info the information about this structure definition
     *      (passed by value so nothing bad can happen)
     */
    explicit StructureDefinitionFile(const KPluginInfo& info);
    virtual ~StructureDefinitionFile();

public:
    QVector<TopLevelDataInformation*> structures() const;
    QStringList structureNames() const;
    TopLevelDataInformation* structure(const QString& name) const;
    /** @return the absolute path to the directory containing the .desktop file */
    QString absolutePath() const;
    KPluginInfo pluginInfo() const;
    bool isValid() const;

private:
    KPluginInfo mPluginInfo;
    QScopedPointer<AbstractStructureParser> mParser;
};

} // namespace Kasten

#endif /* KASTEN_STRUCTUREDEFINITIONFILE_HPP */
