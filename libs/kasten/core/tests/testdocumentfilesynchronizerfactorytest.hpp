/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2007 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_TESTDOCUMENTFILESYNCHRONIZERFACTORYTEST_HPP
#define KASTEN_TESTDOCUMENTFILESYNCHRONIZERFACTORYTEST_HPP

// Qt
#include <QObject>
#include <QByteArray>

class TestFileSystem;
class QUrl;
class QString;
class QByteArray;

class TestDocumentFileSynchronizerFactoryTest : public QObject
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
    void testCreate();
#if 0
    void testLoadFromFile();
    void testLoadSaveFile();
    void testLoadReloadFile();
    void testConnectToFile();

private: // not working tests
    // TODO: need a way besides popups to report problems
    void testLoadFromNotExistingUrl();
    void testLoadFromNotExistingFile();
#endif

private:
    TestFileSystem* mFileSystem;
};

#endif
