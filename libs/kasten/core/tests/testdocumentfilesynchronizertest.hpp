/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2007 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_TESTDOCUMENTFILESYNCHRONIZERTEST_HPP
#define KASTEN_TESTDOCUMENTFILESYNCHRONIZERTEST_HPP

// Qt
#include <QObject>
#include <QByteArray>

class TestFileSystem;
class QString;
class QUrl;

class TestDocumentFileSynchronizerTest : public QObject
{
    Q_OBJECT

private:
    void writeToFile(const QString& filePath, const QByteArray& data, const QByteArray& header = QByteArray());
    void checkFileContent(const QUrl& fileUrl, const QByteArray& data, const QByteArray& header = QByteArray());

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void init();

private Q_SLOTS: // test functions
    void testLoadFromFile();
    void testLoadSaveFile();
    void testLoadReloadFile();
    void testChangeFile();
    void testConnectToFile();

private: // not working tests
    // TODO: need a way besides popups to report problems
    void testHeader();
    void testLoadFromNotExistingUrl();
    void testLoadFromNotExistingFile();

private:
    TestFileSystem* mFileSystem;
};

#endif
