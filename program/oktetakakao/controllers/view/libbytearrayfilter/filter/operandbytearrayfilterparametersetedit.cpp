/***************************************************************************
                          operandbytearrayfilterparametersetedit.cpp  -  description
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


#include "operandbytearrayfilterparametersetedit.h"

//
#include "operandbytearrayfilterparameterset.h"
#include <kbytearraylineedit.h>
// KDE
#include <KLocale>
#include <KDialog>
// Qt
#include <QtGui/QLabel>
#include <QtGui/QCheckBox>
#include <QtGui/QLayout>


OperandByteArrayFilterParameterSetEdit::OperandByteArrayFilterParameterSetEdit( QWidget *parent )
 : AbstractByteArrayFilterParameterSetEdit( parent )
{
    QVBoxLayout *baseLayout = new QVBoxLayout( this );
    baseLayout->setSpacing( KDialog::spacingHint() );
    baseLayout->setMargin( 0 );

    QLabel *label = new QLabel( i18n("Operand:"), this );
    mOperandEdit = new KByteArrayLineEdit( this );
    connect( mOperandEdit, SIGNAL(dataChanged(const QByteArray&)), SLOT(onInputChanged(const QByteArray&)) );
    label->setBuddy( mOperandEdit );
    const QString operandWhatsThis =
        i18n("Enter an operand, or select a previous operand from the list.");
    label->setWhatsThis( operandWhatsThis );
    mOperandEdit->setWhatsThis( operandWhatsThis );

    baseLayout->addWidget( label );
    baseLayout->addWidget( mOperandEdit );

    mAlignAtEndCheckBox = new QCheckBox( i18n("Align at end"), this );
    mAlignAtEndCheckBox->setChecked( false );
    const QString alignWhatsThis =
        i18n("If set, the last operand will be aligned to the end of the data.");
    mAlignAtEndCheckBox->setWhatsThis( alignWhatsThis );

    baseLayout->addWidget( mAlignAtEndCheckBox );
    baseLayout->addStretch( 10 );
}

bool OperandByteArrayFilterParameterSetEdit::isValid() const { return !mOperandEdit->data().isEmpty(); }

void OperandByteArrayFilterParameterSetEdit::setValues( const AbstractByteArrayFilterParameterSet *parameterSet )
{
    const OperandByteArrayFilterParameterSet *operandParameterSet =
        static_cast<const OperandByteArrayFilterParameterSet *>( parameterSet );

//     mOperandEdit->setValue( operandParameterSet->operand() ); TODO: not yet implemented
    mAlignAtEndCheckBox->setChecked( operandParameterSet->alignAtEnd() );
}

void OperandByteArrayFilterParameterSetEdit::getParameterSet( AbstractByteArrayFilterParameterSet *parameterSet ) const
{
    OperandByteArrayFilterParameterSet *operandParameterSet =
        static_cast<OperandByteArrayFilterParameterSet *>( parameterSet );

    operandParameterSet->setOperand( mOperandEdit->data() );
    operandParameterSet->setOperandFormat( mOperandEdit->format() );
    operandParameterSet->setAlignAtEnd( mAlignAtEndCheckBox->isChecked() );
}

void OperandByteArrayFilterParameterSetEdit::onInputChanged( const QByteArray &data )
{
    emit validityChanged( !data.isEmpty() );
}

OperandByteArrayFilterParameterSetEdit::~OperandByteArrayFilterParameterSetEdit()
{}
