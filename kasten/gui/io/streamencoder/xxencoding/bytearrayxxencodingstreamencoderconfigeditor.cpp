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

#include "bytearrayxxencodingstreamencoderconfigeditor.h"

// lib
#include "bytearraytextstreamencoderpreview.h"
// KDE
#include <KLocale>
#include <KLineEdit>
// Qt
#include <QtGui/QFormLayout>
#include <QtGui/QLabel>


namespace Kasten2
{

ByteArrayXxencodingStreamEncoderConfigEditor::ByteArrayXxencodingStreamEncoderConfigEditor( ByteArrayXxencodingStreamEncoder* encoder, QWidget* parent )
 : AbstractModelStreamEncoderConfigEditor( parent ),
   mEncoder( encoder )
{
    mSettings = mEncoder->settings();

    QFormLayout* pageLayout = new QFormLayout( this );
    pageLayout->setMargin( 0 );

    // internal file name
    const QString fileNameLabel =
        i18nc( "@label:textbox file name internally given to the encoded data",
               "Internal name of file:" );

    mFileNameEdit = new KLineEdit( this );
    mFileNameEdit->setText( mSettings.fileName );
    connect( mFileNameEdit, SIGNAL(textChanged(QString)), SLOT(onSettingsChanged()) );
    pageLayout->addRow( fileNameLabel, mFileNameEdit );
}

AbstractSelectionView* ByteArrayXxencodingStreamEncoderConfigEditor::createPreviewView() const
{
    return new ByteArrayTextStreamEncoderPreview( mEncoder );
}

void ByteArrayXxencodingStreamEncoderConfigEditor::onSettingsChanged()
{
    mSettings.fileName = mFileNameEdit->text();

    mEncoder->setSettings( mSettings );
}

ByteArrayXxencodingStreamEncoderConfigEditor::~ByteArrayXxencodingStreamEncoderConfigEditor()
{
}

}
