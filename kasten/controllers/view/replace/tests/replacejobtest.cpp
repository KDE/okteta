/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2019 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "replacejobtest.hpp"

// test object
#include "replacejob.hpp"
#include "replaceuserqueryable.hpp"
// Okteta Kasten gui
#include <Kasten/Okteta/ByteArrayView>
#include <Kasten/Okteta/ByteArrayViewProfileSynchronizer>
// Okteta Kasten core
#include <Kasten/Okteta/ByteArrayDocument>
// Okteta Core
#include <Okteta/PieceTableByteArrayModel>
// Qt
#include <QTest>
#include <QSignalSpy>
// Std
#include <memory>

Q_DECLARE_METATYPE(QVector<Kasten::ReplaceBehaviour>)

class TestReplaceUserQueryAgent : public QObject
                                , public Kasten::If::ReplaceUserQueryable
{
    Q_OBJECT
    Q_INTERFACES(
        Kasten::If::ReplaceUserQueryable
    )

public:
    explicit TestReplaceUserQueryAgent(const QVector<Kasten::ReplaceBehaviour>& replies)
        : m_replies(replies)
    {}

public:
    [[nodiscard]]
    int noOfReplacements() const { return m_noOfReplacements; }

public: // If::ReplaceUserQueryable API
    void queryContinue(Kasten::FindDirection direction, int noOfReplacements) override;
    void queryReplaceCurrent() override;

Q_SIGNALS: // If::ReplaceUserQueryable API
    void queryContinueFinished(bool result) override;
    void queryReplaceCurrentFinished(Kasten::ReplaceBehaviour result) override;

private:
    QVector<Kasten::ReplaceBehaviour> m_replies;
    int m_noOfReplacements = 0;
};

void TestReplaceUserQueryAgent::queryContinue(Kasten::FindDirection direction, int noOfReplacements)
{
    Q_UNUSED(direction)
    m_noOfReplacements = noOfReplacements;
    Q_EMIT queryContinueFinished(true);
}

void TestReplaceUserQueryAgent::queryReplaceCurrent()
{
    Q_ASSERT(!m_replies.isEmpty());
    Q_EMIT queryReplaceCurrentFinished(m_replies.takeFirst());
}


void ReplaceJobTest::compare(const Okteta::PieceTableByteArrayModel* byteArrayModel,
                             const QByteArray& expectedData)
{
    QByteArray actualData(byteArrayModel->size(), 'd');
    std::ignore = byteArrayModel->copyTo(reinterpret_cast<Okteta::Byte*>(actualData.data()), 0, byteArrayModel->size());

    QCOMPARE(actualData, expectedData);
}

template<int N> QByteArray byteArrayFromLiteral(const char (&data)[N])
{
   return QByteArray::fromRawData(data, N-1);
}

void ReplaceJobTest::init()
{
    qRegisterMetaType<Okteta::AddressRange>("Okteta::AddressRange");
}

