/*
 *   This file is part of the Okteta Kasten Framework, part of the KDE project.
 *
 *   Copyright 2010 Alex Richardson <alex.richardson@gmx.de>
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
#include "scriptutils.h"

#include <QtScript/QScriptValueIterator>
#include <QtScript/QScriptValue>
#include <QtScript/QScriptString>
#include <KDebug>

void dumpQScriptValue(QScriptValue& val, const char* file, int line)
{
    kDebug()
        << "dumping called from: " << file << ":" << line;
    if (!val.isValid())
    {
        kDebug()
            << "val is invalid";
        return;
    }

    if (val.isUndefined())
    {
        kDebug()
            << "val is undefined";
        return;
    }
    kDebug()
        << "val=" << val.toString();

    QScriptValueIterator it(val);
    while (it.hasNext())
    {
        it.next();
        kDebug()
            << "name=" << it.name() << "scriptName=" << it.scriptName() << "val="
                    << it.value().toString();
    }
}
