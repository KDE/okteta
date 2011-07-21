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

#include "operandbytearrayfilterparametersetedit.h"

//
#include "operandbytearrayfilterparameterset.h"
// lib
#include <bytearraycombobox.h>
// KDE
#include <KLocale>
// Qt
#include <QtGui/QCheckBox>
#include <QtGui/QFormLayout>


const char* const OperandByteArrayFilterParameterSetEdit::Id = "Operand";


OperandByteArrayFilterParameterSetEdit::OperandByteArrayFilterParameterSetEdit( QWidget* parent )
 : AbstractByteArrayFilterParameterSetEdit( parent )
{
    QFormLayout *baseLayout = new QFormLayout( this );
    baseLayout->setMargin( 0 );

    const QString operandLabelText =
        i18nc( "@label:textbox operand to the arithmetic filter function",
               "Operand:" );
    mOperandEdit = new Okteta::ByteArrayComboBox( this );
    connect( mOperandEdit, SIGNAL(byteArrayChanged(QByteArray)),
             SLOT(onInputChanged(QByteArray)) );
    const QString operandToolTip =
        i18nc( "@info:tooltip",
               "The operand to do the operation with." );
    const QString operandWhatsThis =
        i18nc( "@info:whatsthis",
               "Enter an operand, or select a previous operand from the list." );
    mOperandEdit->setToolTip( operandToolTip );
    mOperandEdit->setWhatsThis( operandWhatsThis );

    baseLayout->addRow( operandLabelText, mOperandEdit );

    const QString alignAtEndLabelText =
        i18nc( "@option:check",
                "Align at end:" );
    mAlignAtEndCheckBox = new QCheckBox( this );
    mAlignAtEndCheckBox->setChecked( false );
    const QString alignToolTip =
        i18nc( "@info:tooltip",
               "Sets if the operation will be aligned to the end of the data instead of to the begin." );
    const QString alignWhatsThis =
        i18nc( "@info:whatsthis",
               "If set, the operation will be aligned to the end of the data." );
    mAlignAtEndCheckBox->setToolTip( alignToolTip );
    mAlignAtEndCheckBox->setWhatsThis( alignWhatsThis );

    baseLayout->addRow( alignAtEndLabelText, mAlignAtEndCheckBox );
}

bool OperandByteArrayFilterParameterSetEdit::isValid() const { return !mOperandEdit->byteArray().isEmpty(); }

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

    operandParameterSet->setOperand( mOperandEdit->byteArray() );
    operandParameterSet->setOperandFormat( mOperandEdit->format() );
    operandParameterSet->setAlignAtEnd( mAlignAtEndCheckBox->isChecked() );
}

void OperandByteArrayFilterParameterSetEdit::rememberCurrentSettings()
{
    mOperandEdit->rememberCurrentByteArray();
}

void OperandByteArrayFilterParameterSetEdit::onInputChanged( const QByteArray &data )
{
    emit validityChanged( !data.isEmpty() );
}

OperandByteArrayFilterParameterSetEdit::~OperandByteArrayFilterParameterSetEdit()
{}
