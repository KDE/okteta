/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2007 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_TESTDOCUMENTTEST_HPP
#define KASTEN_TESTDOCUMENTTEST_HPP

// sut
#include <kastencore.hpp>
// Qt
#include <QObject>

class QSignalSpy;
class QString;

class TestDocumentTest : public QObject
{
    Q_OBJECT

private:
    void checkTitleChanged(QSignalSpy* titleChangedSpy, const QString& title);
    void checkContentFlagsChanged(QSignalSpy* changedSpy, Kasten::ContentFlags contentFlags);

private Q_SLOTS: // test functions
    void testPlainConstructor();
    void testDataConstructor();
    void testChangeData();
    void testSetTitle();
    void testSetContentFlags();

};

#endif
