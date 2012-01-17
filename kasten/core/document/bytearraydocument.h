/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    Copyright 2006-2009 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#ifndef BYTEARRAYDOCUMENT_H
#define BYTEARRAYDOCUMENT_H

// lib
#include "oktetakastencore_export.h"
// #include "kbytearrayinterface.h"
// Kasten core
#include <person.h>
#include <userlistable.h>
#include <versionable.h>
#include <abstractdocument.h>
// Qt
#include <QtCore/QString>

namespace Okteta {
class AbstractByteArrayModel;
class PieceTableByteArrayModel;
}


namespace Kasten2
{

class OKTETAKASTENCORE_EXPORT ByteArrayDocument : public AbstractDocument,
                                                  public If::Versionable,
                                                  public If::UserListable//,
//                                                    public If::ByteArray
{
  Q_OBJECT
  Q_INTERFACES(
    Kasten2::If::Versionable
    Kasten2::If::UserListable
//     Kasten2::If::ByteArray
  )

  public:
    explicit ByteArrayDocument( const QString &initDescription );
    ByteArrayDocument( Okteta::PieceTableByteArrayModel *byteArray, const QString &initDescription );
    virtual ~ByteArrayDocument();

  public: // AbstractModel API
    virtual QString title() const;
    virtual bool isModifiable() const;
    virtual bool isReadOnly() const;
    virtual void setReadOnly( bool isReadOnly );

  public: // AbstractDocument API
    virtual QString typeName() const;
    virtual QString mimeType() const;
    virtual ContentFlags contentFlags() const;

  public: // If::Versionable
    virtual int versionIndex() const;
    virtual DocumentVersionData versionData( int versionIndex ) const;
    virtual int versionCount() const;
    virtual void revertToVersionByIndex( int versionIndex );

  public: // If::UserListable
    virtual Person owner() const;
    virtual QList<Person> userList() const;

  public: // If::ByteArray
    virtual Okteta::AbstractByteArrayModel *content() const;

  public:
    void setTitle( const QString &title );

    void setOwner( const Person& owner );
    void addUsers( const QList<Person>& users );
    void removeUsers( const QList<Person>& users );

  Q_SIGNALS: // If::Versionable
    /*virtual*/ void revertedToVersionIndex( int versionIndex );
    /*virtual*/ void headVersionDataChanged( const Kasten2::DocumentVersionData &versionData );
    /*virtual*/ void headVersionChanged( int newHeadVersionIndex );

  Q_SIGNALS: // If::UserListable
    /*virtual*/ void usersAdded( const QList<Person>& newUserList );
    /*virtual*/ void usersRemoved( const QList<Person>& newUserList );

  private Q_SLOTS:
    void onModelModified( bool newState );
    void onHeadVersionDescriptionChanged( const QString &newDescription );

  private:
    Okteta::PieceTableByteArrayModel *mByteArray;

    mutable QString mTitle;

    const QString mInitDescription;

    QList<Person> mUserList;
};

}

#endif
