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

#ifndef SCRIPTLOGGER_H
#define SCRIPTLOGGER_H

#include <QAbstractListModel>
#include <QVector>
#include <QScriptValue>

class ScriptLogger : public QAbstractListModel {
    Q_OBJECT
public:
    enum LogLevel {
        LogInfo, LogWarning, LogError
    };
    struct Data {
        ScriptLogger::LogLevel level;
        QString message;
        QScriptValue cause;
    };
    virtual QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
    virtual int rowCount(const QModelIndex & parent = QModelIndex()) const;

    void info(const QString& message, const QScriptValue& cause = QScriptValue());
    void warn(const QString& message, const QScriptValue& cause = QScriptValue());
    void error(const QString& message, const QScriptValue& cause = QScriptValue());
    void log(LogLevel level, const QString& message, const QScriptValue& cause);
private:
    QVector<Data> mData;
};

inline void ScriptLogger::error(const QString& message, const QScriptValue& cause)
{
    log(LogError, message, cause);
}

inline void ScriptLogger::warn(const QString& message, const QScriptValue& cause)
{
    log(LogWarning, message, cause);
}

inline void ScriptLogger::info(const QString& message, const QScriptValue& cause)
{
    log(LogInfo, message, cause);
}



#endif // SCRIPTLOGGER_H
