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

#ifndef TESTDOCUMENTFILELOADTHREAD_H
#define TESTDOCUMENTFILELOADTHREAD_H

// Qt
#include <QtCore/QThread>
#include <QtCore/QString>

class TestDocument;

class TestDocumentFileLoadThread : public QThread
{
  Q_OBJECT
  public:
    TestDocumentFileLoadThread( QObject *parent, const QByteArray &header,
                                const QString &filePath );
    virtual ~TestDocumentFileLoadThread();

  public: // QThread API
    virtual void run();

  public:
    TestDocument *document() const;

  Q_SIGNALS:
    void documentRead( TestDocument *document );

  protected:
    const QString mFilePath;
    const QByteArray mHeader;

    TestDocument *mDocument;
};

inline TestDocumentFileLoadThread::TestDocumentFileLoadThread( QObject *parent, const QByteArray &header, const QString &filePath )
 : QThread( parent ), mFilePath( filePath ), mHeader( header ), mDocument( 0 )
{}

inline TestDocument *TestDocumentFileLoadThread::document() const { return mDocument; }

#endif
