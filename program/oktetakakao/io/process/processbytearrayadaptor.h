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

#ifndef PROCESSBYTEARRAYADAPTOR_H
#define PROCESSBYTEARRAYADAPTOR_H

// lib
#include <bytearraychange.h>
// Kakao core
#include <abstractprocessdocument.h>
// Qt
#include <QtCore/QObject>
#include <QtCore/QList>

class KByteArrayDocument;

// Q_DECLARE_METATYPE(KHECore::ByteArrayChange)

class ProcessByteArrayAdaptor : public AbstractProcessDocument
{
  Q_OBJECT

  public:
    explicit ProcessByteArrayAdaptor( KByteArrayDocument* byteArrayDocument );

  public: // AbstractProcessDocument API
    virtual QString title() const;
    virtual QString id() const;
    virtual int versionIndex() const;

  public:
    QByteArray baseData() const;
    int versionCount() const;

  public:
    QList<KHECore::ByteArrayChange> changes( int firstVersionIndex, int lastVersionIndex ) const;

  public: // set/action
    void revertToVersionByIndex( int versionIndex );

  public Q_SLOTS:
    void onChangesDone( const QList<KHECore::ByteArrayChange>& changes,
                        int oldVersionIndex, int newVersionIndex );
    void onRevertedToVersionIndex( int versionIndex );
//     void onHeadVersionDescriptionChanged( const QString& versionDescription );
//     void onHeadVersionChanged( int newHeadVersionIndex );

  Q_SIGNALS:
    void changesDone( const QList<KHECore::ByteArrayChange>& changes,
                      int oldVersionIndex, int newVersionIndex );
    void revertedToVersionIndex( int versionIndex );
//     void headVersionDescriptionChanged( const QString& versionDescription );
//     void headVersionChanged( int newHeadVersionIndex );

  protected:
    KByteArrayDocument* mByteArrayDocument;
};

#endif
