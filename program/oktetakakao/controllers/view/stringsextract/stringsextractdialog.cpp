/*
    This file is part of the Okteta Kakao module, part of the KDE project.

    Copyright 2007 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "stringsextractdialog.h"

// controller
#include "stringsextracttool.h"
// lib
#include <kbytearraydocument.h>
// Okteta gui
#include <kbytearrayview.h>
// Okteta core
#include <kcharcodec.h>
#include <kbytearraymodel.h>
// KDE
#include <KLocale>
// Qt
#include <QtGui/QGroupBox>
#include <QtGui/QLayout>
#include <QtGui/QListWidget>


StringsExtractDialog::StringsExtractDialog( StringsExtractTool *tool, QWidget *parent )
 : KDialog( parent ), mTool( tool )
{
    setCaption( i18nc("@title:window","Extract Strings") );
    setModal( false );

    setButtons( User1 | Cancel );
    setButtonText( User1, i18nc("@action:button Update the list of strings extracted","Update") );
    setDefaultButton( User1 );

    QWidget *page = new QWidget( this );
    setMainWidget( page );

    QVBoxLayout *pageLayout = new QVBoxLayout( page );
    pageLayout->setSpacing( spacingHint() );
    pageLayout->setMargin( 0 );

    mListView = new QListWidget( page );
    pageLayout->addWidget( mListView );

    connect( this, SIGNAL(user1Clicked()),   SLOT(onUpdateClicked()) );
    connect( mTool, SIGNAL(stringsChanged()), SLOT(onStringsChanged()) );
}


void StringsExtractDialog::onUpdateClicked()
{
    mTool->extract();
}

void StringsExtractDialog::onStringsChanged()
{
    mListView->clear();

    foreach( const ContainedString &string, mTool->containedStringList() )
        mListView->addItem( string.string() );
}


StringsExtractDialog::~StringsExtractDialog()
{
}

