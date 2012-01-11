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

#include "bytearraybase32streamencoderconfigeditor.h"

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

ByteArrayBase32StreamEncoderConfigEditor::ByteArrayBase32StreamEncoderConfigEditor( ByteArrayBase32StreamEncoder* encoder, QWidget* parent )
 : AbstractModelStreamEncoderConfigEditor( parent ),
   mEncoder( encoder )
{
    mSettings = mEncoder->settings();

    QFormLayout* pageLayout = new QFormLayout( this );
    pageLayout->setMargin( 0 );

    // data type
    const QString encodingTypeLabel =
        i18nc( "@label:listbox the type of the used encoding: Classic, Base32hex or z-base-32.",
               "Encoding:" );

    mEncodingSelect = new KComboBox( this );
    QStringList list;
    list.append( i18nc("@item:inmenu Doing the base32 using the classical encoding",
                       "Classic") );
    list.append( i18nc("@item:inmenu Doing the base32 using the Base32hex encoding",
                       "Base32hex") );
    list.append( i18nc("@item:inmenu Doing the base32 using the z-base-32 encoding",
                       "z-base-32") );
    mEncodingSelect->addItems( list );
    mEncodingSelect->setCurrentIndex( mSettings.algorithmId );
    connect( mEncodingSelect, SIGNAL(activated(int)), SLOT(onSettingsChanged()) );
    pageLayout->addRow( encodingTypeLabel, mEncodingSelect );
}

AbstractSelectionView* ByteArrayBase32StreamEncoderConfigEditor::createPreviewView() const
{
    return new ByteArrayTextStreamEncoderPreview( mEncoder );
}

void ByteArrayBase32StreamEncoderConfigEditor::onSettingsChanged()
{
    mSettings.algorithmId = (Base32StreamEncoderSettings::AlgorithmId) mEncodingSelect->currentIndex();

    mEncoder->setSettings( mSettings );
}

ByteArrayBase32StreamEncoderConfigEditor::~ByteArrayBase32StreamEncoderConfigEditor()
{
}

}
