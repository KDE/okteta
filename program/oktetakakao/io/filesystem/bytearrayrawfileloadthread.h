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

#ifndef BYTEARRAYRAWFILELOADTHREAD_H
#define BYTEARRAYRAWFILELOADTHREAD_H

// Qt
#include <QtCore/QThread>

class KByteArrayDocument;
class QString;

class ByteArrayRawFileLoadThread : public QThread
{
  Q_OBJECT
  public:
    ByteArrayRawFileLoadThread( QObject *parent,
                                const QString &filePath );
    virtual ~ByteArrayRawFileLoadThread();

  public: // QThread API
    virtual void run();

  public:
    KByteArrayDocument *document() const;

  Q_SIGNALS:
    void documentRead( KByteArrayDocument *document );

  protected:
    const QString mFilePath;

    KByteArrayDocument *mDocument;
};

inline ByteArrayRawFileLoadThread::ByteArrayRawFileLoadThread( QObject *parent, const QString &filePath )
 : QThread( parent ), mFilePath( filePath ), mDocument( 0 )
{}

inline KByteArrayDocument *ByteArrayRawFileLoadThread::document() const { return mDocument; }

#endif
