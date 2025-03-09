/*
    This file is part of the Okteta Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2009, 2010, 2012 Alex Richardson <alex.richardson@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_STRUCTUREDEFINITIONFILE_HPP
#define KASTEN_STRUCTUREDEFINITIONFILE_HPP

// tool
#include "datatypes/primitive/enumdefinition.hpp"
#include "structuremetadata.hpp"
// Qt
#include <QString>
#include <QVector>
#include <QDir>
#include <QStringList>
#include <QDomNodeList>
#include <QFileInfo>
// Std
#include <memory>

class TopLevelDataInformation;
class AbstractStructureParser;

namespace Kasten {

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
    explicit StructureDefinitionFile(const StructureMetaData& metaData);
    StructureDefinitionFile(const StructureDefinitionFile&) = delete;
    StructureDefinitionFile(StructureDefinitionFile&&) = delete;

    virtual ~StructureDefinitionFile();

    StructureDefinitionFile& operator=(const StructureDefinitionFile&) = delete;
    StructureDefinitionFile& operator=(StructureDefinitionFile&&) = delete;

public:
    [[nodiscard]]
    QVector<TopLevelDataInformation*> structures() const;
    [[nodiscard]]
    QStringList structureNames() const;
    [[nodiscard]]
    TopLevelDataInformation* structure(const QString& name) const;
    /** @return the absolute path to the directory containing the .desktop file */
    [[nodiscard]]
    QString absolutePath() const;
    [[nodiscard]]
    StructureMetaData metaData() const;
    [[nodiscard]]
    bool isValid() const;

private:
    StructureMetaData mMetaData;
    std::unique_ptr<AbstractStructureParser> mParser;
};

} // namespace Kasten

#endif /* KASTEN_STRUCTUREDEFINITIONFILE_HPP */
