/*
    This file is part of the Okteta Kakao module, part of the KDE project.

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

#ifndef INT32IMPULSIVESTREAMREADER_H
#define INT32IMPULSIVESTREAMREADER_H

// Kakao core
#include <abstractimpulsivestreamreader.h>
// Qt
#include <QtCore/Qt>

class Int32ImpulsiveStreamReader : public AbstractImpulsiveStreamReader
{
  public:
    Int32ImpulsiveStreamReader();
    virtual ~Int32ImpulsiveStreamReader();

  public: // AbstractImpulsiveStreamReader API
    virtual int nextBytesNeeded() const;
    virtual bool readFrom( QIODevice* ioDevice );
    virtual void reset();

  public:
    qint32 data() const;

  protected:
    qint32 mData;
    bool mHasRead;
};


inline qint32 Int32ImpulsiveStreamReader::data() const { return mData; }

#endif
