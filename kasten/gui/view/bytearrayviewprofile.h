/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    Copyright 2010,2012 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#ifndef BYTEARRAYVIEWPROFILE_H
#define BYTEARRAYVIEWPROFILE_H

// lib
#include "oktetakastengui_export.h"
// Qt
#include <QtCore/QSharedDataPointer>

class QChar;
class QString;


namespace Kasten2
{
class ByteArrayViewProfilePrivate;

class OKTETAKASTENGUI_EXPORT ByteArrayViewProfile
{
  public:
    typedef QString Id;

  public:
    explicit ByteArrayViewProfile();
    ByteArrayViewProfile( const ByteArrayViewProfile& other );

    ~ByteArrayViewProfile();

  public:
    ByteArrayViewProfile& operator=( const ByteArrayViewProfile& other );

  public:
    void setId( const Id& id );
    Id id() const;

  public: // AbstractModel API
    void setViewProfileTitle( const QString& title );
    QString viewProfileTitle() const;

  public: // codings
    void setValueCoding( int valueCoding );
    void setCharCoding( const QString& charCodingName );
    int valueCoding() const;
    QString charCodingName() const;
    void setShowsNonprinting( bool showsNonprinting );
    bool showsNonprinting() const;
    void setSubstituteChar( const QChar& substituteChar );
    void setUndefinedChar( const QChar& undefinedChar );
    QChar substituteChar() const;
    QChar undefinedChar() const;

  public: // elements
    void setOffsetColumnVisible( bool visible );
    void setVisibleByteArrayCodings( int columns );
    bool offsetColumnVisible() const;
    int visibleByteArrayCodings() const;

  public: // table layout
    void setLayoutStyle( int layoutStyle );
    void setNoOfBytesPerLine( int noOfBytesPerLine );
    void setNoOfGroupedBytes( int noOfGroupedBytes );
    int noOfBytesPerLine() const;
    int layoutStyle() const;
    int noOfGroupedBytes() const;

  public:
    void setViewModus( int viewModus );
    int viewModus() const;

  private:
    QSharedDataPointer<ByteArrayViewProfilePrivate> d;
};

}

#endif
