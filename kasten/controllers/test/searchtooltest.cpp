/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2023 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "searchtooltest.hpp"

// test object
#include <view/search/searchtool.hpp>
#include <view/search/searchuserqueryable.hpp>
// Okteta Kasten gui
#include <Kasten/Okteta/ByteArrayView>
// Okteta Kasten core
#include <Kasten/Okteta/ByteArrayDocument>
// Okteta Core
#include <Okteta/PieceTableByteArrayModel>
// Qt
#include <QTest>

Q_DECLARE_METATYPE(Okteta::AddressRange)
namespace Okteta {
char* toString(const AddressRange& addressRange)
{
    const QByteArray string(QByteArrayLiteral("Okteta::AddressRange(") +
                            QByteArray::number(addressRange.start()) + '-' +
                            QByteArray::number(addressRange.end()) + ')');
    return qstrdup(string.data());
}
}

Q_DECLARE_METATYPE(Kasten::FindDirection)

enum WrapQueryStatus
{
    WrapNotQueried,
    WrapQueried
};
Q_DECLARE_METATYPE(WrapQueryStatus)
char* toString(const WrapQueryStatus& status)
{
    if (status == WrapNotQueried) {
        return qstrdup("WrapNotQueried");
    }
    return qstrdup("WrapQueried");
}

template<int N> QByteArray byteArrayFromLiteral(const char (&data)[N])
{
   return QByteArray::fromRawData(data, N-1);
}

class TestSearchUserQueryable : public Kasten::If::SearchUserQueryable
{
public:
    TestSearchUserQueryable() = default;

public:
    WrapQueryStatus wrapQueryStatus() const { return mWrapQueryStatus; }

public: // If::SearchUserQueryable API
    bool queryContinue(Kasten::FindDirection direction) const override;

private:
    mutable WrapQueryStatus mWrapQueryStatus = WrapNotQueried;
};

bool TestSearchUserQueryable::queryContinue(Kasten::FindDirection direction) const
{
    Q_UNUSED(direction);
    mWrapQueryStatus = WrapQueried;
    return true;
}

/*
    Markup format:
    "_":  cursor, ignored currently when there is a selection -> ViewData.cursorPosition
    "[]": selection -> ViewData.selection
    "{}": search match result -> ViewData.match
*/
ViewData SearchToolTest::parseToViewData(const QByteArray& viewMarkup) const
{
    ViewData result;

    bool cursorSeen = false;
    bool selectionBeginSeen = false;
    bool selectionEndSeen = false;
    bool matchBeginSeen = false;
    bool matchEndSeen = false;

    for (const char c : viewMarkup) {
        if (c == '_') {
            Q_ASSERT(!cursorSeen);
            result.cursorPosition = result.data.size();
            cursorSeen = true;
        } else if (c == '[') {
            Q_ASSERT(!selectionBeginSeen);
            result.selection.setStart(result.data.size());
            selectionBeginSeen = true;
        } else if (c == ']') {
            Q_ASSERT(selectionBeginSeen && !selectionEndSeen);
            result.selection.setEnd(result.data.size() - 1);
            selectionEndSeen = true;
        } else if (c == '{') {
            Q_ASSERT(!matchBeginSeen);
            result.match.setStart(result.data.size());
            matchBeginSeen = true;
        } else if (c == '}') {
            Q_ASSERT(matchBeginSeen && !matchEndSeen);
            result.match.setEnd(result.data.size() - 1);
            matchEndSeen = true;
        } else  {
            result.data.append(c);
        }
    }

    return result;
}

