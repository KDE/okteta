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

#ifndef ADDRESSVALIDATOR_H
#define ADDRESSVALIDATOR_H

// Okteta core
#include <oktetacore.h>
#include <address.h>
// Qt
#include <QtGui/QValidator>


namespace Okteta
{

class ValueCodec;


class AddressValidator : public QValidator
{
  Q_OBJECT

  public:
    // matching Okteta::ValueCoding
    enum Coding { InvalidCoding = -1, HexadecimalCoding=0, DecimalCoding=1 };

  public:
    explicit AddressValidator( QObject* parent, Coding codecId = HexadecimalCoding );
    virtual ~AddressValidator();

  public: // QValidator API
    virtual QValidator::State validate( QString& input, int& pos ) const;

  public:
    /// sets one of the value codecs or any char codec
    void setCodec( Coding codecId );

  public:
    Address toAddress( const QString& string ) const;
    QString toString( Address address ) const;

  private:
    Coding mCodecId;
    ValueCodec* mValueCodec;
};

}

#endif
