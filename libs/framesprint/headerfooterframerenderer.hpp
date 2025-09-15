/*
    SPDX-FileCopyrightText: 2007 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef FRAMESPRINT_HEADERFOOTERFRAMERENDERER_HPP
#define FRAMESPRINT_HEADERFOOTERFRAMERENDERER_HPP

// lib
#include "printinfo.hpp"
#include "abstractframerenderer.hpp"
// Qt
#include <QFont>
#include <QColor>
// Std
#include <array>

namespace FramesPrint {

class HeaderFooterFrameRenderer : public AbstractFrameRenderer
{
private:
    static constexpr int HorizontalPositionsSize = 3;

public:
    enum BoxStyle
    {
        NoLines = 0,
        NoBackground = 0,
        NoBox = NoLines | NoBackground,
        BackgroundDrawn = 1,
        LineAbove = 2, LineBelow = 4, LinesAtSide = 8,
        Box = LineAbove | LineBelow | LinesAtSide
    };

public:
    explicit HeaderFooterFrameRenderer(const PrintInfo* info);
    ~HeaderFooterFrameRenderer() override;

public: // AbstractFrameRenderer API
    // make this flags?
//     virtual bool hasFixedWidth() const;
//     virtual bool hasFixedHeight() const;
    [[nodiscard]]
    int height() const override;
    [[nodiscard]]
    int width() const override;
//     virtual QSize sizeHint( const QSize &maxSize ) const;
    // only vertical for now...
//     virtual int framesCount() const;

    void prepare() override;
    void renderFrame(QPainter* painter, int frameIndex) override;

public:
    void setWidth(int width);
    void setTexts(const QString& leftText, const QString& centerText, const QString& rightText);
    void setBoxStyle(int boxStyle);

private:
    void calculateHeight();

private:
    const PrintInfo* const mInfo;

    int mHeight = 0;
    int mWidth = 0;

    int mBoxStyle = NoBox;
//     int mLineWidth = 1;
//     int mBoxMargin = 6;
    QColor mLineColor;
    QColor mBgColor = {Qt::lightGray};
    QColor mFgColor = {Qt::black};
    QFont mFont;

    std::array<QString, HorizontalPositionsSize> mOriginalTextList;
    std::array<QString, HorizontalPositionsSize> mGloballyReplacedTextList;
};

}

#endif
