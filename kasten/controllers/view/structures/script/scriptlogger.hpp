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

#ifndef KASTEN_SCRIPTLOGGER_HPP
#define KASTEN_SCRIPTLOGGER_HPP

#include <QAbstractTableModel>
#include <QVector>
#include <QDebug>
#include <QTime>

class QIcon;

class DataInformation;
/** NOT THREAD SAFE! */
class ScriptLogger : public QAbstractTableModel
{
    Q_DISABLE_COPY(ScriptLogger)
    Q_OBJECT

public:
    explicit ScriptLogger();
    ~ScriptLogger() override;

    enum Columns
    {
        ColumnTime = 0,
        ColumnOrigin,
        ColumnMessage,
        COLUMN_COUNT
    };
    // fits into 2 bits
    enum LogLevel
    {
        LogInvalid = 0,
        LogInfo = 1,
        LogWarning = 2,
        LogError = 3
    };
    struct Data
    {
        inline Data() = default;
        inline Data(LogLevel lvl, const QString& o)
            : level(lvl)
            , origin(o)
            , time(QTime::currentTime())
        {}
        inline Data(const Data& d) = default;
        inline ~Data() = default;
        ScriptLogger::LogLevel level = LogInvalid;
        QString message;
        QString origin;
        QTime time;
    };
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    inline QDebug info(const DataInformation* origin) { return log(LogInfo, origin); }
    inline QDebug warn(const DataInformation* origin) { return log(LogWarning, origin); }
    inline QDebug error(const DataInformation* origin) { return log(LogError, origin); }
    inline QDebug info(const QString& origin = QString()) { return log(LogInfo, origin); }
    inline QDebug warn(const QString& origin = QString()) { return log(LogWarning, origin); }
    inline QDebug error(const QString& origin = QString()) { return log(LogError, origin); }
    /**
     * @return a QDebug to write the message to.
     * Do NOT save this object, since the string it writes to may become invalid!
     * Just write the message using the << operators and do not touch it anymore after the line ends
     */
    QDebug log(LogLevel level, const DataInformation* origin);
    QDebug log(LogLevel level, const QString& origin);
    void clear();
    /**
     * @param info the minimum level that the messages must have
     * @return all the messages, mainly used for testing
     */
    QStringList messages(LogLevel minLevel = LogInfo) const;
    /** whether to log to stdout instead of saving the messages */
    inline void setLogToStdOut(bool val) { mLogToStdOut = val; }

    static QIcon iconForLevel(LogLevel level);

private:
    QVector<Data> mData;
    bool mLogToStdOut = false;
};

struct LoggerWithContext
{
public:
    LoggerWithContext(ScriptLogger* l, const QString& s)
        : logger(l)
        , context(s)
    {}
    inline QDebug info() const { return logger ? logger->info(context) : qDebug(); }
    inline QDebug warn() const { return logger ? logger->warn(context) : qWarning(); }
    inline QDebug error() const { return logger ? logger->error(context) : qWarning(); }

private:
    ScriptLogger* const logger;
    QString context;
};

#endif // KASTEN_SCRIPTLOGGER_HPP
