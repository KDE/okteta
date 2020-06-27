/*
    This file is part of the Okteta Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2010, 2011, 2012 Alex Richardson <alex.richardson@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_SCRIPTFILEPARSER_HPP
#define KASTEN_SCRIPTFILEPARSER_HPP

#include "abstractstructureparser.hpp"

#include <QScriptValue>

class ScriptLogger;
class QScriptEngine;
class DataInformation;

class ScriptFileParser : public AbstractStructureParser
{
    Q_DISABLE_COPY(ScriptFileParser)

public:
    ScriptFileParser(const QString& pluginName, const QString& absolutePath);
    ~ScriptFileParser() override;

    QStringList parseStructureNames() const override;
    QVector<TopLevelDataInformation*> parseStructures() const override;

private:
    QScriptValue loadScriptValue(ScriptLogger* logger, QScriptEngine* engine) const;
};

#endif /* KASTEN_SCRIPTFILEPARSER_HPP */
