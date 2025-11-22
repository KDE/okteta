/*
    SPDX-FileCopyrightText: 2010, 2011, 2012 Alex Richardson <alex.richardson@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_SCRIPTFILEPARSER_HPP
#define KASTEN_SCRIPTFILEPARSER_HPP

// lib
#include "abstractstructureparser.hpp"
// Qt
#include <QScriptValue>

class ScriptLogger;
class QScriptEngine;
class DataInformation;

class ScriptFileParser : public AbstractStructureParser
{
public:
    ScriptFileParser(const QString& pluginName, const QString& absolutePath);
    ~ScriptFileParser() override;

public: // AbstractStructureParser API
    QStringList parseStructureNames() const override;
    std::vector<std::unique_ptr<TopLevelDataInformation>> parseStructures() const override;

private:
    QScriptValue loadScriptValue(ScriptLogger* logger, QScriptEngine* engine) const;
};

#endif /* KASTEN_SCRIPTFILEPARSER_HPP */
