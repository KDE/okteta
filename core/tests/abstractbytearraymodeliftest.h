/*
    This file is part of the Okteta Core library, part of the KDE project.

    Copyright 2006 Friedrich W. H. Kossebau <kossebau@kde.org>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) version 3, or any
    later version accepted by the membership of KDE e.V. (or its
    successor approved by the membership of KDE e.V.), which shall
    act as a proxy defined in Section 6 of version 3 of the license.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef KABSTRACTBYTEARRAYIFTEST_H
#define KABSTRACTBYTEARRAYIFTEST_H


// Qt
#include <QtCore/QObject>

class QSignalSpy;

namespace KDE {
class Section;
}
namespace Okteta {
class AbstractByteArrayModel;
}


class AbstractByteArrayModelIfTest : public QObject
{
  Q_OBJECT

  protected:
    AbstractByteArrayModelIfTest();

  protected: // our API
    virtual Okteta::AbstractByteArrayModel *createByteArrayModel() = 0;
    virtual void deleteByteArrayModel( Okteta::AbstractByteArrayModel *ByteArrayModel ) = 0;

  protected:
    void checkContentsReplaced( int Position, int RemovedLength, int InsertedLength );
    void checkContentsReplaced( const KDE::Section &RemoveSection, int InsertedLength );
    void checkContentsSwapped( int firstStart, int secondStart, int secondLength );
    void checkContentsSwapped( int firstStart, const KDE::Section &secondSection );
    void clearSignalSpys();

    struct KTestData *prepareTestInsert();

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
    //void testReplace();
    void testSwap();
    void testReplaceEqual();
    void testReplaceLess();
    void testReplaceMore();

  private: // used in all tests
    /** pointer to the model to test */
    Okteta::AbstractByteArrayModel *ByteArrayModel;

    QSignalSpy *ContentsChangeListSpy;
};

inline AbstractByteArrayModelIfTest::AbstractByteArrayModelIfTest()  : ByteArrayModel( 0 ) {}

#endif