void ReplaceJobTest::testReplace_data()
{
    QTest::addColumn<QByteArray>("originalData");
    QTest::addColumn<QByteArray>("searchData");
    QTest::addColumn<QByteArray>("replaceData");
    QTest::addColumn<QByteArray>("expectedData");
    QTest::addColumn<int>("expectedReplacementCount");
    QTest::addColumn<Okteta::Address>("startIndex");
    QTest::addColumn<bool>("backwards");
    QTest::addColumn<QVector<Kasten::ReplaceBehaviour>>("replies");

    const struct {
        QString name;
        QByteArray originalData;
        QByteArray expectedData;
        int expectedReplacementCount;
        QByteArray searchData;
        QByteArray replaceData;
    }
    testData[] = {{
        QStringLiteral("full-1-1"),
        byteArrayFromLiteral("11111111"),
        byteArrayFromLiteral("22222222"),
        8,
        byteArrayFromLiteral("1"),
        byteArrayFromLiteral("2")
    }, {
        QStringLiteral("full-1-2"),
        byteArrayFromLiteral("11111111"),
        byteArrayFromLiteral("2323232323232323"),
        8,
        byteArrayFromLiteral("1"),
        byteArrayFromLiteral("23"),
    }, {
        QStringLiteral("full-2-1"),
        byteArrayFromLiteral("1313131313131313"),
        byteArrayFromLiteral("22222222"),
        8,
        byteArrayFromLiteral("13"),
        byteArrayFromLiteral("2")
    }, {
        QStringLiteral("partial-1-1"),
        byteArrayFromLiteral("01001000"),
        byteArrayFromLiteral("02002000"),
        2,
        byteArrayFromLiteral("1"),
        byteArrayFromLiteral("2")
    }, {
        QStringLiteral("partial-1-2"),
        byteArrayFromLiteral("01001000"),
        byteArrayFromLiteral("0230023000"),
        2,
        byteArrayFromLiteral("1"),
        byteArrayFromLiteral("23"),
    }, {
        QStringLiteral("partial-2-1"),
        byteArrayFromLiteral("0130013000"),
        byteArrayFromLiteral("02002000"),
        2,
        byteArrayFromLiteral("13"),
        byteArrayFromLiteral("2")
    }};

    for (const auto& data : testData) {
        QTest::newRow(QString(data.name+QLatin1String("-forward-frombegin")).toLatin1().constData())
            << data.originalData << data.searchData
            << data.replaceData << data.expectedData << data.expectedReplacementCount
            << Okteta::Address(0) << false << QVector<Kasten::ReplaceBehaviour>();
        QTest::newRow(QString(data.name+QLatin1String("-backward-fromend")).toLatin1().constData())
            << data.originalData << data.searchData
            << data.replaceData << data.expectedData << data.expectedReplacementCount
            << Okteta::Address(data.originalData.length()) << true << QVector<Kasten::ReplaceBehaviour>();
        QTest::newRow(QString(data.name+QLatin1String("-forward-frommiddle")).toLatin1().constData())
            << data.originalData << data.searchData
            << data.replaceData << data.expectedData << data.expectedReplacementCount
            << Okteta::Address(4) << false << QVector<Kasten::ReplaceBehaviour>();
        QTest::newRow(QString(data.name+QLatin1String("-backward-frommiddle")).toLatin1().constData())
            << data.originalData << data.searchData
            << data.replaceData << data.expectedData << data.expectedReplacementCount
            << Okteta::Address(3) << true << QVector<Kasten::ReplaceBehaviour>();
    }

    {
        const QByteArray originalData = byteArrayFromLiteral("11111111");
        const QByteArray expectedData = byteArrayFromLiteral("22222222");
        const int expectedReplacementCount = 8;
        const QByteArray searchData = byteArrayFromLiteral("1");
        const QByteArray replaceData = byteArrayFromLiteral("2");

        QTest::newRow("replies-forward-replaceall")
            << originalData << searchData
            << replaceData << expectedData << expectedReplacementCount
            << Okteta::Address(0) << false
            << QVector<Kasten::ReplaceBehaviour>{ Kasten::ReplaceAll };

        QTest::newRow("replies-forward-replace8x")
            << originalData << searchData
            << replaceData << expectedData << expectedReplacementCount
            << Okteta::Address(0) << false
            << QVector<Kasten::ReplaceBehaviour>(8, Kasten::ReplaceCurrent);

        QTest::newRow("replies-forward-replace3xreplaceall")
            << originalData << searchData
            << replaceData << expectedData << expectedReplacementCount
            << Okteta::Address(0) << false
            << QVector<Kasten::ReplaceBehaviour>{Kasten::ReplaceCurrent, Kasten::ReplaceCurrent, Kasten::ReplaceCurrent, Kasten::ReplaceAll };
    }

    {
        QTest::newRow("replies-forward-replaceskipalternate")
            << byteArrayFromLiteral("11111111") << byteArrayFromLiteral("1")
            << byteArrayFromLiteral("2") << byteArrayFromLiteral("12121212") << 4
            << Okteta::Address(0) << false
            << QVector<Kasten::ReplaceBehaviour>{ Kasten::SkipCurrent, Kasten::ReplaceCurrent,
                Kasten::SkipCurrent, Kasten::ReplaceCurrent, Kasten::SkipCurrent, Kasten::ReplaceCurrent,
                Kasten::SkipCurrent, Kasten::ReplaceCurrent };
    }

    {
        const QByteArray originalData = byteArrayFromLiteral("11111111");
        const QByteArray expectedData = byteArrayFromLiteral("11111111");
        const int expectedReplacementCount = 0;
        const QByteArray searchData = byteArrayFromLiteral("1");
        const QByteArray replaceData = byteArrayFromLiteral("2");

        QTest::newRow("replies-forward-cancel")
            << originalData << searchData
            << replaceData << expectedData << expectedReplacementCount
            << Okteta::Address(0) << false
            << QVector<Kasten::ReplaceBehaviour>{ Kasten::CancelReplacing };

        QTest::newRow("replies-forward-skip8x")
            << originalData << searchData
            << replaceData << expectedData << expectedReplacementCount
            << Okteta::Address(0) << false
            << QVector<Kasten::ReplaceBehaviour>(8, Kasten::SkipCurrent);

        QTest::newRow("replies-forward-skip3xcancel")
            << originalData << searchData
            << replaceData << expectedData << expectedReplacementCount
            << Okteta::Address(0) << false
            << QVector<Kasten::ReplaceBehaviour>{Kasten::SkipCurrent, Kasten::SkipCurrent, Kasten::SkipCurrent, Kasten::CancelReplacing };
    }
}

