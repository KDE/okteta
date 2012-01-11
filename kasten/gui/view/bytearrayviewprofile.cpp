/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    Copyright 2010 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "bytearrayviewprofile.h"

// Qt
#include <QtCore/QString>


namespace Kasten2
{

class ByteArrayViewProfilePrivate : public QSharedData
{
public:
    QString mViewProfileTitle;
    QString mCharCodingName;
    int mValueCoding;
    bool mOffsetColumnVisible;
    int mVisibleByteArrayCodings;
    int mNoOfBytesPerLine;
    int mLayoutStyle;
    QChar mSubstituteChar;
    QChar mUndefinedChar;
    bool mShowsNonprinting;
    int mNoOfGroupedBytes;
    int mViewModus;
};

ByteArrayViewProfile::ByteArrayViewProfile()
  : d( new ByteArrayViewProfilePrivate )
{
}
ByteArrayViewProfile::ByteArrayViewProfile( const ByteArrayViewProfile& other )
  : d( other.d )
{
}

ByteArrayViewProfile& ByteArrayViewProfile::operator=( const ByteArrayViewProfile& other )
{
    d = other.d;

    return *this;
}

QString ByteArrayViewProfile::viewProfileTitle()    const { return d->mViewProfileTitle; }
QString ByteArrayViewProfile::charCodingName()      const { return d->mCharCodingName; }
int ByteArrayViewProfile::valueCoding()             const { return d->mValueCoding; }
bool ByteArrayViewProfile::offsetColumnVisible()    const { return d->mOffsetColumnVisible; }
int ByteArrayViewProfile::visibleByteArrayCodings() const { return d->mVisibleByteArrayCodings; }
int ByteArrayViewProfile::noOfBytesPerLine()        const { return d->mNoOfBytesPerLine; }
int ByteArrayViewProfile::layoutStyle()             const { return d->mLayoutStyle; }
QChar ByteArrayViewProfile::substituteChar()        const { return d->mSubstituteChar; }
QChar ByteArrayViewProfile::undefinedChar()         const { return d->mUndefinedChar; }
bool ByteArrayViewProfile::showsNonprinting()       const { return d->mShowsNonprinting; }
int ByteArrayViewProfile::noOfGroupedBytes()        const { return d->mNoOfGroupedBytes; }
int ByteArrayViewProfile::viewModus()               const { return d->mViewModus; }

void ByteArrayViewProfile::setViewProfileTitle( const QString& title ) { d->mViewProfileTitle = title; }
void ByteArrayViewProfile::setValueCoding( int valueCoding ) { d->mValueCoding = valueCoding; }
void ByteArrayViewProfile::setCharCoding( const QString& charCodingName ) { d->mCharCodingName = charCodingName; }
void ByteArrayViewProfile::setSubstituteChar( const QChar& substituteChar ) { d->mSubstituteChar = substituteChar; }
void ByteArrayViewProfile::setUndefinedChar( const QChar& undefinedChar ) { d->mUndefinedChar = undefinedChar; }
void ByteArrayViewProfile::toggleOffsetColumn( bool visible ) { d->mOffsetColumnVisible = visible; }
void ByteArrayViewProfile::setVisibleByteArrayCodings( int columns ) { d->mVisibleByteArrayCodings = columns; }
void ByteArrayViewProfile::setLayoutStyle( int layoutStyle ) { d->mLayoutStyle = layoutStyle; }
void ByteArrayViewProfile::setNoOfBytesPerLine( int noOfBytesPerLine ) { d->mNoOfBytesPerLine = noOfBytesPerLine; }
void ByteArrayViewProfile::setShowsNonprinting( bool showsNonprinting ) { d->mShowsNonprinting = showsNonprinting; }
void ByteArrayViewProfile::setNoOfGroupedBytes( int noOfGroupedBytes ) { d->mNoOfGroupedBytes = noOfGroupedBytes; }
void ByteArrayViewProfile::setViewModus( int viewModus ) { d->mViewModus = viewModus; }


ByteArrayViewProfile::~ByteArrayViewProfile() {}

}
