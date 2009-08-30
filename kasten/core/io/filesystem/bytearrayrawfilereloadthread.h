/*
    This file is part of the Okteta Kasten module, part of the KDE project.

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

#ifndef BYTEARRAYRAWFILERELOADTHREAD_H
#define BYTEARRAYRAWFILERELOADTHREAD_H

// Okteta core
#include <byte.h>
// Qt
#include <QtCore/QThread>

class QString;


namespace Kasten
{

class KByteArrayDocument;


class ByteArrayRawFileReloadThread : public QThread
{
  Q_OBJECT
  public:
    ByteArrayRawFileReloadThread( QObject *parent,
                                  /*KByteArrayDocument *document, */const QString &filePath );
    virtual ~ByteArrayRawFileReloadThread();

  public: // QThread API
    virtual void run();

  public:
    bool success() const;
    Okteta::Byte* data() const;
    int size() const;

  Q_SIGNALS:
    void documentReloaded( bool success );

  protected:
//     KByteArrayDocument *mDocument;
    const QString mFilePath;

    bool mSuccess;
    Okteta::Byte* mData;
    int mSize;
};


inline bool ByteArrayRawFileReloadThread::success() const { return mSuccess; }
inline Okteta::Byte* ByteArrayRawFileReloadThread::data()   const { return mData; }
inline int ByteArrayRawFileReloadThread::size()     const { return mSize; }

}

#endif
