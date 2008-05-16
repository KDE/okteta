/*
    This file is part of the Okteta Kakao module, part of the KDE project.

    Copyright 2006-2007 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "kbytearraydisplay.h"

// lib
#include <kbytearraydocument.h>
// Okteta gui
#include <kbytearrayview.h>
// Okteta core
#include <kabstractbytearraymodel.h>


KByteArrayDisplay::KByteArrayDisplay( KByteArrayDocument *document )
 : mWidget( 0 ), mDocument( document )
{
    KHECore::KAbstractByteArrayModel *content = mDocument->content();
    mWidget = new KHEUI::KByteArrayView( content );

    const bool useOverwriteAsDefault = ( content->size() > 0 );

    mWidget->setOverwriteMode( useOverwriteAsDefault );

    // propagate signals
    connect( mDocument, SIGNAL(titleChanged( QString )), SIGNAL(titleChanged( QString )) );
    connect( mDocument, SIGNAL(modified( KAbstractDocument::SynchronizationStates )),
                        SIGNAL(modified( KAbstractDocument::SynchronizationStates )) );
//     connect( mWidget, SIGNAL(selectionChanged( bool )), SIGNAL(hasSelectedDataChanged( bool )) );
    connect( mWidget, SIGNAL(readOnlyChanged( bool )), SIGNAL(readOnlyChanged( bool )) );
    connect( mWidget, SIGNAL(selectionChanged( bool )), SLOT(onSelectionChange( bool )) );
}

const AbstractModelSelection *KByteArrayDisplay::selection() const { return &mSelection; }

KAbstractDocument *KByteArrayDisplay::document() const { return mDocument; }
QWidget* KByteArrayDisplay::widget()             const { return mWidget; }
QString KByteArrayDisplay::title()               const { return mDocument->title(); }
bool KByteArrayDisplay::isModifiable()           const { return true; }
bool KByteArrayDisplay::isReadOnly()             const { return mWidget->isReadOnly(); }
void KByteArrayDisplay::setReadOnly( bool isReadOnly ) { mWidget->setReadOnly( isReadOnly ); }

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
    return mWidget->selectionAsMimeData();
}

void KByteArrayDisplay::insertData( const QMimeData *data )
{
    mWidget->pasteData( data );
}

QMimeData *KByteArrayDisplay::cutSelectedData()
{
    QMimeData *result = mWidget->selectionAsMimeData();
    mWidget->removeSelectedData();
    return result;
}

void KByteArrayDisplay::deleteSelectedData()
{
    mWidget->removeSelectedData();
}

bool KByteArrayDisplay::canReadData( const QMimeData *data ) const
{
    return mWidget->canReadData( data );
}

void KByteArrayDisplay::onSelectionChange( bool selected )
{
    mSelection.setSection( mWidget->selection() );
    emit hasSelectedDataChanged( selected );
}

KByteArrayDisplay::~KByteArrayDisplay()
{
    delete mWidget;
}
