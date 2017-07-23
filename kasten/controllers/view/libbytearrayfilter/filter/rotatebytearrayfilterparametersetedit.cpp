/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    Copyright 2008-2009 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "rotatebytearrayfilterparametersetedit.h"

// parameterset
#include "rotatebytearrayfilterparameterset.h"
// KF5
#include <KLocalizedString>
// Qt
#include <QFormLayout>
#include <QSpinBox>


const char RotateByteArrayFilterParameterSetEdit::Id[] = "Rotate";


RotateByteArrayFilterParameterSetEdit::RotateByteArrayFilterParameterSetEdit( QWidget* parent )
 : AbstractByteArrayFilterParameterSetEdit( parent )
{
    QFormLayout* baseLayout = new QFormLayout( this );
    baseLayout->setMargin( 0 );

    mGroupSizeEdit = new QSpinBox( this );
    mGroupSizeEdit->setRange( 1, INT_MAX );

    const QString groupSizeLabelText =
         i18nc( "@label:spinbox number of bytes the movement is done within",
                "&Group size (bytes):" );
    const QString groupSizeToolTip =
        i18nc( "@info:tooltip",
               "The number of bytes within which each movement is made." );
    mGroupSizeEdit->setToolTip( groupSizeToolTip );
    const QString groupSizeWhatsThis =
        i18nc( "@info:whatsthis",
               "Control the number of bytes within which each movement is made." );
    mGroupSizeEdit->setWhatsThis( groupSizeWhatsThis );

    baseLayout->addRow( groupSizeLabelText, mGroupSizeEdit );

    mMoveBitWidthEdit = new QSpinBox( this );
    mMoveBitWidthEdit->setRange( INT_MIN, INT_MAX );
    connect( mMoveBitWidthEdit, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged),
             this, &RotateByteArrayFilterParameterSetEdit::onValueChanged );

    const QString moveBitWidthLabelText =
        i18nc( "@label:spinbox width (in number of bits) the bits are moved",
               "S&hift width (bits):" );
    const QString moveBitWidthToolTip =
        i18nc( "@info:tooltip",
               "The width of the shift. Positive numbers move the bits to the right, negative to the left." );
    mMoveBitWidthEdit->setToolTip( moveBitWidthToolTip );
    const QString moveBitWidthWhatsThis =
        i18nc( "@info:whatsthis",
               "Control the width of the shift. Positive numbers move the bits to the right, negative to the left." );
    mMoveBitWidthEdit->setWhatsThis( moveBitWidthWhatsThis );

    baseLayout->addRow( moveBitWidthLabelText, mMoveBitWidthEdit );
}

bool RotateByteArrayFilterParameterSetEdit::isValid() const { return mMoveBitWidthEdit->value() != 0; }

void RotateByteArrayFilterParameterSetEdit::setValues( const AbstractByteArrayFilterParameterSet *parameterSet )
{
    const RotateByteArrayFilterParameterSet *rotateParameterSet =
        static_cast<const RotateByteArrayFilterParameterSet *>( parameterSet );

    mGroupSizeEdit->setValue( rotateParameterSet->groupSize() );
    mMoveBitWidthEdit->setValue( rotateParameterSet->moveBitWidth() );
}

void RotateByteArrayFilterParameterSetEdit::getParameterSet( AbstractByteArrayFilterParameterSet *parameterSet ) const
{
    RotateByteArrayFilterParameterSet *rotateParameterSet =
        static_cast<RotateByteArrayFilterParameterSet *>( parameterSet );

    rotateParameterSet->setGroupSize( mGroupSizeEdit->value() );
    rotateParameterSet->setMoveBitWidth( mMoveBitWidthEdit->value() );
}


void RotateByteArrayFilterParameterSetEdit::onValueChanged( int value )
{
    emit validityChanged( value != 0 );
}

RotateByteArrayFilterParameterSetEdit::~RotateByteArrayFilterParameterSetEdit()
{}
