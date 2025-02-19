/*
    This file is part of the Okteta Gui library, made within the KDE community.

    SPDX-FileCopyrightText: 2025 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "abstractbytearrayviewiftest.hpp"

// test object
#include <abstractbytearrayview.hpp>
// Okteta core
#include <Okteta/ByteArrayModel>
// Qt
#include <QFont>
#include <QTest>
#include <QSignalSpy>

namespace Okteta {

//---------------------------------------------------------------- Tests -----

void AbstractByteArrayViewIfTest::init()
{
    constexpr Byte BlankChar = '\0';
    constexpr Size fillSize = 10;

    m_byteArrayModel = new ByteArrayModel(fillSize);
    m_byteArrayModel->fill(BlankChar);

    m_byteArrayView = createByteArrayView();
    m_byteArrayView->setByteArrayModel(m_byteArrayModel);

    m_zoomScaleSpy = new QSignalSpy(m_byteArrayView, SIGNAL(zoomLevelChanged(double)));
}

void AbstractByteArrayViewIfTest::cleanup()
{
    delete m_zoomScaleSpy;
    m_zoomScaleSpy = nullptr;

    deleteByteArrayView(m_byteArrayView);
    m_byteArrayView = nullptr;

    delete m_byteArrayModel;
    m_byteArrayModel = nullptr;
}

void AbstractByteArrayViewIfTest::checkZoomScaleChanged(double zoomScaleExpected)
{
    QVERIFY(m_zoomScaleSpy->isValid());
    QCOMPARE(m_zoomScaleSpy->count(), 1);
    const QList<QVariant> arguments = m_zoomScaleSpy->takeFirst();
    const auto zoomScale = qvariant_cast<double>(arguments.at(0));
    QCOMPARE(zoomScale, zoomScaleExpected);
}

void AbstractByteArrayViewIfTest::clearSignalSpys()
{
    m_zoomScaleSpy->clear();
}

//---------------------------------------------------------------- Tests -----

void AbstractByteArrayViewIfTest::testNormalZoomScale()
{
    double zoomScale = m_byteArrayView->zoomScale();
    QCOMPARE(zoomScale, 1.0);

    QFont font = m_byteArrayView->font();
    const int initialPointSize = font.pointSize();

    // bigger font
    font.setPointSize(initialPointSize + 1);
    m_byteArrayView->setFont(font);
    zoomScale = m_byteArrayView->zoomScale();
    QCOMPARE(zoomScale, 1.0);

    // smaller font
    font.setPointSize(initialPointSize - 1);
    m_byteArrayView->setFont(font);
    zoomScale = m_byteArrayView->zoomScale();
    QCOMPARE(zoomScale, 1.0);
}

void AbstractByteArrayViewIfTest::testZoomLevels()
{
    const int zoomOutLevelsSize = m_byteArrayView->zoomOutLevelsSize();
    for (int zoomOutLevel = zoomOutLevelsSize; zoomOutLevel > 0; --zoomOutLevel) {
        m_zoomScaleSpy->clear();

        const double zoomScaleForLevel = m_byteArrayView->zoomScaleForLevel(-zoomOutLevel);
        const int zoomLevel = m_byteArrayView->zoomLevelForScale(zoomScaleForLevel);
        QCOMPARE(zoomLevel, -zoomOutLevel);

        m_byteArrayView->setZoomScale(zoomScaleForLevel);
        const double zoomScale = m_byteArrayView->zoomScale();
        QCOMPARE(zoomScale, zoomScaleForLevel);

        checkZoomScaleChanged(zoomScaleForLevel);
    }

    const int zoomInLevelsSize = m_byteArrayView->zoomInLevelsSize();
    for (int zoomInLevel = 1; zoomInLevel <= zoomInLevelsSize; ++zoomInLevel) {
        m_zoomScaleSpy->clear();

        const double zoomScaleForLevel = m_byteArrayView->zoomScaleForLevel(zoomInLevel);
        const int zoomLevel = m_byteArrayView->zoomLevelForScale(zoomScaleForLevel);
        QCOMPARE(zoomLevel, zoomInLevel);

        m_byteArrayView->setZoomScale(zoomScaleForLevel);
        const double zoomScale = m_byteArrayView->zoomScale();
        QCOMPARE(zoomScale, zoomScaleForLevel);

        checkZoomScaleChanged(zoomScaleForLevel);
    }
}

}

#include "moc_abstractbytearrayviewiftest.cpp"
