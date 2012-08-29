/*
 *   This file is part of the Okteta Kasten Framework, made within the KDE community.
 *
 *   Copyright 2010, 2011, 2012 Alex Richardson <alex.richardson@gmx.de>
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

#include "scriptvalueconverter.h"
#include "scriptvalueconverter_p.h"
#include "../script/scriptlogger.h"

#include <QString>
#include <QVector>
#include <QScriptValue>
#include <QScriptEngine>
#include <QScriptValueIterator>

namespace ScriptValueConverter
{

DataInformation* convert(const QScriptValue& value, const QString& name, ScriptLogger* logger,
        DataInformation* parent)
{
    //TODO Q_CHECK_PTR(parent)
    const ParserInfo info(name, logger, parent);
    return toDataInformation(value, info); //could be NULL
}

QVector<DataInformation*> convertValues(const QScriptValue& value, ScriptLogger* logger,
        DataInformation* parent)
{
    //TODO Q_CHECK_PTR(parent);
    QVector<DataInformation*> ret;
    QScriptValueIterator it(value);
    const bool isArray = value.isArray();
    while (it.hasNext())
    {
        it.next();
        if (isArray && it.name() == QLatin1String("length"))
            continue; //skip the length property of arrays
        const ParserInfo info(it.name(), logger, parent);
        DataInformation* inf = toDataInformation(it.value(), info);

        if (inf)
            ret.append(inf);
        else //TODO remove the null check once parent must be nonnull
            logger->info(parent ? parent->fullObjectPath() : QString()).nospace()
                << "Could not convert property '" << it.name() << "'.";
    }
    return ret;
}

} //namespace ScriptValueConverter
