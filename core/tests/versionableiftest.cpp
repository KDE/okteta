/*
    This file is part of the Okteta Core library, made within the KDE community.

    Copyright 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "versionableiftest.h"

// test object
#include <versionable.h>
// Qt
#include <QtTest/QtTest>
#include <QtTest/QSignalSpy>

namespace Okteta
{

// ---------------------------------------------------------------- Tests -----

void VersionableIfTest::init()
{
    mObject = createVersionable();

    mVersionableControl = qobject_cast<Okteta::Versionable*>( mObject );

    mRevertedToVersionIndexSpy =
        new QSignalSpy( mObject, SIGNAL(revertedToVersionIndex(int)) );
    mHeadVersionDescriptionChangedSpy =
        new QSignalSpy( mObject, SIGNAL(headVersionDescriptionChanged(QString)) );
    mHeadVersionChangedSpy =
        new QSignalSpy( mObject, SIGNAL(headVersionChanged(int)) );
}

void VersionableIfTest::cleanup()
{
    deleteVersionable( mObject );

    delete mRevertedToVersionIndexSpy;
    delete mHeadVersionDescriptionChangedSpy;
    delete mHeadVersionChangedSpy;
}

void VersionableIfTest::clearSignalSpys()
{
    mRevertedToVersionIndexSpy->clear();
    mHeadVersionDescriptionChangedSpy->clear();
    mHeadVersionChangedSpy->clear();
}

void VersionableIfTest::checkRevertedToVersionIndex( int versionIndex )
{
    QVERIFY( mRevertedToVersionIndexSpy->isValid() );
    QCOMPARE( mRevertedToVersionIndexSpy->count(), 1 );
    const QList<QVariant> arguments = mRevertedToVersionIndexSpy->takeFirst();
    QCOMPARE( arguments.at(0).toInt(), versionIndex );
}

void VersionableIfTest::checkHeadVersionDescriptionChanged( const QString &versionDescription )
{
    QVERIFY( mHeadVersionDescriptionChangedSpy->isValid() );
    QCOMPARE( mHeadVersionDescriptionChangedSpy->count(), 1 );
    const QList<QVariant> arguments = mHeadVersionDescriptionChangedSpy->takeFirst();
    QCOMPARE( arguments.at(0).toString(), versionDescription );
}

void VersionableIfTest::checkHeadVersionChanged( int newHeadVersionIndex )
{
    QVERIFY( mHeadVersionChangedSpy->isValid() );
    QCOMPARE( mHeadVersionChangedSpy->count(), 1 );
    QList<QVariant> arguments = mHeadVersionChangedSpy->takeFirst();
    QCOMPARE( arguments.at(0).toInt(), newHeadVersionIndex );
}


// ---------------------------------------------------------------- Tests -----

void VersionableIfTest::testBegin()
{
    QCOMPARE( mVersionableControl->versionIndex(), 0 );
    QCOMPARE( mVersionableControl->versionCount(), 1 );
}

#if 0
void VersionableIfTest::testFill()
{


weiter an ende
1. neue Version
2. Versionsänderung
weiter in mitte
weiter an anfang





  // can we alter the buffer at all?
  if( mVersionable->isReadOnly() )
    // skip
    return;

  static const unsigned int FillSize = 10;
  unsigned int Size = mVersionable->size();
  Section FillSection = Section::fromWidth( 0, Size );

  FixedSizeByteArrayModel Copy( Size, PaintChar );

  // fill() all
  mVersionable->setModified( false );
  mVersionable->fill( BlankChar );
  clearSignalSpys();

  mVersionable->fill( PaintChar );
  QCOMPARE( Copy.compare(*mVersionable), 0 );
  QVERIFY( mVersionable->isModified() );
  checkContentsReplaced( FillSection, FillSection.width() );
  checkContentsChanged( 0, mVersionable->size()-1 );

  // fill() at begin
  FillSection.set( 0, FillSize );
  mVersionable->setModified( false );
  mVersionable->fill( BlankChar );
  clearSignalSpys();

  mVersionable->fill( PaintChar, FillSection );
  QCOMPARE( Copy.compare(*mVersionable, FillSection), 0 );
  QCOMPARE( mVersionable->datum(FillSection.nextBehindEnd()), BlankChar );
  QVERIFY( mVersionable->isModified() );
  checkContentsReplaced( FillSection, FillSection.width() );
  checkContentsChanged( FillSection );

  // fill() at end
  mVersionable->setModified( false );
  mVersionable->fill( BlankChar );
  FillSection.moveToEnd( Size - 1 );
  clearSignalSpys();

  mVersionable->fill( PaintChar, FillSection );
  QCOMPARE( mVersionable->datum(FillSection.nextBeforeStart()), BlankChar );
  QCOMPARE( Copy.compare(*mVersionable, FillSection, FillSection.start()), 0 );
  QVERIFY( mVersionable->isModified() );
  checkContentsReplaced( FillSection, FillSection.width() );
  checkContentsChanged( FillSection );

  // fill() at mid
  mVersionable->setModified( false );
  mVersionable->fill( BlankChar );
  FillSection.moveToStart( Size/2 );
  clearSignalSpys();

  mVersionable->fill( PaintChar, FillSection );
  QCOMPARE( mVersionable->datum(FillSection.nextBeforeStart()), BlankChar );
  QCOMPARE( Copy.compare(*mVersionable, FillSection, FillSection.start()), 0 );
  QCOMPARE( mVersionable->datum(FillSection.nextBehindEnd()), BlankChar );
  QVERIFY( mVersionable->isModified() );
  checkContentsReplaced( FillSection, FillSection.width() );
  checkContentsChanged( FillSection );
}
#endif

}
