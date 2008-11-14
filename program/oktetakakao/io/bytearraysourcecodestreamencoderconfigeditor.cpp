/*
    This file is part of the Kakao Framework, part of the KDE project.

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

#include "bytearraysourcecodestreamencoderconfigeditor.h"

// lib
#include "bytearraysourcecodestreamencoderpreview.h"
// KDE
#include <KLocale>
#include <KDialog>
#include <KLineEdit>
#include <KIntNumInput>
#include <KComboBox>
// Qt
#include <QtGui/QLayout>
#include <QtGui/QLabel>


ByteArraySourceCodeStreamEncoderConfigEditor::ByteArraySourceCodeStreamEncoderConfigEditor( KByteArraySourceCodeStreamEncoder* encoder, QWidget* parent )
 : AbstractModelStreamEncoderConfigEditor( parent ),
   mEncoder( encoder )
{
    QGridLayout* pageLayout = new QGridLayout( this ); // unknown rows
    pageLayout->setSpacing( KDialog::spacingHint() );
    pageLayout->setMargin( 0 );
    pageLayout->setColumnStretch( 0, 0 );
    pageLayout->setColumnStretch( 1, 0 );

    // variable name
    QLabel* label = new QLabel( i18nc("","Name of variable:"), this );
    pageLayout->addWidget( label, 0, 0, Qt::AlignRight);

    mVariableNameEdit = new KLineEdit( this );
    connect( mVariableNameEdit, SIGNAL(textChanged( const QString& )), SLOT(onSettingsChanged()) );
    pageLayout->addWidget( mVariableNameEdit, 0, 1);

    // variable name
    label = new QLabel( i18nc("","Items per line:"), this );
    pageLayout->addWidget( label, 1, 0, Qt::AlignRight);

    mItemsPerLineEdit = new KIntNumInput( this );
    mItemsPerLineEdit->setMinimum( 1 );
    connect( mItemsPerLineEdit, SIGNAL(valueChanged( int )), SLOT(onSettingsChanged()) );
    pageLayout->addWidget( mItemsPerLineEdit, 1, 1);

    // variable name
    label = new QLabel( i18nc("","Data type:"), this );
    pageLayout->addWidget( label, 2, 0, Qt::AlignRight);

    mDataTypeSelect = new KComboBox( this );
    const char** dataTypeNames = mEncoder->dataTypeNames();
    const int dataTypesCount = mEncoder->dataTypesCount();
    QStringList dataTypeNameStrings;
    for( int i=0; i<dataTypesCount; ++i )
        dataTypeNameStrings << dataTypeNames[i];
    mDataTypeSelect->addItems( dataTypeNameStrings );
    connect( mDataTypeSelect, SIGNAL(activated(int)), SLOT(onSettingsChanged()) );
    pageLayout->addWidget( mDataTypeSelect, 2, 1);
    pageLayout->setRowStretch( 3, 10 );

    mSettings = mEncoder->settings();

    mVariableNameEdit->setText( mSettings.variableName );
    mItemsPerLineEdit->setValue( mSettings.elementsPerLine );
    mDataTypeSelect->setCurrentIndex( mSettings.dataType );
}

bool ByteArraySourceCodeStreamEncoderConfigEditor::isValid() const
{
    return true; // TODO: warn if not all selected bytes are used due to the data type length
}

AbstractSelectionView* ByteArraySourceCodeStreamEncoderConfigEditor::createPreviewView() const
{
    return new ByteArraySourceCodeStreamEncoderPreview( mEncoder );
}

void ByteArraySourceCodeStreamEncoderConfigEditor::onSettingsChanged()
{
    mSettings.variableName = mVariableNameEdit->text();
    mSettings.elementsPerLine = mItemsPerLineEdit->value();
    mSettings.dataType = mDataTypeSelect->currentIndex();
    mEncoder->setSettings( mSettings );
}

ByteArraySourceCodeStreamEncoderConfigEditor::~ByteArraySourceCodeStreamEncoderConfigEditor()
{
}
