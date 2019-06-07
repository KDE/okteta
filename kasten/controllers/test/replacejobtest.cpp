/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    Copyright 2019 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "replacejobtest.hpp"

// test object
#include "../view/replace/replacejob.hpp"
// Okteta Kasten gui
#include <Kasten/Okteta/ByteArrayView>
// Okteta Kasten core
#include <Kasten/Okteta/ByteArrayDocument>
// Okteta Core
#include <Okteta/PieceTableByteArrayModel>
// Qt
#include <QTest>
#include <QSignalSpy>

void ReplaceJobTest::compare(const Okteta::PieceTableByteArrayModel* byteArrayModel,
                             const QByteArray& expectedData)
{
    QByteArray actualData(byteArrayModel->size(), 'd');
    byteArrayModel->copyTo(reinterpret_cast<Okteta::Byte*>(actualData.data()), 0, byteArrayModel->size());

    QCOMPARE(actualData, expectedData);
}

template<int N> QByteArray byteArrayFromLiteral(const char (&data)[N])
{
   return QByteArray::fromRawData(data, N-1);
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

    for (auto& data : testData) {
        QTest::newRow(QString(data.name+QLatin1String("-forward-frombegin")).toLatin1().constData())
            << data.originalData << data.searchData
            << data.replaceData << data.expectedData << data.expectedReplacementCount
            << Okteta::Address(0) << false;
        QTest::newRow(QString(data.name+QLatin1String("-backward-fromend")).toLatin1().constData())
            << data.originalData << data.searchData
            << data.replaceData << data.expectedData << data.expectedReplacementCount
            << Okteta::Address(data.originalData.length()) << true;
        QTest::newRow(QString(data.name+QLatin1String("-forward-frommiddle")).toLatin1().constData())
            << data.originalData << data.searchData
            << data.replaceData << data.expectedData << data.expectedReplacementCount
            << Okteta::Address(4) << false;
        QTest::newRow(QString(data.name+QLatin1String("-backward-frommiddle")).toLatin1().constData())
            << data.originalData << data.searchData
            << data.replaceData << data.expectedData << data.expectedReplacementCount
            << Okteta::Address(3) << true;
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

    auto byteArray = new Okteta::PieceTableByteArrayModel(originalData);
    auto document = new Kasten::ByteArrayDocument(byteArray, QStringLiteral("init"));
    auto view = new Kasten::ByteArrayView(document, nullptr);

    Okteta::Address  replaceFirstIndex;
    Okteta::Address  replaceLastIndex;
    if (!backwards && startIndex > 0) {
        replaceFirstIndex = startIndex;
        replaceLastIndex =  startIndex - 1;
    } else if (backwards && startIndex < byteArray->size())  {
        replaceFirstIndex = startIndex + 1;
        replaceLastIndex =  startIndex;
    } else {
        replaceFirstIndex = 0;
        replaceLastIndex =  byteArray->size() - 1;
    }

    auto job = new Kasten::ReplaceJob(view, byteArray, nullptr);
    job->setSearchData(searchData);
    job->setReplaceData(replaceData);
    job->setRange(replaceFirstIndex, replaceLastIndex,
                  backwards ? Kasten::FindBackward : Kasten::FindForward);

    QSignalSpy finishedSpy(job, &Kasten::ReplaceJob::finished);

    job->start();
    if (finishedSpy.count() == 0) {
        QVERIFY(finishedSpy.wait());
    }

    QCOMPARE(finishedSpy.count(), 1);

    compare(byteArray, expectedData);

    const QList<QVariant> arguments = finishedSpy.takeFirst();
    QCOMPARE(arguments.at(0).toBool(), true);
    QCOMPARE(arguments.at(1).toInt(), expectedReplacementCount);

    delete job;
    delete view;
    delete document;
}

QTEST_MAIN(ReplaceJobTest)
