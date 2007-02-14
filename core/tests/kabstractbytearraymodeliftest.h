/***************************************************************************
                          kabstractbytearraymodeliftest.h  -  description
                            -------------------
    begin                : Son Mai 7 2006
    copyright            : (C) 2006 by Friedrich W. H. Kossebau
    email                : Friedrich.W.H@Kossebau.de
***************************************************************************/

/***************************************************************************
*                                                                         *
*   This library is free software; you can redistribute it and/or         *
*   modify it under the terms of the GNU Library General Public           *
*   License version 2 as published by the Free Software Foundation.       *
*                                                                         *
***************************************************************************/


#ifndef KABSTRACTBYTEARRAYIFTEST_H
#define KABSTRACTBYTEARRAYIFTEST_H


// qt specific
#include <QObject>

class QSignalSpy;

namespace KHE {
class KSection;
}
namespace KHECore {
class KAbstractByteArrayModel;
}


class KAbstractByteArrayModelIfTest : public QObject
{
  Q_OBJECT

  protected:
    KAbstractByteArrayModelIfTest();

  protected: // our API
    virtual KHECore::KAbstractByteArrayModel *createByteArrayModel() = 0;
    virtual void deleteByteArrayModel( KHECore::KAbstractByteArrayModel *ByteArrayModel ) = 0;

  protected:
    void checkContentsReplaced( int Position, int RemovedLength, int InsertedLength );
    void checkContentsReplaced( const KHE::KSection &RemoveSection, int InsertedLength );
    void checkContentsMoved( int Destination, int Source, int MovedLength );
    void checkContentsMoved( int Destination, const KHE::KSection &SourceSection );
    void checkContentsChanged( const KHE::KSection &Section );
    void checkContentsChanged( int Start, int End );
    void clearSignalSpys();

    struct KTestData *prepareTestInsert();

  private Q_SLOTS: // test functions
    void init();
    void cleanup();

    void testModified();
    void testCopyTo();
    void testFill();
    void testSetGet();

    void testRemove();
    void testInsertAtBegin();
    void testInsertAtMid();
    void testInsertAtEnd();
    //void testReplace();
    void testMove();
    void testReplaceEqual();
    void testReplaceLess();
    void testReplaceMore();

  private: // used in all tests
    /** pointer to the model to test */
    KHECore::KAbstractByteArrayModel *ByteArrayModel;

    QSignalSpy *ContentsChangedSpy;
    QSignalSpy *ContentsReplacedSpy;
    QSignalSpy *ContentsMovedSpy;
};

inline KAbstractByteArrayModelIfTest::KAbstractByteArrayModelIfTest()  : ByteArrayModel( 0 ) {}

#endif
