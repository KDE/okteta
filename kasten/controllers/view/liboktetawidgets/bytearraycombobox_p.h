/*
    This file is part of the Okteta Kasten module, part of the KDE project.

    Copyright 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

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
#include <QtCore/QString>

class KLineEdit;

namespace Okteta
{

class ByteArrayComboBoxPrivate
{
  public:
    explicit ByteArrayComboBoxPrivate( ByteArrayComboBox* parent );

  public:
    QByteArray data() const;
    int format() const;

  public:
    void init();
    void setCharCodec( const QString& charCodecName );

    void onFormatChanged( int index );
    void onDataChanged( const QString &data );

  protected:
    ByteArrayComboBox* const q_ptr;
    Q_DECLARE_PUBLIC( ByteArrayComboBox )

    KComboBox* mFormatComboBox;
    KLineEdit* mDataEdit;

    QString mData[ByteArrayValidator::CodecNumber];
    ByteArrayValidator* mValidator;
};


inline ByteArrayComboBoxPrivate::ByteArrayComboBoxPrivate( ByteArrayComboBox* parent )
  : q_ptr( parent )
{}

inline QByteArray ByteArrayComboBoxPrivate::data() const
{
    return mValidator->toByteArray( mData[mFormatComboBox->currentIndex()] );
}
inline int ByteArrayComboBoxPrivate::format() const
{
    return mFormatComboBox->currentIndex();
}

}

#endif
