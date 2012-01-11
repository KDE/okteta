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

#include "bytespergroupdialog.h"

// KDE
#include <KIntNumInput>
#include <KLocale>
// Qt
#include <QtGui/QFormLayout>


namespace Kasten2
{

BytesPerGroupDialog::BytesPerGroupDialog( QWidget* parent )
  : KDialog( parent )
{
    QWidget* page = new QWidget( this );
    setMainWidget( page );

    QFormLayout* pageLayout = new QFormLayout( page );
    pageLayout->setMargin( 0 );

    mGroupedBytesCountEdit = new KIntNumInput( this );
    mGroupedBytesCountEdit->setRange( 0, INT_MAX );
    mGroupedBytesCountEdit->setSuffix( ki18np(" byte"," bytes") );
    const QString noGroupingText = i18nc( "@label",
                                          "No grouping." );
    mGroupedBytesCountEdit->setSpecialValueText( noGroupingText );
    const QString groupedBytesCountLabel =
        i18nc( "@label:spinbox number of bytes which are grouped",
               "Per Group:" );
    pageLayout->addRow( groupedBytesCountLabel, mGroupedBytesCountEdit );

    const QString caption =
        i18nc("@title:window","Bytes per Group");
    setCaption( caption );
    setButtons( Ok | Cancel );
}

int BytesPerGroupDialog::groupedBytesCount() const { return mGroupedBytesCountEdit->value(); }

void BytesPerGroupDialog::setGroupedBytesCount( int groupedBytesCount )
{
    mGroupedBytesCountEdit->setValue( groupedBytesCount );
}

BytesPerGroupDialog::~BytesPerGroupDialog() {}

}
