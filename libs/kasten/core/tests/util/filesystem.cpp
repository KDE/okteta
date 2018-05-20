/***************************************************************************
                          filesystem.cpp  -  description
                            -------------------
    begin                : Wed Mai 30 2007
    copyright            : 2007 by Friedrich W. H. Kossebau
    email                : kossebau@kde.org
***************************************************************************/

/***************************************************************************
*                                                                         *
*   This library is free software; you can redistribute it and/or         *
*   modify it under the terms of the GNU Library General Public           *
*   License version 2 as published by the Free Software Foundation.       *
*                                                                         *
***************************************************************************/

#include "filesystem.h"

// Qt
#include <QDir>
#include <QFile>
#include <QTest>

// TODO QStringLiteral
static const char basePath[] = "/.kde-unit-test/";

TestFileSystem::TestFileSystem(const QString& name)
    : mBasePath(QDir::homePath() + QLatin1Char('/')
        + QLatin1String(basePath) + QLatin1Char('/') + name)
{
    // clean up
    _removeDir(mBasePath);

    _createDir(mBasePath);
}

TestFileSystem::~TestFileSystem()
{
    _removeDir(mBasePath);

//     removeDir(QStringLiteral("kdatetimetest/Africa"));
//     removeDir( QStringLiteral("share/config"));
//     removeDir( QStringLiteral("share") );
//     QDir().rmpath(QDir::homePath() + "/.kde-unit-test/share");
}

void TestFileSystem::removeDir(const QString& subPath)
{
    _removeDir(mBasePath + QLatin1Char('/') + subPath);
}

void TestFileSystem::createDir(const QString& subPath)
{
    _createDir(mBasePath + QLatin1Char('/') + subPath);
}

QString TestFileSystem::createFilePath(const QString& fileName, const QString& subPath)
{
    return mBasePath + QLatin1Char('/') + subPath + QLatin1Char('/') + fileName;
}

void TestFileSystem::_removeDir(const QString& path)
{
    QDir localDir(path);
    const auto filesInDir = localDir.entryList(QDir::Files);
    for (const QString& fileName : filesInDir) {
        if (!localDir.remove(fileName)) {
            qWarning("%s: removing failed", qPrintable(fileName));
        }
    }

    QCOMPARE((int)localDir.entryList(QDir::Files).count(), 0);
    QString subDirectory = path;
    subDirectory.remove(QRegExp(QStringLiteral("^.*/")));
    localDir.cdUp();
    localDir.rmpath(subDirectory);
}

void TestFileSystem::_createDir(const QString& path)
{
    QVERIFY(QDir().mkpath(path));
}
