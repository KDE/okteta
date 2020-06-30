/*
    This file is part of the Okteta Core library, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef ABSTRACTPIECETABLECHANGEIFTEST_HPP
#define ABSTRACTPIECETABLECHANGEIFTEST_HPP

// Qt
#include <QObject>

namespace KPieceTable {

class AbstractPieceTableChange;
class PieceTable;

class AbstractPieceTableChangeIfTest : public QObject
{
    Q_OBJECT

protected:
    AbstractPieceTableChangeIfTest();

protected: // our API
    virtual AbstractPieceTableChange* createPieceTableChange() = 0;
    virtual void deletePieceTableChange(AbstractPieceTableChange* pieceTableChange) = 0;

    virtual void changePieceTable(PieceTable* pieceTable) = 0;

private:
    struct KTestData* prepareTestInsert();

private Q_SLOTS: // test functions
    void init();
    void cleanup();

    void testMerge();
    void testRevertApply();

private: // used in all tests
    /** pointer to the change to test */
    AbstractPieceTableChange* mPieceTableChange = nullptr;
};

inline AbstractPieceTableChangeIfTest::AbstractPieceTableChangeIfTest() = default;

}

#endif
