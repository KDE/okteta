/***************************************************************************
                          kbytearraydisplay.cpp  -  description
                             -------------------
    begin                : Fri Jun 2 2006
    copyright            : 2006 by Friedrich W. H. Kossebau
    email                : kossebau@kde.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU General Public License           *
 *   version 2 as published by the Free Software Foundation.               *
 *                                                                         *
 ***************************************************************************/


#include "kbytearraydisplay.h"

// program
#include "kbytearraydocument.h"
// lib
#include <kbytearrayview.h>
#include <kbytearraymodel.h>


KByteArrayDisplay::KByteArrayDisplay( KByteArrayDocument *document )
 : mWidget( 0 ), mDocument( document )
{
    mWidget = new KHEUI::KByteArrayView( mDocument->content() );
    // propagate signals
    connect( mDocument, SIGNAL(titleChanged( QString )), SIGNAL(titleChanged( QString )) );
    connect( mDocument, SIGNAL(modified( KAbstractDocument::SynchronizationStates )),
                        SIGNAL(modified( KAbstractDocument::SynchronizationStates )) );
    connect( mWidget, SIGNAL(selectionChanged( bool )), SIGNAL(hasSelectedDataChanged( bool )) );
}


KAbstractDocument *KByteArrayDisplay::document() const { return mDocument; }
QWidget* KByteArrayDisplay::widget()             const { return mWidget; }
QString KByteArrayDisplay::title()               const { return mDocument->title(); }

void KByteArrayDisplay::setZoomLevel( double Level )
{
    mWidget->setZoomLevel( Level );
    emit zoomLevelChanged( mWidget->zoomLevel() );
}

double KByteArrayDisplay::zoomLevel() const
{
    return mWidget->zoomLevel();
}

void KByteArrayDisplay::selectAllData( bool selectAll )
{
    mWidget->selectAll( selectAll );
}

bool KByteArrayDisplay::hasSelectedData() const
{
    return mWidget->hasSelectedData();
}

QMimeData *KByteArrayDisplay::copySelectedData() const
{
    return mWidget->dragObject();
}

void KByteArrayDisplay::insertData( const QMimeData *data )
{
    mWidget->pasteFromSource( data );
}

QMimeData *KByteArrayDisplay::cutSelectedData()
{
    QMimeData *result = mWidget->dragObject();
    mWidget->removeSelectedData();
    return result;
}

void KByteArrayDisplay::deleteSelectedData()
{
    mWidget->removeSelectedData();
}

KByteArrayDisplay::~KByteArrayDisplay()
{
    delete mWidget;
}

#include "kbytearraydisplay.moc"
