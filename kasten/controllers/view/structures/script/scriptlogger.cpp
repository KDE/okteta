/*
    This file is part of the Okteta Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2010, 2011, 2012 Alex Richardson <alex.richardson@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "scriptlogger.hpp"
#include "../datatypes/datainformation.hpp"

#include <KLocalizedString>
#include <QIcon>

QIcon ScriptLogger::iconForLevel(ScriptLogger::LogLevel level)
{
    switch (level)
    {
    case LogInfo:
        return QIcon::fromTheme(QStringLiteral("dialog-information"));
    case LogWarning:
        return QIcon::fromTheme(QStringLiteral("dialog-warning"));
    case LogError:
        return QIcon::fromTheme(QStringLiteral("dialog-error"));
    default:
        Q_ASSERT_X(false, "ScriptLogger::iconForLevel", "Invalid log level passed");
        return {};
    }
}

ScriptLogger::ScriptLogger() = default;

ScriptLogger::~ScriptLogger() = default;

QVariant ScriptLogger::data(const QModelIndex& index, int role) const
{
    if (!index.isValid()) {
        return {};
    }
    int row = index.row();
    Q_ASSERT(row < mData.size());
    Q_ASSERT(!index.parent().isValid());
    if (role == Qt::DisplayRole) {
        const Data& data = mData.at(row);
        switch (index.column())
        {
        case ColumnTime:
            return data.time.toString(QStringLiteral("hh:mm:ss.zzz"));
        case ColumnOrigin:
            return data.origin;
        case ColumnMessage:
            return data.message;
        default:
            return {};
        }
    }
    if (role == Qt::DecorationRole && index.column() == ColumnTime) {
        return iconForLevel(mData.at(row).level);
    }
    return {};
}

int ScriptLogger::rowCount(const QModelIndex& parent) const
{
    if (parent.isValid()) {
        return 0;
    }
    return mData.size();
}

int ScriptLogger::columnCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent)
    return COLUMN_COUNT;
}

QVariant ScriptLogger::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        switch (section)
        {
        case ColumnTime:
            return i18nc("@title:column", "Time");
        case ColumnOrigin:
            return i18nc("@title:column", "Origin");
        case ColumnMessage:
            return i18nc("@title:column", "Message");
        default:
            return {};
        }
    }
    return {};
}

QDebug ScriptLogger::log(LogLevel level, const DataInformation* origin)
{
    Q_CHECK_PTR(origin);
    if (origin->loggedData() < level) {
        origin->setLoggedData(level);
    }
    return log(level, origin->fullObjectPath());
}

QDebug ScriptLogger::log(LogLevel level, const QString& origin)
{
    Q_ASSERT(level != LogInvalid);
    if (mLogToStdOut) {
        return (level == LogInvalid || level == LogInfo) ? qDebug() : qWarning();
    }

    beginInsertRows(QModelIndex(), mData.size(), mData.size());
    mData.append(Data(level, origin));
    endInsertRows();
    return QDebug(&mData.last().message);
}

void ScriptLogger::clear()
{
    if (mData.isEmpty()) {
        return;
    }

    beginRemoveRows(QModelIndex(), 0, mData.size() - 1);
    mData.clear();
    endRemoveRows();
}

QStringList ScriptLogger::messages(LogLevel minLevel) const
{
    QStringList ret;
    for (const auto& d : mData) {
        if (d.level >= minLevel) {
            ret << d.message;
        }
    }

    return ret;
}