void SearchToolTest::testParseToViewData_data()
{
    QTest::addColumn<QByteArray>("viewMarkup");
    QTest::addColumn<Okteta::Address>("cursorPosition");
    QTest::addColumn<Okteta::AddressRange>("selection");
    QTest::addColumn<Okteta::AddressRange>("match");
    QTest::addColumn<QByteArray>("data");

    QTest::newRow("cursor-begin-selection-none")
        << byteArrayFromLiteral("_aaa")
        << Okteta::Address(0)
        << Okteta::AddressRange()
        << Okteta::AddressRange()
        << byteArrayFromLiteral("aaa");

    QTest::newRow("cursor-end-selection-none")
        << byteArrayFromLiteral("aaa_")
        << Okteta::Address(3)
        << Okteta::AddressRange()
        << Okteta::AddressRange()
        << byteArrayFromLiteral("aaa");

    QTest::newRow("cursor-begin-selection-all")
        << byteArrayFromLiteral("_[aaa]")
        << Okteta::Address(0)
        << Okteta::AddressRange(0, 2)
        << Okteta::AddressRange()
        << byteArrayFromLiteral("aaa");

    QTest::newRow("cursor-end-selection-all")
        << byteArrayFromLiteral("[aaa]_")
        << Okteta::Address(3)
        << Okteta::AddressRange(0, 2)
        << Okteta::AddressRange()
        << byteArrayFromLiteral("aaa");

    QTest::newRow("cursor-begin-selection-firsthalf")
        << byteArrayFromLiteral("_[aaa]bbb")
        << Okteta::Address(0)
        << Okteta::AddressRange(0, 2)
        << Okteta::AddressRange()
        << byteArrayFromLiteral("aaabbb");

    QTest::newRow("cursor-end-selection-firsthalf")
        << byteArrayFromLiteral("[aaa]_bbb")
        << Okteta::Address(3)
        << Okteta::AddressRange(0, 2)
        << Okteta::AddressRange()
        << byteArrayFromLiteral("aaabbb");

    QTest::newRow("cursor-begin-selection-secondhalf")
        << byteArrayFromLiteral("{aaa}_[bbb]")
        << Okteta::Address(3)
        << Okteta::AddressRange(3, 5)
        << Okteta::AddressRange(0, 2)
        << byteArrayFromLiteral("aaabbb");

    QTest::newRow("cursor-end-selection-secondhalf")
        << byteArrayFromLiteral("aaa{[bbb]}_")
        << Okteta::Address(6)
        << Okteta::AddressRange(3, 5)
        << Okteta::AddressRange(3, 5)
        << byteArrayFromLiteral("aaabbb");
}

void SearchToolTest::testParseToViewData()
{
    QFETCH(const QByteArray, viewMarkup);
    QFETCH(const Okteta::Address, cursorPosition);
    QFETCH(const Okteta::AddressRange, selection);
    QFETCH(const Okteta::AddressRange, match);
    QFETCH(const QByteArray, data);

    const ViewData viewData = parseToViewData(viewMarkup);

    QCOMPARE(viewData.data, data);
    QCOMPARE(viewData.cursorPosition, cursorPosition);
    QCOMPARE(viewData.selection, selection);
    QCOMPARE(viewData.match, match);
}

