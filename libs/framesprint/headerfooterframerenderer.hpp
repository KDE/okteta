/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    Copyright 2007 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#ifndef FRAMESPRINT_HEADERFOOTERFRAMERENDERER_HPP
#define FRAMESPRINT_HEADERFOOTERFRAMERENDERER_HPP

// lib
#include "abstractframerenderer.hpp"
// Qt
#include <QStringList>
#include <QFont>
#include <QColor>
#include <QUrl>

class QString;

class PrintInfo
{
public:
    QUrl url() const;
    int noOfPages() const;

public:
    void setUrl(const QUrl& url);
    void setNoOfPages(int noOfPages);

private:
    QUrl mUrl;
    int mNoOfPages;
};

inline QUrl PrintInfo::url()      const { return mUrl; }
inline int PrintInfo::noOfPages() const { return mNoOfPages; }
inline void PrintInfo::setUrl(const QUrl& url) { mUrl = url; }
inline void PrintInfo::setNoOfPages(int noOfPages) { mNoOfPages = noOfPages; }

class HeaderFooterFrameRenderer : public AbstractFrameRenderer
{
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
    int height() const override;
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
    const PrintInfo* mInfo;

    int mHeight = 0;
    int mWidth = 0;

    int mBoxStyle = NoBox;
//     int mLineWidth = 1;
//     int mBoxMargin = 6;
    QColor mLineColor;
    QColor mBgColor = {Qt::lightGray};
    QColor mFgColor = {Qt::black};
    QFont mFont;

    QStringList mOriginalTextList = {QString(), QString(), QString()};
    QStringList mGloballyReplacedTextList;
};

#endif
