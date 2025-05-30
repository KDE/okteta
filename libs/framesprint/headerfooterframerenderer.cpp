/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2007 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "headerfooterframerenderer.hpp"

// KF
#include <KUser>
// Qt
#include <QLocale>
#include <QDateTime>
#include <QPainter>
#include <QFontMetrics>
#include <QRegularExpression>
// Std
#include <unordered_map>

namespace FramesPrint {

HeaderFooterFrameRenderer::HeaderFooterFrameRenderer(const PrintInfo* info)
    : mInfo(info)
{
    calculateHeight();
}

HeaderFooterFrameRenderer::~HeaderFooterFrameRenderer() = default;

int HeaderFooterFrameRenderer::height() const { return mHeight; }
int HeaderFooterFrameRenderer::width() const { return mWidth; }

void HeaderFooterFrameRenderer::setWidth(int width) { mWidth = width; }

void HeaderFooterFrameRenderer::setTexts(const QString& leftText, const QString& centerText, const QString& rightText)
{
    mOriginalTextList = {
        leftText,
        centerText,
        rightText,
    };
}

void HeaderFooterFrameRenderer::setBoxStyle(int boxStyle)
{
    if (mBoxStyle == boxStyle) {
        return;
    }
    mBoxStyle = boxStyle;
    calculateHeight();
}

void HeaderFooterFrameRenderer::calculateHeight()
{
    const QFontMetrics fontMetrics(mFont);

    mHeight = fontMetrics.height();

//     const bool hasSpaceAbove = ( mBoxStyle & (BackgroundDrawn|LineAbove) );
//     const bool hasSpacedBelow = ( mBoxStyle & (BackgroundDrawn|LineBelow) );
//     if( !(hasSpaceAbove||hasSpacedBelow) )
    mHeight += fontMetrics.leading();
//     else
//     {
//         if( hasSpaceAbove )
//         {
//         mHeight += mBoxMargin * 2;
//
//         }
//     }
}

void HeaderFooterFrameRenderer::prepare()
{
    const QDateTime dateTime = QDateTime::currentDateTime();
    const KUser user(KUser::UseRealUserID);
    const QUrl url = mInfo->url();

    // create list of replacements
    std::unordered_map<char, QString> tagReplacements;

    QLocale locale;
    tagReplacements['d'] = locale.toString(dateTime, QLocale::ShortFormat);
    tagReplacements['D'] = locale.toString(dateTime, QLocale::LongFormat);
    tagReplacements['h'] = locale.toString(dateTime.time(), QLocale::ShortFormat);
    tagReplacements['y'] = locale.toString(dateTime.date(), QLocale::ShortFormat);
    tagReplacements['Y'] = locale.toString(dateTime.date(), QLocale::LongFormat);
    tagReplacements['u'] = user.loginName();
    tagReplacements['U'] = user.property(KUser::FullName).toString();
//     tagReplacements['f'] = isSelection ? i18n("(Selection of) %1", url.fileName()) : url.fileName();
    tagReplacements['f'] = url.fileName();
    tagReplacements['F'] = url.toDisplayString(QUrl::PrettyDecoded | QUrl::PreferLocalFile);
    tagReplacements['P'] = QString::number(mInfo->noOfPages());

    // create text with globally replaced tags
    const int sizeOfTag = 2;
    QRegularExpression tagsPattern(QStringLiteral("%([dDhyYuUfFP])"));

    for (int i = 0; i < HorizontalPositionsSize; ++i) {
        QString text = mOriginalTextList.at(i);
        int pos = 0;
        while (true) {
            pos = text.indexOf(tagsPattern, pos);
            if (pos < 0) {
                break;
            }
            QString replacement = tagReplacements[text[pos + 1].toLatin1()];
            text.replace((uint)pos, sizeOfTag, replacement);
            pos += replacement.length();
        }
        mGloballyReplacedTextList[i] = text;
    }
}

void HeaderFooterFrameRenderer::renderFrame(QPainter* painter, int frameIndex)
{
    painter->setPen(mFgColor);
    painter->setFont(mFont);
//     if( mBoxStyle & BackgroundDrawn )
//         painter->fillRect( 0, 0, mWidth, mHeight, mBgColor );

    const bool isTextFramed = false;// ( mBoxStyle & (Box|BackgroundDrawn) );

    const std::array<Qt::AlignmentFlag, HorizontalPositionsSize> horizontalAligns = {
        Qt::AlignLeft,
        Qt::AlignHCenter,
        Qt::AlignRight,
    };
    const Qt::AlignmentFlag verticalAlign = isTextFramed ? Qt::AlignVCenter : Qt::AlignTop;
    int margin = 0;// ( mBoxStyle & (mBoxDrawn || mBackgroundDrawn ) ? mBoxMargin : 0;
//     if ( mBoxStyle & LinesAtSide )
//         margin += mLineWidth;
    const int rightEnd = mWidth - (margin * 2);

    for (int i = 0; i < HorizontalPositionsSize; ++i) {
        QString text = mGloballyReplacedTextList[i];
        // substitute locally
        const QString pageNumberTag = QStringLiteral("%p");
        text.replace(pageNumberTag, QString::number(frameIndex + 1)); // TODO: frameIndex != pageNumber in general
        const int align = verticalAlign | horizontalAligns[i];

        painter->drawText(margin, 0, rightEnd, mHeight, align, text);
    }

//     if ( !isTextFramed )
    {
//         painter->drawLine( 0, mHeight-mLineWidth, mWidth, mHeight-mLineWidth );
//         painter->drawLine( 0, maxHeight + mBoxMargin - 1, headerWidth, maxHeight + mBoxMargin - 1 );
//         painter->drawRect( 0, 0, pdmWidth, pdmHeight);
//             paint.drawLine(0, headerHeight, headerWidth, headerHeight);
    }
}

}
