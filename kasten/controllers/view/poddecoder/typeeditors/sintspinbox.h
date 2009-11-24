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

#ifndef SINTSPINBOX_H
#define SINTSPINBOX_H

// Qt
#include <QtGui/QAbstractSpinBox>
// C++
#include <limits>


class SIntSpinBox : public QAbstractSpinBox
{
  public:
    explicit SIntSpinBox( QWidget* parent = 0, quint8 base = 10 );

    virtual ~SIntSpinBox();

  public:
    qint64 value() const;

  public:
    void setValue( qint64 value );

    void setRange( qint64 minimum, qint64 maximum );

    void setBase(int base);
  protected: // QAbstractSpinBox API
    virtual QValidator::State validate( QString& input, int& pos ) const;
    virtual void stepBy( int steps );
    virtual void fixup( QString& input ) const;
    virtual StepEnabled stepEnabled () const;

  protected:
    void updateEditLine() const;

  protected:
    mutable qint64 mValue;

    qint64 mMinimum;
    qint64 mMaximum;
    quint8 mBase;

    QString mPrefix;
};


inline SIntSpinBox::SIntSpinBox( QWidget *parent, quint8 base)
  : QAbstractSpinBox( parent ),
    mValue( 0 ),
    mMinimum( std::numeric_limits<qint64>::min() ),
    mMaximum( std::numeric_limits<qint64>::max() ),
    mBase(base)
{
}

inline qint64 SIntSpinBox::value() const { return mValue; }

inline void SIntSpinBox::setValue( qint64 value )
{
    if( value > mMaximum )
        value = mMaximum;
    else if( value < mMinimum )
        value = mMinimum;

    if( mValue == value )
        return;

    mValue = value;

    updateEditLine();
}

inline void SIntSpinBox::setRange( qint64 minimum, qint64 maximum )
{
    mMinimum = minimum;
    mMaximum = (maximum>minimum) ? maximum : minimum;

    if( mValue > mMaximum )
    {
        mValue = mMaximum;

        updateEditLine();
    }
    else if( mValue < mMinimum )
    {
        mValue = mMinimum;

        updateEditLine();
    }
}

inline void SIntSpinBox::setBase(int base)
{
    mBase = qBound(2,base,36);
    if (base  == 16)
        mPrefix = QString::fromLatin1( "0x" );
    if (base == 8)
        mPrefix = QString::fromLatin1( "0o" );
    if (base == 2)
        mPrefix = QString::fromLatin1( "0b" );
}

inline SIntSpinBox::~SIntSpinBox() {}

#endif
