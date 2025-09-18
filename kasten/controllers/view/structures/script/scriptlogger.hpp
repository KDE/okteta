/*
    SPDX-FileCopyrightText: 2010, 2011, 2012 Alex Richardson <alex.richardson@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_SCRIPTLOGGER_HPP
#define KASTEN_SCRIPTLOGGER_HPP

// Qt
#include <QAbstractTableModel>
#include <QDebug>
#include <QTime>
// Std
#include <vector>

class QIcon;

class DataInformation;
/** NOT THREAD SAFE! */
class ScriptLogger : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit ScriptLogger();
    ~ScriptLogger() override;

public:
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

        Data& operator=(const Data&) = default;

        ScriptLogger::LogLevel level = LogInvalid;
        QString message;
        QString origin;
        QTime time;
    };

public: // QAbstractItemModel API
    [[nodiscard]]
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    [[nodiscard]]
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    [[nodiscard]]
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    [[nodiscard]]
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

public:
    [[nodiscard]]
    inline QDebug info(const DataInformation* origin) { return log(LogInfo, origin); }
    [[nodiscard]]
    inline QDebug warn(const DataInformation* origin) { return log(LogWarning, origin); }
    [[nodiscard]]
    inline QDebug error(const DataInformation* origin) { return log(LogError, origin); }
    [[nodiscard]]
    inline QDebug info(const QString& origin = QString()) { return log(LogInfo, origin); }
    [[nodiscard]]
    inline QDebug warn(const QString& origin = QString()) { return log(LogWarning, origin); }
    [[nodiscard]]
    inline QDebug error(const QString& origin = QString()) { return log(LogError, origin); }
    /**
     * @return a QDebug to write the message to.
     * Do NOT save this object, since the string it writes to may become invalid!
     * Just write the message using the << operators and do not touch it anymore after the line ends
     */
    [[nodiscard]]
    QDebug log(LogLevel level, const DataInformation* origin);
    [[nodiscard]]
    QDebug log(LogLevel level, const QString& origin);
    void clear();
    /**
     * @param minLevel the minimum level that the messages must have
     * @return all the messages, mainly used for testing
     */
    [[nodiscard]]
    QStringList messages(LogLevel minLevel = LogInfo) const;
    /** whether to log to stdout instead of saving the messages */
    inline void setLogToStdOut(bool val) { mLogToStdOut = val; }

    [[nodiscard]]
    static QIcon iconForLevel(LogLevel level);

private:
    std::vector<Data> mData;
    bool mLogToStdOut = false;
};

struct LoggerWithContext
{
public:
    LoggerWithContext(ScriptLogger* l, const QString& s)
        : logger(l)
        , context(s)
    {}
    LoggerWithContext(const LoggerWithContext&) = delete;
    LoggerWithContext(LoggerWithContext&&) = delete;

    ~LoggerWithContext() = default;

public:
    LoggerWithContext& operator=(const LoggerWithContext&) = delete;
    LoggerWithContext& operator=(LoggerWithContext&&) = delete;

public:
    [[nodiscard]]
    inline QDebug info() const { return logger ? logger->info(context) : qDebug(); }
    [[nodiscard]]
    inline QDebug warn() const { return logger ? logger->warn(context) : qWarning(); }
    [[nodiscard]]
    inline QDebug error() const { return logger ? logger->error(context) : qWarning(); }

private:
    ScriptLogger* const logger;
    const QString context;
};

#endif // KASTEN_SCRIPTLOGGER_HPP
