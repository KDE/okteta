/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2007 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "filesystem.hpp"

// Qt
#include <QDir>
#include <QFile>
#include <QTest>

// TODO QStringLiteral
static constexpr char basePath[] = "/.kde-unit-test/";

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
