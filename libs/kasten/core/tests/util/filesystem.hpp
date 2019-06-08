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
