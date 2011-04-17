/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    Copyright 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "reversebytearrayfilterparametersetedit.h"

//
#include "reversebytearrayfilterparameterset.h"
// KDE
#include <KLocale>
// Qt
#include <QtGui/QCheckBox>
#include <QtGui/QFormLayout>


const char* const ReverseByteArrayFilterParameterSetEdit::Id = "Reverse";


ReverseByteArrayFilterParameterSetEdit::ReverseByteArrayFilterParameterSetEdit( QWidget* parent )
 : AbstractByteArrayFilterParameterSetEdit( parent )
{
    QFormLayout* baseLayout = new QFormLayout( this );
    baseLayout->setMargin( 0 );

    const QString invertsBitsLabelText =
        i18nc( "@option:check",
               "Reverse also bits:" );
    mInvertsBitsCheckBox = new QCheckBox( this );
    mInvertsBitsCheckBox->setChecked( false );
    const QString alignWhatsThis =
        i18nc( "@info:whatsthis",
               "If set, the bits are arranged in reverse order as well." );
    mInvertsBitsCheckBox->setWhatsThis( alignWhatsThis );

    baseLayout->addRow( invertsBitsLabelText, mInvertsBitsCheckBox );
}

void ReverseByteArrayFilterParameterSetEdit::setValues( const AbstractByteArrayFilterParameterSet *parameterSet )
{
    const ReverseByteArrayFilterParameterSet *reverseParameterSet =
        static_cast<const ReverseByteArrayFilterParameterSet *>( parameterSet );

    mInvertsBitsCheckBox->setChecked( reverseParameterSet->invertsBits() );
}

void ReverseByteArrayFilterParameterSetEdit::getParameterSet( AbstractByteArrayFilterParameterSet *parameterSet ) const
{
    ReverseByteArrayFilterParameterSet *reverseParameterSet =
        static_cast<ReverseByteArrayFilterParameterSet *>( parameterSet );

    reverseParameterSet->setInvertsBits( mInvertsBitsCheckBox->isChecked() );
}

ReverseByteArrayFilterParameterSetEdit::~ReverseByteArrayFilterParameterSetEdit() {}
