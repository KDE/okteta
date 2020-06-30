/*
    This file is part of the Okteta Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2010, 2011, 2012 Alex Richardson <alex.richardson@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "scriptvalueconverter.hpp"
#include "scriptvalueconverter_p.hpp"
#include "../script/scriptlogger.hpp"

#include <QString>
#include <QVector>
#include <QScriptValue>
#include <QScriptEngine>
#include <QScriptValueIterator>

namespace ScriptValueConverter {

DataInformation* convert(const QScriptValue& value, const QString& name, ScriptLogger* logger,
                         DataInformation* parent)
{
    // TODO Q_CHECK_PTR(parent)
    const ParserInfo info(name, logger, parent, value.engine());
    return toDataInformation(value, info); // could be NULL
}

QVector<DataInformation*> convertValues(const QScriptValue& value, ScriptLogger* logger,
                                        DataInformation* parent)
{
    // TODO Q_CHECK_PTR(parent);
    QVector<DataInformation*> ret;
    QScriptValueIterator it(value);
    const bool isArray = value.isArray();
    while (it.hasNext()) {
        it.next();
        if (isArray && it.name() == QLatin1String("length")) {
            continue; // skip the length property of arrays
        }
        const ParserInfo info(it.name(), logger, parent, value.engine());
        DataInformation* inf = toDataInformation(it.value(), info);

        if (inf) {
            ret.append(inf);
        } else { // TODO remove the null check once parent must be nonnull
            logger->info(parent ? parent->fullObjectPath() : QString()).nospace()
                << "Could not convert property '" << it.name() << "'.";
        }
    }
    return ret;
}

} // namespace ScriptValueConverter
