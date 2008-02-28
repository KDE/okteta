/*
    This file is part of the Okteta Kakao module, part of the KDE project.

    Copyright 2006-2008 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#ifndef KBYTEARRAYDOCUMENT_H
#define KBYTEARRAYDOCUMENT_H

// lib
// #include "kbytearrayinterface.h"
// Kakao core
#include <kiversionable.h>
#include <kabstractdocument.h>
// Qt
#include <QtCore/QString>

namespace KHECore {
class KAbstractByteArrayModel;
class KPieceTableByteArrayModel;
}


class KByteArrayDocument : public KAbstractDocument, public KDE::If::Versionable//, public KDE::If::ByteArray
{
    Q_OBJECT
    Q_INTERFACES(KDE::If::Versionable)
//     Q_INTERFACES(KDE::If::ByteArray)

  public:
    explicit KByteArrayDocument( const QString &initDescription );
    KByteArrayDocument( KHECore::KPieceTableByteArrayModel *byteArray, const QString &initDescription );
    virtual ~KByteArrayDocument();

  public: // KAbstractDocument API
    virtual QString title() const;
    virtual QString typeName() const;
    virtual QString mimeType() const;
    virtual SynchronizationStates synchronizationStates() const;
    virtual bool isModifiable() const;
    virtual bool isReadOnly() const;
    virtual void setReadOnly( bool isReadOnly );

  public: // KDE::If::Versionable
    virtual int versionIndex() const;
    virtual KDocumentVersionData versionData( int versionIndex ) const;
    virtual int versionCount() const;
    virtual void revertToVersionByIndex( int versionIndex );

  public: // KDE::If::ByteArray
    virtual KHECore::KAbstractByteArrayModel *content() const;

  public:
    void setTitle( const QString &title );

  Q_SIGNALS: // KDE::If::Versionable
    virtual void revertedToVersionIndex( int versionIndex );
    virtual void headVersionDataChanged( const KDocumentVersionData &versionData );
    virtual void headVersionChanged( int newHeadVersionIndex );

  protected Q_SLOTS:
    void onModelModification( bool newState );
    void onHeadVersionDescriptionChanged( const QString &newDescription );

  protected:
    KHECore::KPieceTableByteArrayModel *mByteArray;

    mutable QString mTitle;

    const QString mInitDescription;
};

#endif
