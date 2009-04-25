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
// Qt
#include <QtGui/QLayout>
#include <QtGui/QLabel>
#include <QtGui/QSpinBox>


ModSumByteArrayChecksumParameterSetEdit::ModSumByteArrayChecksumParameterSetEdit( QWidget* parent )
  : AbstractByteArrayChecksumParameterSetEdit( parent )
{
#if 0
    QVBoxLayout *baseLayout = new QVBoxLayout( this );
    baseLayout->setMargin( 0 );

    mGroupSizeSpinBox = new QSpinBox( this );
    mGroupSizeSpinBox->setRange( 1, INT_MAX );

    QLabel *label = new QLabel( i18nc("@label:spinbox number of bytes the rotation is done within",
                                      "&Group size [bytes]"),
                                this );
    label->setBuddy( mGroupSizeSpinBox );
    const QString groupSizeWhatsThis =
        i18nc( "@info:whatsthis",
               "Control the number of bytes within which each rotation is made." );
    label->setWhatsThis( groupSizeWhatsThis );
    mGroupSizeSpinBox->setWhatsThis( groupSizeWhatsThis );

    baseLayout->addWidget( label );
    baseLayout->addWidget( mGroupSizeSpinBox );

    mMoveBitWidthSpinBox = new QSpinBox( this );
    mMoveBitWidthSpinBox->setRange( INT_MIN, INT_MAX );
    connect( mMoveBitWidthSpinBox, SIGNAL(valueChanged( int )), SLOT(onValueChanged( int )) );

    label = new QLabel( i18nc("@label:spinbox","S&hift width [bits]"), this );
    label->setBuddy( mMoveBitWidthSpinBox );
    const QString moveBitWidthWhatsThis =
        i18nc( "@info:whatsthis",
               "Control the width of the shift. Positive numbers move the bits to the right, negative to the left." );
    label->setWhatsThis( moveBitWidthWhatsThis );
    mMoveBitWidthSpinBox->setWhatsThis( moveBitWidthWhatsThis );

    baseLayout->addWidget( label );
    baseLayout->addWidget( mMoveBitWidthSpinBox );
    baseLayout->addStretch( 10 );
#endif
}

bool ModSumByteArrayChecksumParameterSetEdit::isValid() const { return true; } //mMoveBitWidthSpinBox->value() != 0; }

void ModSumByteArrayChecksumParameterSetEdit::setParameterSet( const AbstractByteArrayChecksumParameterSet *parameterSet )
{
#if 0
    const RotateByteArrayFilterParameterSet *rotateParameterSet =
        static_cast<const RotateByteArrayFilterParameterSet *>( parameterSet );

    mGroupSizeSpinBox->setValue( rotateParameterSet->groupSize() );
    mMoveBitWidthSpinBox->setValue( rotateParameterSet->moveBitWidth() );
#endif
}

void ModSumByteArrayChecksumParameterSetEdit::getParameterSet( AbstractByteArrayChecksumParameterSet* parameterSet ) const
{
#if 0
    RotateByteArrayFilterParameterSet *rotateParameterSet =
        static_cast<RotateByteArrayFilterParameterSet *>( parameterSet );

    rotateParameterSet->setGroupSize( mGroupSizeSpinBox->value() );
    rotateParameterSet->setMoveBitWidth( mMoveBitWidthSpinBox->value() );
#endif
}

#if 0
void ModSumByteArrayChecksumParameterSetEdit::onValueChanged( int value )
{
    emit validityChanged( value != 0 );
}
#endif

ModSumByteArrayChecksumParameterSetEdit::~ModSumByteArrayChecksumParameterSetEdit()
{}
