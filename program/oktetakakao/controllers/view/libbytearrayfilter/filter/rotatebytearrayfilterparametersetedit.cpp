/***************************************************************************
                          rotatebytearrayfilterparametersetedit.cpp  -  description
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


#include "rotatebytearrayfilterparametersetedit.h"

// parameterset
#include "rotatebytearrayfilterparameterset.h"
// KDE
#include <KLocale>
#include <KDialog>
// Qt
#include <QtGui/QLayout>
#include <QtGui/QLabel>
#include <QtGui/QSpinBox>


RotateByteArrayFilterParameterSetEdit::RotateByteArrayFilterParameterSetEdit( QWidget *parent )
 : AbstractByteArrayFilterParameterSetEdit( parent )
{
    QVBoxLayout *baseLayout = new QVBoxLayout( this );
    baseLayout->setSpacing( KDialog::spacingHint() );
    baseLayout->setMargin( 0 );

    mGroupSizeSpinBox = new QSpinBox( this );
    mGroupSizeSpinBox->setRange( 1, INT_MAX );

    QLabel *label = new QLabel( i18n("&Group size [bytes]"), this );
    label->setBuddy( mGroupSizeSpinBox );

    baseLayout->addWidget( label );
    baseLayout->addWidget( mGroupSizeSpinBox );

    mMoveBitWidthSpinBox = new QSpinBox( this );
    mMoveBitWidthSpinBox->setRange( INT_MIN, INT_MAX );
    connect( mMoveBitWidthSpinBox, SIGNAL(valueChanged( int )), SLOT(onValueChanged( int )) );

    label = new QLabel( i18n("S&hift size [bits]"), this );
    label->setBuddy( mMoveBitWidthSpinBox );

    baseLayout->addWidget( label );
    baseLayout->addWidget( mMoveBitWidthSpinBox );
    baseLayout->addStretch( 10 );
}

bool RotateByteArrayFilterParameterSetEdit::isValid() const { return mMoveBitWidthSpinBox->value() != 0; }

void RotateByteArrayFilterParameterSetEdit::setValues( const AbstractByteArrayFilterParameterSet *parameterSet )
{
    const RotateByteArrayFilterParameterSet *rotateParameterSet =
        static_cast<const RotateByteArrayFilterParameterSet *>( parameterSet );

    mGroupSizeSpinBox->setValue( rotateParameterSet->groupSize() );
    mMoveBitWidthSpinBox->setValue( rotateParameterSet->moveBitWidth() );
}

void RotateByteArrayFilterParameterSetEdit::getParameterSet( AbstractByteArrayFilterParameterSet *parameterSet ) const
{
    RotateByteArrayFilterParameterSet *rotateParameterSet =
        static_cast<RotateByteArrayFilterParameterSet *>( parameterSet );

    rotateParameterSet->setGroupSize( mGroupSizeSpinBox->value() );
    rotateParameterSet->setMoveBitWidth( mMoveBitWidthSpinBox->value() );
}


void RotateByteArrayFilterParameterSetEdit::onValueChanged( int value )
{
    emit validityChanged( value != 0 );
}

RotateByteArrayFilterParameterSetEdit::~RotateByteArrayFilterParameterSetEdit()
{}
