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
#include <KLocalizedString>

KIcon ScriptLogger::iconForLevel(ScriptLogger::LogLevel level)
{
    if (level == LogInfo)
        return KIcon(QLatin1String("dialog-information"));
    else if (level == LogWarning)
        return KIcon(QLatin1String("dialog-warning"));
    else
        return KIcon(QLatin1String("dialog-error"));
    return KIcon();
}

QVariant ScriptLogger::data(const QModelIndex& index, int role) const
{
    if (!index.isValid())
        return QVariant();
    int row = index.row();
    Q_ASSERT(row < mData.size());
    Q_ASSERT(!index.parent().isValid());
    if (role == Qt::DisplayRole)
    {
        const Data& data = mData.at(row);
        switch (index.column())
        {
        case ColumnTime:
            return data.time.toString(QLatin1String("hh:mm:ss.zzz"));
        case ColumnOrigin:
            return data.origin;
        case ColumnMessage:
            return data.message;
        default:
            return QVariant();
        }
    }
    if (role == Qt::DecorationRole && index.column() == ColumnTime)
        return iconForLevel(mData.at(row).level);
    return QVariant();
}

int ScriptLogger::rowCount(const QModelIndex& parent) const
{
    if (parent.isValid())
        return 0;
    return mData.size();
}

int ScriptLogger::columnCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent)
    return COLUMN_COUNT;
}

QVariant ScriptLogger::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        switch (section)
        {
        case ColumnTime:
            return i18nc("@title:column", "Time");
        case ColumnOrigin:
            return i18nc("@title:column", "Origin");
        case ColumnMessage:
            return i18nc("@title:column", "Message");
        default:
            return QVariant();
        }
    }
    return QVariant();
}

QDebug ScriptLogger::log(LogLevel level, const DataInformation* origin)
{
    Q_CHECK_PTR(origin);
    if (origin->loggedData() < level)
        origin->setLoggedData(level);
    return log(level, origin->fullObjectPath());
}

QDebug ScriptLogger::log(LogLevel level, const QString& origin)
{
    Q_ASSERT(level != LogInvalid);
    if (mLogToStdOut)
        return (level == LogInvalid || level == LogInfo) ? qDebug() : qWarning();

    beginInsertRows(QModelIndex(), mData.size(), mData.size());
    mData.append(Data(level, origin));
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
        : mLogToStdOut(false)
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
