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

#include "bytearrayrandomdatageneratorconfigeditor.h"

// KDE
#include <KLocale>
#include <KIntNumInput>
// Qt
#include <QtGui/QFormLayout>


namespace Kasten2
{

ByteArrayRandomDataGeneratorConfigEditor::ByteArrayRandomDataGeneratorConfigEditor( ByteArrayRandomDataGenerator* generator, QWidget* parent )
  : AbstractModelDataGeneratorConfigEditor( parent ),
    mGenerator( generator )
{
    mSettings = mGenerator->settings();

    QFormLayout* pageLayout = new QFormLayout( this );
    pageLayout->setMargin( 0 );

    // number
    const QString numberInputLabel =
        i18nc( "@label:spinbox size of the bytes to generate",
               "&Size:" );
    mSizeInput = new KIntNumInput( this );
    mSizeInput->setRange( 1, INT_MAX );
    mSizeInput->setValue( mSettings.size );
    mSizeInput->setSuffix( ki18np(" byte"," bytes") );
    connect( mSizeInput, SIGNAL(valueChanged(int)), SLOT(onSettingsChanged()) );
    const QString numberWhatsThis =
        i18nc( "@info:whatsthis",
               "Enter the size of the byte to generate." );
    mSizeInput->setWhatsThis( numberWhatsThis );

    pageLayout->addRow( numberInputLabel, mSizeInput );
}

QString ByteArrayRandomDataGeneratorConfigEditor::name() const
{
    return i18nc("@item name of the generated data","Random Data");
}

// TODO: get char codec
#if 0
void InsertRandomDataDialog::setCharCodec( const QString& codecName )
{
    mRandomDataEdit->setCharCodec( codecName );
}
#endif

void ByteArrayRandomDataGeneratorConfigEditor::onSettingsChanged()
{
    mSettings.size = mSizeInput->value();

    mGenerator->setSettings( mSettings );
}

ByteArrayRandomDataGeneratorConfigEditor::~ByteArrayRandomDataGeneratorConfigEditor()
{
}

}
