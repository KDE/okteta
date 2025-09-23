/*
    SPDX-FileCopyrightText: 2010, 2011, 2012 Alex Richardson <alex.richardson@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "scriptvalueconverter.hpp"
#include "scriptvalueconverter_p.hpp"

// lib
#include <scriptlogger.hpp>
// Qt
#include <QString>
#include <QScriptValue>
#include <QScriptEngine>
#include <QScriptValueIterator>

namespace ScriptValueConverter {

std::unique_ptr<DataInformation> convert(const QScriptValue& value, const QString& name, ScriptLogger* logger,
                                         DataInformation* parent)
{
    // TODO Q_CHECK_PTR(parent)
    const ParserInfo info(name, logger, parent, value.engine());
    return std::unique_ptr<DataInformation>(toDataInformation(value, info)); // could be NULL
}

std::vector<std::unique_ptr<DataInformation>> convertValues(const QScriptValue& value, ScriptLogger* logger,
                                                            DataInformation* parent)
{
    // TODO Q_CHECK_PTR(parent);
    std::vector<std::unique_ptr<DataInformation>> ret;
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
            ret.emplace_back(std::unique_ptr<DataInformation>(inf));
        } else { // TODO remove the null check once parent must be nonnull
            logger->info(parent ? parent->fullObjectPath() : QString()).nospace()
                << "Could not convert property '" << it.name() << "'.";
        }
    }
    return ret;
}

} // namespace ScriptValueConverter
