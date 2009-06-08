/*
    This file is part of the Okteta Kakao module, part of the KDE project.

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

#include "modsumbytearraychecksumparametersetedit.h"

// parameterset
#include "modsumbytearraychecksumparameterset.h"
// KDE
#include <KLocale>
#include <KComboBox>
// Qt
#include <QtGui/QLayout>


ModSumByteArrayChecksumParameterSetEdit::ModSumByteArrayChecksumParameterSetEdit( QWidget* parent )
  : AbstractByteArrayChecksumParameterSetEdit( parent )
{
    QVBoxLayout* baseLayout = new QVBoxLayout( this );
    baseLayout->setMargin( 0 );

    // TODO KDE4.4: add label
    mEndiannessComboBox = new KComboBox( this );
    mEndiannessComboBox->addItem( i18nc("@item:inlistbox","Little-endian") ); // add first for index
    mEndiannessComboBox->addItem( i18nc("@item:inlistbox","Big-endian") );    // add second for index
    connect( mEndiannessComboBox, SIGNAL(activated( int )),
             SIGNAL(valuesChanged()) );

    baseLayout->addWidget( mEndiannessComboBox );
    baseLayout->addStretch( 10 );
}

bool ModSumByteArrayChecksumParameterSetEdit::isValid() const { return true; }

void ModSumByteArrayChecksumParameterSetEdit::setParameterSet( const AbstractByteArrayChecksumParameterSet* parameterSet )
{
    const ModSumByteArrayChecksumParameterSet* modSumParameterSet =
        static_cast<const ModSumByteArrayChecksumParameterSet *>( parameterSet );

    mEndiannessComboBox->setCurrentIndex( modSumParameterSet->endianness() );
}

void ModSumByteArrayChecksumParameterSetEdit::getParameterSet( AbstractByteArrayChecksumParameterSet* parameterSet ) const
{
    ModSumByteArrayChecksumParameterSet* modSumParameterSet =
        static_cast<ModSumByteArrayChecksumParameterSet *>( parameterSet );

    modSumParameterSet->setEndianness( static_cast<Endianness>( mEndiannessComboBox->currentIndex() ) );
}


ModSumByteArrayChecksumParameterSetEdit::~ModSumByteArrayChecksumParameterSetEdit()
{}
