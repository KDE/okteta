/*
    This file is part of the Kasten Framework, made within the KDE community.

    Copyright 2010 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "bytearrayihexstreamencoderconfigeditor.h"

// lib
#include "bytearraytextstreamencoderpreview.h"
// KDE
#include <KLocale>
#include <KComboBox>
// Qt
#include <QtGui/QFormLayout>
#include <QtGui/QLabel>


namespace Kasten2
{

ByteArrayIHexStreamEncoderConfigEditor::ByteArrayIHexStreamEncoderConfigEditor( ByteArrayIHexStreamEncoder* encoder, QWidget* parent )
 : AbstractModelStreamEncoderConfigEditor( parent ),
   mEncoder( encoder )
{
    mSettings = mEncoder->settings();

    QFormLayout* pageLayout = new QFormLayout( this );
    pageLayout->setMargin( 0 );

    // data type
    const QString addressSizeLabel =
        i18nc( "@label:listbox the size in bits of the addresses.",
               "Address size:" );

    mAddressSizeSelect = new KComboBox( this );
    // keep order in sync with IHexStreamEncoderSettings::AddressSizeId
    QStringList addressSizeList;
    addressSizeList
        << i18nc("@item:inmenu address size",
                 "32-bit")
        << i18nc("@item:inmenu address size",
                 "16-bit")
        << i18nc("@item:inmenu address size",
                 "8-bit");
    mAddressSizeSelect->addItems( addressSizeList );
    mAddressSizeSelect->setCurrentIndex( mSettings.addressSizeId );
    connect( mAddressSizeSelect, SIGNAL(activated(int)), SLOT(onSettingsChanged()) );
    pageLayout->addRow( addressSizeLabel, mAddressSizeSelect );
}


AbstractSelectionView* ByteArrayIHexStreamEncoderConfigEditor::createPreviewView() const
{
    return new ByteArrayTextStreamEncoderPreview( mEncoder );
}

void ByteArrayIHexStreamEncoderConfigEditor::onSettingsChanged()
{
    mSettings.addressSizeId = (IHexStreamEncoderSettings::AddressSizeId) mAddressSizeSelect->currentIndex();

    mEncoder->setSettings( mSettings );
}

ByteArrayIHexStreamEncoderConfigEditor::~ByteArrayIHexStreamEncoderConfigEditor()
{
}

}
