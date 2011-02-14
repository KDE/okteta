/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    Copyright 2009,2011 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#ifndef BYTEARRAYCOMBOBOX_P_H
#define BYTEARRAYCOMBOBOX_P_H

// lib
#include "bytearraycombobox.h"
#include "bytearrayvalidator.h"
// KDE
#include <KComboBox>
// Qt
#include <QtCore/QByteArray>


namespace Okteta
{

class ByteArrayComboBoxPrivate
{
  public:
    explicit ByteArrayComboBoxPrivate( ByteArrayComboBox* parent );

  public:
    QByteArray byteArray() const;
    int format() const;
    int maxLength() const;
    int minLength() const;

  public:
    void init();
    void setByteArray( const QByteArray& byteArray );
    void setCharCodec( const QString& charCodecName );
    void setMaxLength( int maxLength );
    void setMinLength( int minLength );
    void rememberCurrentByteArray();

    void onFormatChanged( int index );
    void onValueEdited( const QString& value );
    void onValueActivated( int index );

  protected:
    ByteArrayComboBox* const q_ptr;
    Q_DECLARE_PUBLIC( ByteArrayComboBox )

    KComboBox* mFormatComboBox;
    KComboBox* mValueComboBox;

    ByteArrayValidator* mValidator;
};


inline ByteArrayComboBoxPrivate::ByteArrayComboBoxPrivate( ByteArrayComboBox* parent )
  : q_ptr( parent )
{}

inline QByteArray ByteArrayComboBoxPrivate::byteArray() const
{
    return mValidator->toByteArray( mValueComboBox->currentText() );
}
inline int ByteArrayComboBoxPrivate::format() const
{
    return mFormatComboBox->currentIndex();
}

}

#endif
