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

#include "modsumbytearraychecksumparametersetedit.h"

// parameterset
#include "modsumbytearraychecksumparameterset.h"
// KDE
#include <KLocale>
#include <KComboBox>
// Qt
#include <QtGui/QFormLayout>


const char* const ModSumByteArrayChecksumParameterSetEdit::Id = "ModSum";


ModSumByteArrayChecksumParameterSetEdit::ModSumByteArrayChecksumParameterSetEdit( QWidget* parent )
  : AbstractByteArrayChecksumParameterSetEdit( parent )
{
    QFormLayout* baseLayout = new QFormLayout( this );
    baseLayout->setMargin( 0 );

    mByteOrderComboBox = new KComboBox( this );
    mByteOrderComboBox->addItem( i18nc("@item:inlistbox","Little-endian") ); // add first for index
    mByteOrderComboBox->addItem( i18nc("@item:inlistbox","Big-endian") );    // add second for index
    connect( mByteOrderComboBox, SIGNAL(activated(int)),
             SIGNAL(valuesChanged()) );

    const QString byteOrderLabelText =
         i18nc( "@label:listbox byte order to use for decoding the bytes into integer values",
                "Byte Order:" );
    const QString groupSizeToolTip =
        i18nc( "@info:tooltip",
               "The byte order to use for decoding the bytes into integer values." );
    mByteOrderComboBox->setToolTip( groupSizeToolTip );

    baseLayout->addRow( byteOrderLabelText, mByteOrderComboBox );
}

bool ModSumByteArrayChecksumParameterSetEdit::isValid() const { return true; }

void ModSumByteArrayChecksumParameterSetEdit::setParameterSet( const AbstractByteArrayChecksumParameterSet* parameterSet )
{
    const ModSumByteArrayChecksumParameterSet* modSumParameterSet =
        static_cast<const ModSumByteArrayChecksumParameterSet *>( parameterSet );

    mByteOrderComboBox->setCurrentIndex( modSumParameterSet->endianness() );
}

void ModSumByteArrayChecksumParameterSetEdit::getParameterSet( AbstractByteArrayChecksumParameterSet* parameterSet ) const
{
    ModSumByteArrayChecksumParameterSet* modSumParameterSet =
        static_cast<ModSumByteArrayChecksumParameterSet *>( parameterSet );

    modSumParameterSet->setEndianness( static_cast<Endianness>( mByteOrderComboBox->currentIndex() ) );
}


ModSumByteArrayChecksumParameterSetEdit::~ModSumByteArrayChecksumParameterSetEdit()
{}
