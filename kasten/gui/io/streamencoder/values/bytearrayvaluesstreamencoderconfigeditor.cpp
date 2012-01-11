/*
    This file is part of the Kasten Framework, made within the KDE community.

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

#include "bytearrayvaluesstreamencoderconfigeditor.h"

// lib
#include "bytearraytextstreamencoderpreview.h"
// KDE
#include <KLocale>
#include <KLineEdit>
// #include <KComboBox>
// Qt
#include <QtGui/QLayout>
#include <QtGui/QLabel>


namespace Kasten2
{

ByteArrayValuesStreamEncoderConfigEditor::ByteArrayValuesStreamEncoderConfigEditor( ByteArrayValuesStreamEncoder* encoder, QWidget* parent )
 : AbstractModelStreamEncoderConfigEditor( parent ),
   mEncoder( encoder )
{
    mSettings = mEncoder->settings();

    QGridLayout* pageLayout = new QGridLayout( this ); // unknown rows
    pageLayout->setMargin( 0 );
    pageLayout->setColumnStretch( 0, 0 );
    pageLayout->setColumnStretch( 1, 0 );

#if 0
    // data type
    QLabel* label = new QLabel( i18n("Value coding:"), this );
    pageLayout->addWidget( label, 0, 0, Qt::AlignRight);

    mValueCodingSelect = new KComboBox( this );
    QStringList list;
    list.append( i18nc("@item:inmenu encoding of the bytes as values in the hexadecimal format","Hexadecimal") );
    list.append( i18nc("@item:inmenu encoding of the bytes as values in the decimal format",    "Decimal")     );
    list.append( i18nc("@item:inmenu encoding of the bytes as values in the octal format",      "Octal")       );
    list.append( i18nc("@item:inmenu encoding of the bytes as values in the binary format",     "Binary")      );
    mValueCodingSelect->addItems( list );
    mValueCodingSelect->setCurrentIndex( mSettings.valueCoding );
    connect( mValueCodingSelect, SIGNAL(activated(int)), SLOT(onSettingsChanged()) );
    pageLayout->addWidget( mValueCodingSelect, 0, 1);
#endif
    // separation string
    QLabel* label = new QLabel( i18nc("@label:textbox substring which separates the values","Separation:"), this );
    pageLayout->addWidget( label, 0, 0, Qt::AlignRight);

    mSeparationEdit = new KLineEdit( this );
    mSeparationEdit->setText( mSettings.separation );
    connect( mSeparationEdit, SIGNAL(textChanged(QString)), SLOT(onSettingsChanged()) );
    pageLayout->addWidget( mSeparationEdit, 0, 1);


    // finish
    pageLayout->setRowStretch( 2, 10 );
}


AbstractSelectionView* ByteArrayValuesStreamEncoderConfigEditor::createPreviewView() const
{
    return new ByteArrayTextStreamEncoderPreview( mEncoder );
}

void ByteArrayValuesStreamEncoderConfigEditor::onSettingsChanged()
{
//     mSettings.valueCoding = (Okteta::ValueCoding) mValueCodingSelect->currentIndex();
    mSettings.separation = mSeparationEdit->text();

    mEncoder->setSettings( mSettings );
}

ByteArrayValuesStreamEncoderConfigEditor::~ByteArrayValuesStreamEncoderConfigEditor()
{
}

}
