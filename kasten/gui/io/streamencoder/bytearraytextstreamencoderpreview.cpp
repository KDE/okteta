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

#include "bytearraytextstreamencoderpreview.h"

// lib
#include "abstractbytearraystreamencoder.h"
// KDE
#include <KTextEdit>
#include <KGlobalSettings>
#include <KLocale>


namespace Kasten2
{

ByteArrayTextStreamEncoderPreview::ByteArrayTextStreamEncoderPreview( AbstractByteArrayStreamEncoder* encoder )
 : mEncoder( encoder ), mModel( 0 )
{
    mWidget = new KTextEdit(); // TODO: use Kate for syntax highlighting
    mWidget->setReadOnly( true );
    mWidget->setLineWrapMode( QTextEdit::NoWrap );
    mWidget->setToolTip( i18n("The preview uses maximal the first 100 bytes.") );

    connect( KGlobalSettings::self(), SIGNAL(kdisplayFontChanged()),
             SLOT(setFixedFontByGlobalSettings()) );
    setFixedFontByGlobalSettings();

    connect( mEncoder, SIGNAL(settingsChanged()), SLOT(update()) );
}

QWidget* ByteArrayTextStreamEncoderPreview::widget() const { return mWidget; }

void ByteArrayTextStreamEncoderPreview::setData( AbstractModel* model, const AbstractModelSelection* selection )
{
    mModel = model;
    mSelection = selection;

    update();
}


void ByteArrayTextStreamEncoderPreview::update()
{
    if( mModel )
        mWidget->setText( mEncoder->previewData(mModel,mSelection) );
}

void ByteArrayTextStreamEncoderPreview::setFixedFontByGlobalSettings()
{
    mWidget->setFont( KGlobalSettings::fixedFont() );
}

ByteArrayTextStreamEncoderPreview::~ByteArrayTextStreamEncoderPreview()
{
    delete mWidget;
}

}
