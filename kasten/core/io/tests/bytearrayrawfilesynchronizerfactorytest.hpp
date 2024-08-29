/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2007-2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_BYTEARRAYRAWFILESYNCHRONIZERFACTORYTEST_HPP
#define KASTEN_BYTEARRAYRAWFILESYNCHRONIZERFACTORYTEST_HPP

// Qt
#include <QObject>
// Std
#include <memory>

class TestFileSystem;

namespace Kasten {

class ByteArrayRawFileSynchronizerFactoryTest : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();

private Q_SLOTS: // test functions
    void testCreate();
    void testLoadFromUrl();
    void testNewSaveAsToUrl();

private: // not working tests
    void testLoadFromNotExistingUrl();

private:
    std::unique_ptr<TestFileSystem> mFileSystem;
};

}

#endif
