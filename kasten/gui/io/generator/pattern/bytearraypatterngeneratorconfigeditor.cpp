/*
    This file is part of the Kasten Framework, made within the KDE community.

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

#include "bytearraypatterngeneratorconfigeditor.h"

// lib
#include <bytearraycombobox.h>
// KDE
#include <KLocale>
#include <KIntNumInput>
// Qt
#include <QtGui/QFormLayout>


namespace Kasten2
{

ByteArrayPatternGeneratorConfigEditor::ByteArrayPatternGeneratorConfigEditor( ByteArrayPatternGenerator* generator, QWidget* parent )
  : AbstractModelDataGeneratorConfigEditor( parent ),
    mGenerator( generator )
{
    mSettings = mGenerator->settings();

    QFormLayout* pageLayout = new QFormLayout( this );
    pageLayout->setMargin( 0 );

    // pattern
    const QString patternEditLabel =
        i18nc( "@label:textbox",
               "Pattern:" );
    mPatternEdit = new Okteta::ByteArrayComboBox( this );
    mPatternEdit->setByteArray( mSettings.pattern );
    connect( mPatternEdit, SIGNAL(byteArrayChanged(QByteArray)), SLOT(onSettingsChanged()) );
    connect( mPatternEdit, SIGNAL(byteArrayChanged(QByteArray)), SLOT(onPatternChanged(QByteArray)) );
    const QString inputWhatsThis =
        i18nc( "@info:whatsthis",
               "Enter a pattern to search for, or select a previous pattern from the list." );
    mPatternEdit->setWhatsThis( inputWhatsThis );

    pageLayout->addRow( patternEditLabel, mPatternEdit );

    // number
    const QString numberInputLabel =
        i18nc( "@label:spinbox number of times to insert the pattern",
               "&Number:" );
    mNumberInput = new KIntNumInput( this );
    mNumberInput->setRange( 1, INT_MAX );
    mNumberInput->setValue( mSettings.count );
    connect( mNumberInput, SIGNAL(valueChanged(int)), SLOT(onSettingsChanged()) );
    const QString numberWhatsThis =
        i18nc( "@info:whatsthis",
               "Enter the number of times the pattern should be inserted." );
    mNumberInput->setWhatsThis( numberWhatsThis );

    pageLayout->addRow( numberInputLabel, mNumberInput );
}

bool ByteArrayPatternGeneratorConfigEditor::isValid() const { return ( ! mSettings.pattern.isEmpty() ); }

QString ByteArrayPatternGeneratorConfigEditor::name() const
{
    return i18nc("@item name of the generated data","Pattern");
}

// TODO: get char codec
#if 0
void InsertPatternDialog::setCharCodec( const QString& codecName )
{
    mPatternEdit->setCharCodec( codecName );
}
#endif

void ByteArrayPatternGeneratorConfigEditor::onSettingsChanged()
{
    mSettings.pattern = mPatternEdit->byteArray();
    mSettings.count = mNumberInput->value();

    mGenerator->setSettings( mSettings );
}

void ByteArrayPatternGeneratorConfigEditor::onPatternChanged( const QByteArray& pattern )
{
    emit validityChanged( ! pattern.isEmpty() );
}

ByteArrayPatternGeneratorConfigEditor::~ByteArrayPatternGeneratorConfigEditor()
{
}

}
