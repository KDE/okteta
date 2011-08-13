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

#include "singledocumentstrategy.h"

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
// KDE
#include <KApplication>
#include <KUrl>
#include <KProcess>
// QtGui
#include <QtGui/QClipboard>
#include <QtGui/QApplication>
// QtCore
#include <QtCore/QMimeData>


namespace Kasten
{

SingleDocumentStrategy::SingleDocumentStrategy( DocumentManager* documentManager,
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

QList<AbstractDocument*> SingleDocumentStrategy::documents() const
{
    return mDocumentManager->documents();
}

bool SingleDocumentStrategy::canClose( AbstractDocument* document ) const
{
    return mDocumentManager->canClose( document );
}

bool SingleDocumentStrategy::canCloseAllOther( AbstractDocument* document ) const
{
    return mDocumentManager->canCloseAllOther( document );
}

bool SingleDocumentStrategy::canCloseAll() const
{
    return mDocumentManager->canCloseAll();
}

void SingleDocumentStrategy::createNew()
{
    if( mDocumentManager->isEmpty() )
        mDocumentManager->createManager()->createNew();
    else
    {
        const QString executable = QCoreApplication::applicationFilePath();
        // TODO: get parameters from common place with KCmdLineOptions
        // TODO: forward also interesting parameters passed to this program
        const QStringList parameters = QStringList() << QLatin1String( "-c" );
        KProcess::startDetached( executable, parameters );
    }
}

void SingleDocumentStrategy::createNewFromClipboard()
{
    if( mDocumentManager->isEmpty() )
    {
        const QMimeData* mimeData =
            QApplication::clipboard()->mimeData( QClipboard::Clipboard );

        mDocumentManager->createManager()->createNewFromData( mimeData, true );
    }
    else
    {
        const QString executable = QCoreApplication::applicationFilePath();
        // TODO: get parameters from common place with KCmdLineOptions
        // TODO: forward also interesting parameters passed to this program
        const QStringList parameters = QStringList()
            << QLatin1String( "-c" )
            << QLatin1String( "-g" )
            << QLatin1String( "FromClipboard" );
        KProcess::startDetached( executable, parameters );
    }
}

void SingleDocumentStrategy::createNewWithGenerator( AbstractModelDataGenerator* generator )
{
    // TODO: show dialog in this process, meanwhile start other process, but hidden,
    // on result of dialog pass on the paramaters
    if( ! mDocumentManager->isEmpty() )
    {
        const QString executable = QCoreApplication::applicationFilePath();
        // TODO: get parameters from common place with KCmdLineOptions
        // TODO: forward also interesting parameters passed to this program
        // TODO: add id to AbstractModelDataGenerator, to use instead of className
        const QStringList parameters = QStringList()
            << QLatin1String( "-c" )
            << QLatin1String( "-g" )
            << QLatin1String(generator->metaObject()->className());
        KProcess::startDetached( executable, parameters );
        return;
    }

    AbstractModelDataGeneratorConfigEditor* configEditor =
        mViewManager->codecViewManager()->createConfigEditor( generator );

    if( configEditor )
    {
        // TODO: make dialog abstract for different UIs
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

void SingleDocumentStrategy::load( const KUrl& url )
{
    if( mDocumentManager->isEmpty() )
        mDocumentManager->syncManager()->load( url );
    else
    {
        const QString executable = QCoreApplication::applicationFilePath();
        // TODO: get parameters from common place with KCmdLineOptions
        // TODO: forward also interesting parameters passed to this program
        const QStringList parameters = QStringList() << url.url();
        KProcess::startDetached( executable, parameters );
    }
}

void SingleDocumentStrategy::closeAll()
{
    mDocumentManager->closeAll();
}

void SingleDocumentStrategy::closeAllOther( AbstractDocument* document )
{
    mDocumentManager->closeAllOther( document );
}

void SingleDocumentStrategy::closeDocument( AbstractDocument* document )
{
    mDocumentManager->closeDocument( document );
}

SingleDocumentStrategy::~SingleDocumentStrategy()
{
}

}
