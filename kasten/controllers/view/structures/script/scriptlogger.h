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
#include <QDebug>
#include <QTime>
#include <QScriptValue>

class DataInformation;

class ScriptLogger: public QAbstractListModel {
Q_OBJECT
Q_DISABLE_COPY(ScriptLogger)
public:
    ScriptLogger();
    ~ScriptLogger();

	enum LogLevel {
	    LogInvalid = 0, LogInfo = 1, LogWarning = 2, LogError = 3
	};
	struct Data {
	    inline Data() : level(LogInvalid) {}
	    inline Data(LogLevel lvl, const QString& o, const QScriptValue& c)
	        : level(lvl), origin(o), cause(c), time(QTime::currentTime()) {}
	    inline Data(const Data& d)
	        : level(d.level), message(d.message), origin(d.origin), cause(d.cause), time(d.time) {}
	    inline ~Data() {}
		ScriptLogger::LogLevel level;
		QString message;
		QString origin;
		QScriptValue cause;
		QTime time;
	};
	virtual QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
	virtual int rowCount(const QModelIndex & parent = QModelIndex()) const;

	QDebug info(const QScriptValue& cause = QScriptValue()) { return log(LogInfo, cause); }
	QDebug warn(const QScriptValue& cause = QScriptValue()) { return log(LogWarning, cause); }
	QDebug error(const QScriptValue& cause = QScriptValue()) { return log(LogError, cause); }
	QDebug info(const DataInformation* origin) { return log(LogInfo, origin); }
	QDebug warn(const DataInformation* origin) { return log(LogWarning, origin); }
	QDebug error(const DataInformation* origin) { return log(LogError, origin); }
	/**
	 * @return a QDebug to write the message to.
	 * Do NOT save this object, since the string it writes to may become invalid!
	 * Just write the message using the << operators and do not touch it anymore after the line ends
	 */
	QDebug log(LogLevel level, const QScriptValue& cause);
	QDebug log(LogLevel level, const DataInformation* cause);
	QDebug log(LogLevel level, const QString& origin, const QScriptValue& cause);
	void clear();
	/**
	 * @param info the minimum level that the messages must have
	 * @return all the messages, mainly used for testing
	 */
	QStringList messages(LogLevel minLevel = LogInfo) const;

private:
	QVector<Data> mData;
};

#endif // SCRIPTLOGGER_H
