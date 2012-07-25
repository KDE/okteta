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

#include "scriptlogger.h"
#include "../datatypes/datainformation.h"

#include <KIcon>

QVariant ScriptLogger::data(const QModelIndex& index, int role) const
{
    if (!index.isValid() || index.column() != 0)
        return QVariant();
    int row = index.row();
    Q_ASSERT(row < mData.size());
    Q_ASSERT(!index.parent().isValid());
    if (role == Qt::DisplayRole)
    {
        const Data& data = mData.at(row);
        const QString time = data.time.toString(QLatin1String("hh:mm:ss.zzz")) + QLatin1String(" - ");
        if (!data.origin.isEmpty())
            return QString(time + data.origin + QLatin1String(": ") + data.message);
        return QString(time + data.message);
    }
    else if (role == Qt::DecorationRole)
    {
        LogLevel level = mData.at(row).level;
        if (level == LogInfo)
            return KIcon(QLatin1String("dialog-info"));
        else if (level == LogWarning)
            return KIcon(QLatin1String("dialog-warning"));
        else
            return KIcon(QLatin1String("dialog-error"));
    }
    return QVariant();
}

int ScriptLogger::rowCount(const QModelIndex& parent) const
{
    if (parent.isValid())
        return 0;
    return mData.size();
}

QDebug ScriptLogger::log(LogLevel level, const QScriptValue& cause)
{
    return log(level, QString(), cause);
}

QDebug ScriptLogger::log(LogLevel level, const DataInformation* origin)
{
    return log(level, origin->fullObjectPath(), QScriptValue());
}

QDebug ScriptLogger::log(LogLevel level, const QString& origin, const QScriptValue& cause)
{
    beginInsertRows(QModelIndex(), mData.size(), mData.size());
    mData.append(Data(level, origin, cause));
    endInsertRows();
    return QDebug(&mData.last().message);
}

void ScriptLogger::clear()
{
    beginRemoveRows(QModelIndex(), 0, qMax(0, mData.size() - 1));
    mData.clear();
    endRemoveRows();
}

ScriptLogger::ScriptLogger()
{
}

ScriptLogger::~ScriptLogger()
{
}

QStringList ScriptLogger::messages(LogLevel minLevel) const
{
    QStringList ret;
    for (int i = 0; i < mData.size(); ++i)
    {
        const Data& d = mData.at(i);
        if (d.level >= minLevel)
            ret << d.message;
    }
    return ret;
}
