/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2007 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef OKTETATEST_FILESYSTEM_HPP
#define OKTETATEST_FILESYSTEM_HPP

// Qt
#include <QString>

class TestFileSystem
{
public:
    explicit TestFileSystem(const QString& name);
    TestFileSystem(const TestFileSystem& name) = delete;
    ~TestFileSystem();

public:
    TestFileSystem& operator=(const TestFileSystem& name) = delete;

public:
    /// tries to remove the given subPath
    void removeDir(const QString& subPath);
    /// tries to create the given subPath
    void createDir(const QString& subPath);
    ///
    QString createFilePath(const QString& fileName, const QString& subPath = QString());

private:
    void _removeDir(const QString& path);
    /// tries to create the given subPath
    void _createDir(const QString& path);

private:
    QString mBasePath;
};

#endif
