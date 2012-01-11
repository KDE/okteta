/*
    This file is part of the Okteta Kasten module, made within the KDE community.

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

class QFile;


namespace Kasten2
{

class ByteArrayDocument;


class ByteArrayRawFileLoadThread : public QThread
{
  Q_OBJECT
  public:
    ByteArrayRawFileLoadThread( QObject* parent,
                                QFile* file );
    virtual ~ByteArrayRawFileLoadThread();

  public: // QThread API
    virtual void run();

  public:
    ByteArrayDocument* document() const;

    const QString& errorString() const;

  Q_SIGNALS:
    void documentRead( ByteArrayDocument* document );

  protected:
    QFile* mFile;

    ByteArrayDocument* mDocument;
    QString mErrorString;
};


inline ByteArrayRawFileLoadThread::ByteArrayRawFileLoadThread( QObject* parent, QFile* file )
  : QThread( parent ),
    mFile( file ),
    mDocument( 0 )
{}

inline ByteArrayDocument* ByteArrayRawFileLoadThread::document() const { return mDocument; }
inline const QString& ByteArrayRawFileLoadThread::errorString()  const { return mErrorString; }

}

#endif
