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
// Std
#include <memory>

class QSignalSpy;

namespace Okteta {
class AbstractByteArrayModel;
struct KTestData;

class AbstractByteArrayModelIfTest : public QObject
{
    Q_OBJECT

public:
    ~AbstractByteArrayModelIfTest();

protected:
    AbstractByteArrayModelIfTest();

protected: // our API
    virtual std::unique_ptr<AbstractByteArrayModel> createByteArrayModel() = 0;
    virtual void deleteByteArrayModel(std::unique_ptr<AbstractByteArrayModel>&& byteArrayModel) = 0;
    virtual bool byteArrayModelSizeCanBeChanged() const;

private:
    void checkContentsReplaced(Address offset, Size removeLength, int insertLength);
    void checkContentsReplaced(const AddressRange& removeRange, int insertLength);
    void checkContentsSwapped(Address firstStart, Address secondStart, Size secondLength);
    void checkContentsSwapped(Address firstStart, const AddressRange& secondSection);
    void clearSignalSpys();

    std::unique_ptr<KTestData> prepareTestInsert() const;

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
    /** the model to test */
    std::unique_ptr<AbstractByteArrayModel> mByteArrayModel;

    std::unique_ptr<QSignalSpy> ContentsChangeListSpy;
};

}

#endif
