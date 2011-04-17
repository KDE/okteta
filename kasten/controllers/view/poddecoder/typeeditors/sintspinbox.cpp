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

#include "sintspinbox.h"

// Qt
#include <QtGui/QLineEdit>


QValidator::State SIntSpinBox::validate( QString& input, int& pos ) const
{
    Q_UNUSED( pos );

    QValidator::State result;

    if( input.isEmpty()
        || (mMinimum < 0 && input == QLatin1String("-")) )
    {
        mValue = 0;
        result = QValidator::Intermediate;
    }
    else
    {
        bool ok;
        qint64 newValue = input.toLongLong( &ok );
        if( !ok
            || (newValue > mMaximum)
            || (newValue < mMinimum) )
            result = QValidator::Invalid;
        else
        {
            mValue = newValue;
            result = QValidator::Acceptable;
        }
    }

    return result;
}


void SIntSpinBox::fixup( QString& input ) const
{
    Q_UNUSED( input );

    // TODO: what can be done here? remove localized stuff?
}

void SIntSpinBox::stepBy( int steps )
{
    if( steps == 0 )
        return;

    if( steps > 0 )
    {
        const qint64 left = mMaximum - mValue;
        mValue = ( static_cast<qint64>(steps) > left ) ? mMaximum : mValue + steps;
    }
    else
    {
        const qint64 left = mValue - mMinimum;
        mValue = ( static_cast<qint64>(-steps) > left ) ? mMinimum : mValue + steps;
    }

    updateEditLine();
}

QAbstractSpinBox::StepEnabled SIntSpinBox::stepEnabled() const
{
    StepEnabled result;

    if( mValue > mMinimum )
        result |= StepDownEnabled;
    if( mValue < mMaximum )
        result |= StepUpEnabled;

    return result;
}


void SIntSpinBox::updateEditLine() const
{
    const QString text = QString::number( mValue, mBase );
    lineEdit()->setText( text );
}
