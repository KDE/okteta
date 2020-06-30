/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2007 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "abstractframerenderer.hpp"

// Qt
#include <QPoint>
#include <QSize>
#include <QRect>

class AbstractFrameRendererPrivate
{
public:
    AbstractFrameRendererPrivate();

public:
    int mX = 0;
    int mY = 0;
};
AbstractFrameRendererPrivate::AbstractFrameRendererPrivate() = default;

AbstractFrameRenderer::AbstractFrameRenderer() : d(new AbstractFrameRendererPrivate()) {}
AbstractFrameRenderer::~AbstractFrameRenderer() = default;

QPoint AbstractFrameRenderer::pos() const { return {d->mX, d->mY}; }
QSize AbstractFrameRenderer::size() const { return {width(), height()}; }
QRect AbstractFrameRenderer::rect() const { return {pos(), size()}; }

int AbstractFrameRenderer::x() const { return d->mX; }
int AbstractFrameRenderer::y() const { return d->mY; }

void AbstractFrameRenderer::setPos(int x, int y) { d->mX = x; d->mY = y; }
void AbstractFrameRenderer::setPos(QPoint point) { d->mX = point.x(); d->mY = point.y(); }

