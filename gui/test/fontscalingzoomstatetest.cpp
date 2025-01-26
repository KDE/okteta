/*
    This file is part of the Okteta Gui library, made within the KDE community.

    SPDX-FileCopyrightText: 2025 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "fontscalingzoomstatetest.hpp"

// test object
#include <fontscalingzoomstate.hpp>
// Qt
#include <QTest>

namespace Okteta {

//---------------------------------------------------------------- Tests -----

void FontScalingZoomStateTest::testNormalZoomScale()
{
    FontScalingZoomState fontScalingZoomState;

    double zoomScale = fontScalingZoomState.scale();
    QCOMPARE(zoomScale, 1.0);

    QFont font = fontScalingZoomState.font();
    const int initialPointSize = font.pointSize();

    // bigger font
    font.setPointSize(initialPointSize + 1);
    fontScalingZoomState.setFont(font);
    zoomScale = fontScalingZoomState.scale();
    QCOMPARE(zoomScale, 1.0);

    // smaller font
    font.setPointSize(initialPointSize - 1);
    fontScalingZoomState.setFont(font);
    zoomScale = fontScalingZoomState.scale();
    QCOMPARE(zoomScale, 1.0);
}

void FontScalingZoomStateTest::testSetFontSize()
{
    FontScalingZoomState fontScalingZoomState;

    const int minimumFontSize = fontScalingZoomState.minimumFontSize();
    const int maximumFontSize = fontScalingZoomState.maximumFontSize();

    // assumption: default font is not matching the smallest zoom state
    QVERIFY(minimumFontSize < fontScalingZoomState.fontSize());

    for (int fontSize = minimumFontSize; fontSize <= maximumFontSize; ++fontSize) {
        const bool hasFontSizeChanged = fontScalingZoomState.zoomTo(fontSize);
        QVERIFY(hasFontSizeChanged);
        const int currentFontSize = fontScalingZoomState.fontSize();
        QCOMPARE(currentFontSize, fontSize);
    }

    // try setting too small size, from non-minimum one
    bool hasFontSizeChanged = fontScalingZoomState.zoomTo(minimumFontSize - 1);
    QVERIFY(hasFontSizeChanged);
    int currentFontSize = fontScalingZoomState.fontSize();
    QCOMPARE(currentFontSize, minimumFontSize);
    // try setting too small size, from minimum one
    hasFontSizeChanged = fontScalingZoomState.zoomTo(minimumFontSize - 1);
    QVERIFY(!hasFontSizeChanged);
    currentFontSize = fontScalingZoomState.fontSize();
    QCOMPARE(currentFontSize, minimumFontSize);

    // try setting too big size, from non-minimum one
    hasFontSizeChanged = fontScalingZoomState.zoomTo(maximumFontSize + 1);
    QVERIFY(hasFontSizeChanged);
    currentFontSize = fontScalingZoomState.fontSize();
    QCOMPARE(currentFontSize, maximumFontSize);
    // try setting too small size, from minimum one
    hasFontSizeChanged = fontScalingZoomState.zoomTo(maximumFontSize + 1);
    QVERIFY(!hasFontSizeChanged);
    currentFontSize = fontScalingZoomState.fontSize();
    QCOMPARE(currentFontSize, maximumFontSize);
}

void FontScalingZoomStateTest::testZoomLevels()
{
    FontScalingZoomState fontScalingZoomState;

    const int zoomOutLevelsSize = fontScalingZoomState.outLevelsSize();
    for (int zoomOutLevel = zoomOutLevelsSize; zoomOutLevel > 0; --zoomOutLevel) {
        const double zoomScaleForLevel = fontScalingZoomState.scaleForLevel(-zoomOutLevel);
        const int zoomLevel = fontScalingZoomState.levelForScale(zoomScaleForLevel);
        QCOMPARE(zoomLevel, -zoomOutLevel);

        const bool hasScaleChanged = fontScalingZoomState.setScale(zoomScaleForLevel);
        QVERIFY(hasScaleChanged);
        const double zoomScale = fontScalingZoomState.scale();
        QCOMPARE(zoomScale, zoomScaleForLevel);
    }

    const int zoomInLevelsSize = fontScalingZoomState.inLevelsSize();
    for (int zoomInLevel = 1; zoomInLevel <= zoomInLevelsSize; ++zoomInLevel) {
        const double zoomScaleForLevel = fontScalingZoomState.scaleForLevel(zoomInLevel);
        const int zoomLevel = fontScalingZoomState.levelForScale(zoomScaleForLevel);
        QCOMPARE(zoomLevel, zoomInLevel);

        const bool hasScaleChanged = fontScalingZoomState.setScale(zoomScaleForLevel);
        QVERIFY(hasScaleChanged);
        const double zoomScale = fontScalingZoomState.scale();
        QCOMPARE(zoomScale, zoomScaleForLevel);
    }
}

}

QTEST_MAIN(Okteta::FontScalingZoomStateTest)

#include "moc_fontscalingzoomstatetest.cpp"
