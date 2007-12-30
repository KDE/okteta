/***************************************************************************
                          stringsextractdialog.cpp  -  description
                             -------------------
    begin                : Wed Dec 26 2007
    copyright            : 2007 by Friedrich W. H. Kossebau
    email                : kossebau@kde.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU General Public License           *
 *   version 2 as published by the Free Software Foundation.               *
 *                                                                         *
 ***************************************************************************/


#include "stringsextractdialog.h"

// controller
#include "stringsextracttool.h"
//
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
    setCaption( i18n("Extract Strings") );
    setModal( false );

    setButtons( User1 | Cancel );
    setButtonText( User1, i18n("Update") );
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

    foreach( ContainedString string, mTool->containedStringList() )
        mListView->addItem( string.string() );
}


StringsExtractDialog::~StringsExtractDialog()
{
}

