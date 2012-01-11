/*
    This file is part of the Okteta Kasten module, made within the KDE community.

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

#include "bytesperlinedialog.h"

// KDE
#include <KIntNumInput>
#include <KLocale>
// Qt
#include <QtGui/QFormLayout>


namespace Kasten2
{

BytesPerLineDialog::BytesPerLineDialog( QWidget* parent )
  : KDialog( parent )
{
    QWidget* page = new QWidget( this );
    setMainWidget( page );

    QFormLayout* pageLayout = new QFormLayout( page );
    pageLayout->setMargin( 0 );

    mBytesPerLineEdit = new KIntNumInput( this );
    mBytesPerLineEdit->setRange( 1, INT_MAX );
    mBytesPerLineEdit->setSuffix( ki18np(" byte"," bytes") );
    const QString bytesPerLineLabel =
        i18nc( "@label:spinbox number of bytes which are shown per line",
                "Per Line:" );
    pageLayout->addRow( bytesPerLineLabel, mBytesPerLineEdit );

    const QString caption =
        i18nc("@title:window","Bytes per Line");
    setCaption( caption );
    setButtons( Ok | Cancel );
}

int BytesPerLineDialog::bytesPerLine()      const { return mBytesPerLineEdit->value(); }

void BytesPerLineDialog::setBytesPerLine( int bytesPerLine )
{
    mBytesPerLineEdit->setValue( bytesPerLine );
}

BytesPerLineDialog::~BytesPerLineDialog() {}

}
