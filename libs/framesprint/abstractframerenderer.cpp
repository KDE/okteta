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
