/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2007 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_BYTEARRAYRAWFILESYNCHRONIZERTEST_HPP
#define KASTEN_BYTEARRAYRAWFILESYNCHRONIZERTEST_HPP

// Qt
#include <QObject>
// Std
#include <memory>

// namespace Okteta {
class TestFileSystem;
// }

namespace Kasten {

class ByteArrayRawFileSynchronizerTest : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();

private Q_SLOTS: // test functions

private:
    std::unique_ptr<TestFileSystem> mFileSystem;
};

}

#endif
