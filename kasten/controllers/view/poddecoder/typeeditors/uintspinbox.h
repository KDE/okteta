/*
    This file is part of the Okteta Kasten module, made within the KDE community.

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

#ifndef UINTSPINBOX_H
#define UINTSPINBOX_H

// Qt
#include <QtGui/QAbstractSpinBox>
// C++
#include <limits>


class UIntSpinBox : public QAbstractSpinBox
{
  public:
    explicit UIntSpinBox( QWidget* parent = 0, int base = 10 );

    virtual ~UIntSpinBox();

  public:
    quint64 value() const;

    quint64 maximum() const;

  public:
    void setValue( quint64 value );

    void setMaximum( quint64 max );
    void setBase( int base );

  protected: // QAbstractSpinBox API
    virtual QValidator::State validate( QString& input, int& pos ) const;
    virtual void stepBy( int steps );
    virtual void fixup( QString& input ) const;
    virtual StepEnabled stepEnabled () const;

  protected:
    void updateEditLine() const;

  protected:
    mutable quint64 mValue;

    quint64 mMaximum;
    int mBase;

    QString mPrefix;
};


inline UIntSpinBox::UIntSpinBox( QWidget *parent, int base )
  : QAbstractSpinBox( parent ),
    mValue( 0 ),
    mMaximum( std::numeric_limits<quint64>::max() ),
    mBase( 0 )
{
    setBase( base );
}

inline quint64 UIntSpinBox::value()   const { return mValue; }
inline quint64 UIntSpinBox::maximum() const { return mMaximum; }

inline void UIntSpinBox::setMaximum( quint64 maximum )
{
    if( mMaximum == maximum )
        return;

    mMaximum = maximum;

    if( mValue > mMaximum )
    {
        mValue = mMaximum;

        updateEditLine();
    }
}

inline void UIntSpinBox::setValue( quint64 value )
{
    if( value > mMaximum )
        value = mMaximum;

    if( mValue == value )
        return;

    mValue = value;

    updateEditLine();
}

inline void UIntSpinBox::setBase( int base )
{
    base = qBound( 2, base, 36 );

    if( mBase == base )
        return;

    mBase = base;
    mPrefix = QString::fromLatin1(
        (base == 16) ? "0x" :
        (base ==  8) ? "0o" :
        (base ==  2) ? "0b" :
        /* else */     0 );
}

inline UIntSpinBox::~UIntSpinBox() {}

#endif
