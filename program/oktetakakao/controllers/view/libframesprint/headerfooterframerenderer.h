/***************************************************************************
                          headerfooterframerenderer.h  -  description
                             -------------------
    begin                : Tue Dec 11 2007
    copyright            : 2007 by Friedrich W. H. Kossebau
    email                : kossebau@kde.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU General Public License           *
 *   version 2 as published by the Free Software Foundation.               *
 *                                                                         *
 ***************************************************************************/


#ifndef HEADERFOOTERFRAMERENDERER_H
#define HEADERFOOTERFRAMERENDERER_H

// lib
#include "abstractframerenderer.h"
// KDE
#include <KUrl>
// Qt
#include <QtCore/QStringList>
#include <QtGui/QFont>
#include <QtGui/QColor>

class QString;

class PrintInfo
{
  public:
    KUrl url() const;
    int noOfPages() const;

  public:
    void setUrl( const KUrl &url );
    void setNoOfPages( int noOfPages );

  protected:
    KUrl mUrl;
    int mNoOfPages;
};

inline KUrl PrintInfo::url()      const { return mUrl; }
inline int PrintInfo::noOfPages() const { return mNoOfPages; }
inline void PrintInfo::setUrl( const KUrl &url ) { mUrl = url; }
inline void PrintInfo::setNoOfPages( int noOfPages ) { mNoOfPages = noOfPages; }


class HeaderFooterFrameRenderer : public AbstractFrameRenderer
{
  public:
    enum BoxStyle {
        NoLines = 0,
        NoBackground = 0,
        NoBox = NoLines | NoBackground,
        BackgroundDrawn = 1,
        LineAbove = 2, LineBelow = 4, LinesAtSide = 8,
        Box = LineAbove | LineBelow | LinesAtSide };

  public:
    explicit HeaderFooterFrameRenderer( const PrintInfo* info );
    virtual ~HeaderFooterFrameRenderer();

  public: // AbstractFrameRenderer API
    // make this flags?
//     virtual bool hasFixedWidth() const;
//     virtual bool hasFixedHeight() const;
    virtual int height() const;
    virtual int width() const;
//     virtual QSize sizeHint( const QSize &maxSize ) const;
    // only vertical for now...
//     virtual int framesCount() const;

    virtual void prepare();
    virtual void renderFrame( QPainter *painter, int frameIndex );

  public:
    void setWidth( int width );
    void setTexts( const QString &leftText, const QString &centerText, const QString &rightText );
    void setBoxStyle( int boxStyle );

  protected:
    void calculateHeight();

  protected:
    const PrintInfo* mInfo;

    int mHeight;
    int mWidth;

    int mBoxStyle;
    int mLineWidth;
    int mBoxMargin;
    QColor mLineColor;
    QColor mBgColor;
    QColor mFgColor;
    QFont mFont;

    QStringList mOriginalTextList;
    QStringList mGloballyReplacedTextList;
};

#endif
