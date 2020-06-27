/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2007 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "headerfooterframerenderer.hpp"

// KF
#include <KLocalizedString>
#include <KUser>
// Qt
#include <QLocale>
#include <QHash>
#include <QDateTime>
#include <QPainter>
#include <QFontMetrics>

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
    mOriginalTextList.clear();
    mOriginalTextList << leftText << centerText << rightText;
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
    QHash<char, QString> tagReplacements;

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
    QRegExp tagsPattern(QStringLiteral("%([dDhyYuUfFP])"));

    mGloballyReplacedTextList.clear();

    for (int i = 0; i < 3; ++i) {
        QString text = mOriginalTextList.at(i);
        int pos = 0;
        while (true) {
            pos = tagsPattern.indexIn(text, pos);
            if (pos < 0) {
                break;
            }
            QString replacement = tagReplacements[text[pos + 1].toLatin1()];
            text.replace((uint)pos, sizeOfTag, replacement);
            pos += replacement.length();
        }
        mGloballyReplacedTextList << text;
    }
}

void HeaderFooterFrameRenderer::renderFrame(QPainter* painter, int frameIndex)
{
    painter->setPen(mFgColor);
    painter->setFont(mFont);
//     if( mBoxStyle & BackgroundDrawn )
//         painter->fillRect( 0, 0, mWidth, mHeight, mBgColor );

    const bool isTextFramed = false;// ( mBoxStyle & (Box|BackgroundDrawn) );

    const int horizontalAlign[3] = { Qt::AlignLeft, Qt::AlignHCenter, Qt::AlignRight };
    int verticalAlign = isTextFramed ? Qt::AlignVCenter : Qt::AlignTop;
    int margin = 0;// ( mBoxStyle & (mBoxDrawn || mBackgroundDrawn ) ? mBoxMargin : 0;
//     if ( mBoxStyle & LinesAtSide )
//         margin += mLineWidth;
    const int rightEnd = mWidth - (margin * 2);

    for (int i = 0; i < 3; i++) {
        QString text = mGloballyReplacedTextList[i];
        // substitute locally
        const QString pageNumberTag = QStringLiteral("%p");
        if (text.indexOf(pageNumberTag) != -1) {
            text.replace(pageNumberTag, QString::number(frameIndex + 1)); // TODO: frameIndex != pageNumber in general
        }
        int align = verticalAlign | horizontalAlign[i];

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
