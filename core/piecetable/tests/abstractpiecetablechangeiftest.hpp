/*
    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef ABSTRACTPIECETABLECHANGEIFTEST_HPP
#define ABSTRACTPIECETABLECHANGEIFTEST_HPP

// Qt
#include <QObject>
// Std
#include <memory>

namespace KPieceTable {

class AbstractPieceTableChange;
class PieceTable;

class AbstractPieceTableChangeIfTest : public QObject
{
    Q_OBJECT

public:
    ~AbstractPieceTableChangeIfTest() override;

protected:
    AbstractPieceTableChangeIfTest();

protected: // our API
    virtual std::unique_ptr<AbstractPieceTableChange> createPieceTableChange() = 0;
    virtual void deletePieceTableChange(std::unique_ptr<AbstractPieceTableChange>&& pieceTableChange) = 0;

    virtual void changePieceTable(PieceTable* pieceTable) = 0;

private:
    struct KTestData* prepareTestInsert();

private Q_SLOTS: // test functions
    void init();
    void cleanup();

    void testMerge();
    void testRevertApply();

private: // used in all tests
    /** the change to test */
    std::unique_ptr<AbstractPieceTableChange> mPieceTableChange;
};

inline AbstractPieceTableChangeIfTest::AbstractPieceTableChangeIfTest() = default;

}

#endif