void SearchToolTest::testSearch_data()
{
    QTest::addColumn<QByteArray>("viewMarkup");
    QTest::addColumn<QByteArray>("searchData");
    QTest::addColumn<Kasten::FindDirection>("direction");
    QTest::addColumn<WrapQueryStatus>("wrapQueryStatus");

    QTest::newRow("cursor-begin-selection-none-forward-single")
        << byteArrayFromLiteral("_{a}bbbaaa")
        << byteArrayFromLiteral("a")
        << Kasten::FindForward
        << WrapNotQueried;

    QTest::newRow("cursor-begin-selection-none-forward-single-nonlatin")
        << byteArrayFromLiteral("_{\xf0}bbb\xf0\xf0\xf0")
        << byteArrayFromLiteral("\xf0")
        << Kasten::FindForward
        << WrapNotQueried;

    QTest::newRow("cursor-begin-selection-none-forward-multi")
        << byteArrayFromLiteral("_{aaa}bbbaaa")
        << byteArrayFromLiteral("aaa")
        << Kasten::FindForward
        << WrapNotQueried;

    QTest::newRow("cursor-begin-selection-none-forward-multi-nonlatin")
        << byteArrayFromLiteral("_{aa\xf0}bbbaa\xf0")
        << byteArrayFromLiteral("aa\xf0")
        << Kasten::FindForward
        << WrapNotQueried;

    QTest::newRow("cursor-begin-selection-partmatch-forward-multi")
        << byteArrayFromLiteral("_{[a]aa}bbbaaa")
        << byteArrayFromLiteral("aaa")
        << Kasten::FindForward
        << WrapNotQueried;

    QTest::newRow("cursor-begin-selection-match-forward-single")
        << byteArrayFromLiteral("_[a]bbb{a}")
        << byteArrayFromLiteral("a")
        << Kasten::FindForward
        << WrapNotQueried;

    QTest::newRow("cursor-begin-selection-match-forward-multi")
        << byteArrayFromLiteral("_[aaa]bbb{aaa}")
        << byteArrayFromLiteral("aaa")
        << Kasten::FindForward
        << WrapNotQueried;

    QTest::newRow("cursor-begin-selection-match-overlap-forward-multi")
        << byteArrayFromLiteral("_[aaa]abb{aaa}")
        << byteArrayFromLiteral("aaa")
        << Kasten::FindForward
        << WrapNotQueried;

    QTest::newRow("cursor-begin-selection-match-overlap-duplicate-forward-multi")
        << byteArrayFromLiteral("_[aaa]{aaa}aaa")
        << byteArrayFromLiteral("aaa")
        << Kasten::FindForward
        << WrapNotQueried;

    QTest::newRow("cursor-beforeend-selection-none-forward-single")
        << byteArrayFromLiteral("aaabbb_{a}")
        << byteArrayFromLiteral("a")
        << Kasten::FindForward
        << WrapNotQueried;

    QTest::newRow("cursor-beforeend-selection-none-forward-multi")
        << byteArrayFromLiteral("aaabbb_{aaa}")
        << byteArrayFromLiteral("aaa")
        << Kasten::FindForward
        << WrapNotQueried;

    QTest::newRow("cursor-beforeend-selection-partmatch-forward-multi")
        << byteArrayFromLiteral("aaabbb{[aa]_a}")
        << byteArrayFromLiteral("aaa")
        << Kasten::FindForward
        << WrapNotQueried;

    QTest::newRow("cursor-beforeend-selection-wrongpartmatch1-forward-multi")
        << byteArrayFromLiteral("{aaa}bbbaa[a]_")
        << byteArrayFromLiteral("aaa")
        << Kasten::FindForward
        << WrapQueried;

    QTest::newRow("cursor-beforeend-selection-wrongpartmatch2-forward-multi")
        << byteArrayFromLiteral("{aaa}bbba[aa]_")
        << byteArrayFromLiteral("aaa")
        << Kasten::FindForward
        << WrapQueried;

    QTest::newRow("cursor-end-selection-none-forward-single")
        << byteArrayFromLiteral("{a}bbbaaa_")
        << byteArrayFromLiteral("a")
        << Kasten::FindForward
        << WrapQueried;

    QTest::newRow("cursor-end-selection-none-forward-multi")
        << byteArrayFromLiteral("{aaa}bbbaaa_")
        << byteArrayFromLiteral("aaa")
        << Kasten::FindForward
        << WrapQueried;

    QTest::newRow("cursor-end-selection-match-forward-single")
        << byteArrayFromLiteral("{a}bbb[a]_")
        << byteArrayFromLiteral("a")
        << Kasten::FindForward
        << WrapQueried;

    QTest::newRow("cursor-end-selection-match-forward-multi")
        << byteArrayFromLiteral("{aaa}bbb[aaa]_")
        << byteArrayFromLiteral("aaa")
        << Kasten::FindForward
        << WrapQueried;

    QTest::newRow("cursor-end-selection-none-backward-single")
        << byteArrayFromLiteral("aaabbb{a}_")
        << byteArrayFromLiteral("a")
        << Kasten::FindBackward
        << WrapNotQueried;

    QTest::newRow("cursor-end-selection-none-backward-multi")
        << byteArrayFromLiteral("aaabbb{aaa}_")
        << byteArrayFromLiteral("aaa")
        << Kasten::FindBackward
        << WrapNotQueried;

    QTest::newRow("cursor-end-selection-partmatch-backward-multi")
        << byteArrayFromLiteral("aaabbb{aa[a]}_")
        << byteArrayFromLiteral("aaa")
        << Kasten::FindBackward
        << WrapNotQueried;

    QTest::newRow("cursor-end-selection-match-backward-single")
        << byteArrayFromLiteral("{a}bbb[a]_")
        << byteArrayFromLiteral("a")
        << Kasten::FindBackward
        << WrapNotQueried;

    QTest::newRow("cursor-end-selection-match-backward-multi")
        << byteArrayFromLiteral("{aaa}bbb[aaa]_")
        << byteArrayFromLiteral("aaa")
        << Kasten::FindBackward
        << WrapNotQueried;

    QTest::newRow("cursor-end-selection-match-overlap-backward-multi")
        << byteArrayFromLiteral("{aaa}bba[aaa]_")
        << byteArrayFromLiteral("aaa")
        << Kasten::FindBackward
        << WrapNotQueried;

    QTest::newRow("cursor-end-selection-match-overlap-duplicate-backward-multi")
        << byteArrayFromLiteral("aaa{aaa}[aaa]_")
        << byteArrayFromLiteral("aaa")
        << Kasten::FindBackward
        << WrapNotQueried;

    QTest::newRow("cursor-begin-selection-none-backward-single")
        << byteArrayFromLiteral("_aaabbb{a}")
        << byteArrayFromLiteral("a")
        << Kasten::FindBackward
        << WrapQueried;

    QTest::newRow("cursor-begin-selection-none-backward-single-nonlatin")
        << byteArrayFromLiteral("_\xf0\xf0\xf0""bbb{\xf0}")
        << byteArrayFromLiteral("\xf0")
        << Kasten::FindBackward
        << WrapQueried;

    QTest::newRow("cursor-begin-selection-none-backward-multi")
        << byteArrayFromLiteral("_aaabbb{aaa}")
        << byteArrayFromLiteral("aaa")
        << Kasten::FindBackward
        << WrapQueried;

    QTest::newRow("cursor-begin-selection-wrongpartmatch1-backward-multi")
        << byteArrayFromLiteral("[a]_aabbb{aaa}")
        << byteArrayFromLiteral("aaa")
        << Kasten::FindBackward
        << WrapQueried;

    QTest::newRow("cursor-begin-selection-wrongpartmatch2-backward-multi")
        << byteArrayFromLiteral("[aa]_abbb{aaa}")
        << byteArrayFromLiteral("aaa")
        << Kasten::FindBackward
        << WrapQueried;

    QTest::newRow("cursor-begin-selection-match-backward-single")
        << byteArrayFromLiteral("[a]_bbb{a}")
        << byteArrayFromLiteral("a")
        << Kasten::FindBackward
        << WrapQueried;

    QTest::newRow("cursor-begin-selection-match-backward-multi")
        << byteArrayFromLiteral("[aaa]_bbb{aaa}")
        << byteArrayFromLiteral("aaa")
        << Kasten::FindBackward
        << WrapQueried;
}

