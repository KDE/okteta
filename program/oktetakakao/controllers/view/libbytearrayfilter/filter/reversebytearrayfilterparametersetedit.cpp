/***************************************************************************
                          reversebytearrayfilterparametersetedit.cpp  -  description
                             -------------------
    begin                : Fri Jan 11 2008
    copyright            : 2008 by Friedrich W. H. Kossebau
    email                : kossebau@kde.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU General Public License           *
 *   version 2 as published by the Free Software Foundation.               *
 *                                                                         *
 ***************************************************************************/


#include "reversebytearrayfilterparametersetedit.h"

//
#include "reversebytearrayfilterparameterset.h"
// KDE
#include <KLocale>
#include <KDialog>
// Qt
#include <QtGui/QCheckBox>
#include <QtGui/QLayout>


ReverseByteArrayFilterParameterSetEdit::ReverseByteArrayFilterParameterSetEdit( QWidget *parent )
 : AbstractByteArrayFilterParameterSetEdit( parent )
{
    QVBoxLayout *baseLayout = new QVBoxLayout( this );
    baseLayout->setSpacing( KDialog::spacingHint() );
    baseLayout->setMargin( 0 );

    mInvertsBitsCheckBox = new QCheckBox( i18n("Reverse also Bits"), this );
    mInvertsBitsCheckBox->setChecked( false );
    const QString alignWhatsThis =
        i18n("If set, also the bits are ordered reverse.");
    mInvertsBitsCheckBox->setWhatsThis( alignWhatsThis );

    baseLayout->addWidget( mInvertsBitsCheckBox );
    baseLayout->addStretch( 10 );
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
