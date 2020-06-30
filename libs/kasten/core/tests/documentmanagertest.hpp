/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2007, 2011 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_DOCUMENTMANAGERTEST_HPP
#define KASTEN_DOCUMENTMANAGERTEST_HPP

// Qt
#include <QObject>

namespace Kasten {
class AbstractDocument;
}
class QSignalSpy;

class DocumentManagerTest : public QObject
{
    Q_OBJECT

private:
    void checkAdded(QSignalSpy* changedSpy, Kasten::AbstractDocument* document);
    void checkRemoving(QSignalSpy* changedSpy, Kasten::AbstractDocument* document);

private Q_SLOTS: // test functions
    void initTestCase();

    void testConstructor();
    void testAddRemove();
    void testCanClose();

};

#endif
