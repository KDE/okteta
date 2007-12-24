/***************************************************************************
                          abstractframerenderer.cpp  -  description
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

#include "abstractframerenderer.h"

// Qt
#include <QtCore/QPoint>
#include <QtCore/QSize>
#include <QtCore/QRect>

class AbstractFrameRenderer::Private
{
  public:
    Private();
  public:
    int mX;
    int mY;
};
AbstractFrameRenderer::Private::Private() : mX( 0 ), mY( 0 ) {}


AbstractFrameRenderer::AbstractFrameRenderer() : d( new Private() ) {}

QPoint AbstractFrameRenderer::pos () const { return QPoint(d->mX, d->mY); }
QSize AbstractFrameRenderer::size () const { return QSize(width(),height()); }
QRect AbstractFrameRenderer::rect () const { return QRect(pos(),size()); }

int AbstractFrameRenderer::x() const { return d->mX; }
int AbstractFrameRenderer::y() const { return d->mY; }

void AbstractFrameRenderer::setPos( int x, int y ) { d->mX = x; d->mY = y; }
void AbstractFrameRenderer::setPos( const QPoint &point ) { d->mX = point.x(); d->mY = point.y(); }

AbstractFrameRenderer::~AbstractFrameRenderer() { delete d; }
