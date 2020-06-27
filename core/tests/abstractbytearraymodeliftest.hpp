/*
    This file is part of the Okteta Core library, made within the KDE community.

    SPDX-FileCopyrightText: 2006 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef ABSTRACTBYTEARRAYMODELIFTEST_HPP
#define ABSTRACTBYTEARRAYMODELIFTEST_HPP

// lib
#include <addressrange.hpp>
// Qt
#include <QObject>

class QSignalSpy;

namespace Okteta {
class AbstractByteArrayModel;

class AbstractByteArrayModelIfTest : public QObject
{
    Q_OBJECT

protected:
    AbstractByteArrayModelIfTest();

protected: // our API
    virtual AbstractByteArrayModel* createByteArrayModel() = 0;
    virtual void deleteByteArrayModel(AbstractByteArrayModel* byteArrayModel) = 0;
    virtual bool byteArrayModelSizeCanBeChanged() const;

private:
    void checkContentsReplaced(Address Position, Size removedLength, int insertedLength);
    void checkContentsReplaced(const AddressRange& removeSection, int insertedLength);
    void checkContentsSwapped(Address firstStart, Address secondStart, Size secondLength);
    void checkContentsSwapped(Address firstStart, const AddressRange& secondSection);
    void clearSignalSpys();

    struct KTestData* prepareTestInsert();

private Q_SLOTS: // test functions
    void init();
    void cleanup();

    void testModified();
    void testSetReadOnly();
    void testCopyTo();
    void testFill();
    void testSetGet();

    void testRemove();
    void testInsertAtBegin();
    void testInsertAtMid();
    void testInsertAtEnd();
    // void testReplace();
    void testSwap();
    void testReplaceEqual();
    void testReplaceLess();
    void testReplaceMore();

private: // used in all tests
    /** pointer to the model to test */
    AbstractByteArrayModel* mByteArrayModel = nullptr;

    QSignalSpy* ContentsChangeListSpy = nullptr;
};

inline AbstractByteArrayModelIfTest::AbstractByteArrayModelIfTest() = default;

}

#endif