void SearchToolTest::testSearch()
{
    QFETCH(const QByteArray, viewMarkup);
    QFETCH(const QByteArray, searchData);
    QFETCH(const Kasten::FindDirection, direction);
    QFETCH(const WrapQueryStatus, wrapQueryStatus);

    const ViewData viewData = parseToViewData(viewMarkup);

    auto* byteArray = new Okteta::PieceTableByteArrayModel(viewData.data);
    auto* document = new Kasten::ByteArrayDocument(byteArray, QStringLiteral("init"));
    auto* view = new Kasten::ByteArrayView(document, nullptr);

    auto* queryAgent = new TestSearchUserQueryable();
    auto* tool = new Kasten::SearchTool();
    tool->setTargetModel(view);
    tool->setSearchData(searchData);
    tool->setUserQueryAgent(queryAgent);

    if (viewData.selection.isValid()) {
        view->setSelection(viewData.selection.start(), viewData.selection.end());
    } else {
        view->setCursorPosition(viewData.cursorPosition);
    }

    const Okteta::AddressRange beforeMatch = view->selection();

    tool->search(direction, true, false);

    const Okteta::AddressRange match = view->selection();

    QCOMPARE(match, viewData.match);
    QCOMPARE(queryAgent->wrapQueryStatus(), wrapQueryStatus);

    delete tool;
    delete view;
    delete document;
    delete queryAgent;
}

QTEST_MAIN(SearchToolTest)
