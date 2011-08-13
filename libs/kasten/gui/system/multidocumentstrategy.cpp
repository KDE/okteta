/*
    This file is part of the Kasten Framework, made within the KDE community.

    Copyright 2011 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "multidocumentstrategy.h"

// lib
#include "createdialog.h"
// Kasten gui
#include <abstractmodeldatageneratorconfigeditor.h>
#include <modelcodecviewmanager.h>
#include <viewmanager.h>
// Kasten core
#include <modeldatageneratethread.h>
#include <multidocumentstrategy.h>
#include <documentsyncmanager.h>
#include <documentcreatemanager.h>
#include <documentmanager.h>
#include <abstractmodeldatagenerator.h>
#include <abstractmodel.h>
// Qt
#include <QtGui/QClipboard>
#include <QtGui/QApplication>
#include <QtCore/QMimeData>


namespace Kasten
{

MultiDocumentStrategy::MultiDocumentStrategy( DocumentManager* documentManager,
                                              ViewManager* viewManager )
  : AbstractDocumentStrategy( 0 )
  , mDocumentManager( documentManager )
  , mViewManager( viewManager )
{
    // setup
    connect( mDocumentManager, SIGNAL(added(QList<Kasten::AbstractDocument*>)),
             mViewManager, SLOT(createViewsFor(QList<Kasten::AbstractDocument*>)) );
    connect( mDocumentManager, SIGNAL(closing(QList<Kasten::AbstractDocument*>)),
             mViewManager, SLOT(removeViewsFor(QList<Kasten::AbstractDocument*>)) );
    connect( mDocumentManager->syncManager(), SIGNAL(urlUsed(KUrl)),
             SIGNAL(urlUsed(KUrl)) );
}

QList<AbstractDocument*> MultiDocumentStrategy::documents() const
{
    return mDocumentManager->documents();
}

bool MultiDocumentStrategy::canClose( AbstractDocument* document ) const
{
    return mDocumentManager->canClose( document );
}

bool MultiDocumentStrategy::canCloseAllOther( AbstractDocument* document ) const
{
    return mDocumentManager->canCloseAllOther( document );
}

bool MultiDocumentStrategy::canCloseAll() const
{
    return mDocumentManager->canCloseAll();
}

void MultiDocumentStrategy::createNew()
{
    mDocumentManager->createManager()->createNew();
}

void MultiDocumentStrategy::createNewFromClipboard()
{
    const QMimeData* mimeData = QApplication::clipboard()->mimeData( QClipboard::Clipboard );

    mDocumentManager->createManager()->createNewFromData( mimeData, true );
}

void MultiDocumentStrategy::createNewWithGenerator( AbstractModelDataGenerator* generator )
{
    AbstractModelDataGeneratorConfigEditor* configEditor =
        mViewManager->codecViewManager()->createConfigEditor( generator );

    if( configEditor )
    {
        CreateDialog* dialog = new CreateDialog( configEditor );
//         dialog->setData( mModel, selection ); TODO
        if( ! dialog->exec() )
            return;
    }

    QApplication::setOverrideCursor( Qt::WaitCursor );

    ModelDataGenerateThread* generateThread =
        new ModelDataGenerateThread( this, generator );
    generateThread->start();
    while( !generateThread->wait(100) )
        QApplication::processEvents( QEventLoop::ExcludeUserInputEvents | QEventLoop::ExcludeSocketNotifiers, 100 );

    QMimeData* mimeData = generateThread->data();

    delete generateThread;

    const bool setModified = ( generator->flags() & AbstractModelDataGenerator::DynamicGeneration );
    mDocumentManager->createManager()->createNewFromData( mimeData, setModified );

    QApplication::restoreOverrideCursor();
}

void MultiDocumentStrategy::load( const KUrl& url )
{
    mDocumentManager->syncManager()->load( url );
}

void MultiDocumentStrategy::closeAll()
{
    mDocumentManager->closeAll();
}

void MultiDocumentStrategy::closeAllOther( AbstractDocument* document )
{
    mDocumentManager->closeAllOther( document );
}

void MultiDocumentStrategy::closeDocument( AbstractDocument* document )
{
    mDocumentManager->closeDocument( document );
}


MultiDocumentStrategy::~MultiDocumentStrategy()
{
}

}
