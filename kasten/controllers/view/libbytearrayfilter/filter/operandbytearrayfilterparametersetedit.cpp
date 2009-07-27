/*
    This file is part of the Okteta Kasten module, part of the KDE project.

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

#include "operandbytearrayfilterparametersetedit.h"

//
#include "operandbytearrayfilterparameterset.h"
// lib
#include <kbytearraylineedit.h>
// KDE
#include <KLocale>
// Qt
#include <QtGui/QLabel>
#include <QtGui/QCheckBox>
#include <QtGui/QLayout>


OperandByteArrayFilterParameterSetEdit::OperandByteArrayFilterParameterSetEdit( QWidget* parent )
 : AbstractByteArrayFilterParameterSetEdit( parent )
{
    QVBoxLayout *baseLayout = new QVBoxLayout( this );
    baseLayout->setMargin( 0 );

    QLabel *label = new QLabel( i18nc("@label:textbox operand to the arithmetic filter function","Operand:"), this );
    mOperandEdit = new KByteArrayLineEdit( this );
    connect( mOperandEdit, SIGNAL(dataChanged(const QByteArray&)), SLOT(onInputChanged(const QByteArray&)) );
    label->setBuddy( mOperandEdit );
    const QString operandWhatsThis =
        i18nc( "@info:whatsthis",
               "Enter an operand, or select a previous operand from the list." );
    label->setWhatsThis( operandWhatsThis );
    mOperandEdit->setWhatsThis( operandWhatsThis );

    baseLayout->addWidget( label );
    baseLayout->addWidget( mOperandEdit );

    mAlignAtEndCheckBox = new QCheckBox( i18nc("@option:check","Align at end"), this );
    mAlignAtEndCheckBox->setChecked( false );
    const QString alignWhatsThis =
        i18nc( "@info:whatsthis",
               "If set, the last operand will be aligned to the end of the data." );
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

void OperandByteArrayFilterParameterSetEdit::setCharCodec( const QString &charCodecName )
{
    mOperandEdit->setCharCodec( charCodecName );
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
