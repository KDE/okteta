/*
    This file is part of the Kakao Framework, part of the KDE project.

    Copyright 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#ifndef QIMAGEIMPULSIVESTREAMREADER_H
#define QIMAGEIMPULSIVESTREAMREADER_H

// lib
#include "int32impulsivestreamreader.h"
// Kakao core
#include <abstractimpulsivestreamreader.h>
// Qt
#include <QtGui/QImageReader>
#include <QtGui/QImage>


class QImageImpulsiveStreamReader : public AbstractImpulsiveStreamReader
{
  public:
    QImageImpulsiveStreamReader();
    virtual ~QImageImpulsiveStreamReader();

  public: // AbstractImpulsiveStreamReader API
    virtual int nextBytesNeeded() const;
    virtual bool readFrom( QIODevice* ioDevice );
    virtual void reset();

  public:
    QImage image() const;

  protected:
    QImage mImage;

    bool mHasRead;

    Int32ImpulsiveStreamReader mSizeReader;
    QImageReader mImageReader;
};


inline QImage QImageImpulsiveStreamReader::image() const { return mImage; }

#endif