void ReplaceJobTest::testReplace()
{
    QFETCH(Okteta::Address, startIndex);
    QFETCH(bool, backwards);
    QFETCH(QByteArray, originalData);
    QFETCH(QByteArray, searchData);
    QFETCH(QByteArray, replaceData);
    QFETCH(QByteArray, expectedData);
    QFETCH(int, expectedReplacementCount);
    QFETCH(QVector<Kasten::ReplaceBehaviour>, replies);

    std::unique_ptr<TestReplaceUserQueryAgent> queryAgent;
    if (!replies.isEmpty()) {
        queryAgent = std::make_unique<TestReplaceUserQueryAgent>(replies);
    }

    auto byteArray = std::make_unique<Okteta::PieceTableByteArrayModel>(originalData);
    auto* const rawByteArray = byteArray.get();
    auto document = std::make_unique<Kasten::ByteArrayDocument>(std::move(byteArray), QStringLiteral("init"));
    auto view = std::make_unique<Kasten::ByteArrayView>(document.get(), nullptr);

    Okteta::Address  replaceFirstIndex;
    Okteta::Address  replaceLastIndex;
    if (!backwards && startIndex > 0) {
        replaceFirstIndex = startIndex;
        replaceLastIndex =  startIndex - 1;
    } else if (backwards && startIndex < rawByteArray->size())  {
        replaceFirstIndex = startIndex + 1;
        replaceLastIndex =  startIndex;
    } else {
        replaceFirstIndex = 0;
        replaceLastIndex =  rawByteArray->size() - 1;
    }

    auto job = std::make_unique<Kasten::ReplaceJob>(view.get(), rawByteArray, queryAgent.get());
    job->setSearchData(searchData);
    job->setReplaceData(replaceData);
    job->setRange(replaceFirstIndex, replaceLastIndex,
                  backwards ? Kasten::FindBackward : Kasten::FindForward);
    job->setDoPrompt(queryAgent != nullptr);

    QSignalSpy finishedSpy(job.get(), &Kasten::ReplaceJob::finished);

    job->start();
    if (finishedSpy.isEmpty()) {
        QVERIFY(finishedSpy.wait());
    }

    QCOMPARE(finishedSpy.size(), 1);

    compare(rawByteArray, expectedData);

    const int beforeWrap = queryAgent ? queryAgent->noOfReplacements() : 0;
    const QList<QVariant> arguments = finishedSpy.takeFirst();
    QCOMPARE(arguments.at(0).toBool(), true);
    QCOMPARE(arguments.at(1).toInt() + beforeWrap, expectedReplacementCount);
}

QTEST_MAIN(ReplaceJobTest)

#include "replacejobtest.moc"
#include "moc_replacejobtest.cpp"
