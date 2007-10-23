/***************************************************************************
                          dummyview.cpp  -  description
                             -------------------
    begin                : Fri Aug 31 2007
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


#include "dummyview.h"

// lib
#include "kabstractdocument.h"
// Qt
#include <QtGui/QLabel>

DummyView::DummyView( KAbstractDocument *document )
 : mLabel( new QLabel() ),
   mDocument( document )
{
    mLabel->setText( mDocument->title() );

    connect( document, SIGNAL(titleChanged( const QString & )), SLOT(onTitleChange( const QString & )) );
}


KAbstractDocument *DummyView::document() const { return mDocument; }
QWidget* DummyView::widget()             const { return mLabel; }
QString DummyView::title()               const { return mDocument->title(); }

void DummyView::onTitleChange( const QString &newTitle )
{
    mLabel->setText( newTitle );
}


DummyView::~DummyView()
{
    delete mLabel;
}

#include "dummyview.moc"
