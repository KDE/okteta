/*
    This file is part of the Okteta Kakao module, part of the KDE project.

    Copyright 2007-2008 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "printtool.h"

// controller
#include "printjob.h"
#include "framestopaperprinter.h"
#include "headerfooterframerenderer.h"
#include "bytearrayframerenderer.h"
// lib
#include <kbytearraydisplay.h>
#include <kbytearraydocument.h>
// Kakao gui
#include <kabstractview.h>
// Okteta gui
#include <kbytearrayview.h>
// Okteta core
#include <kbytearraymodel.h>
// KDE
#include <KLocale>
#include <KMessageBox>
#include <kdeprintdialog.h>
#include <kdeversion.h>
// Qt
#include <QtGui/QApplication>
#include <QtGui/QPrintDialog>
#include <QtGui/QPrinter>
#include <QtGui/QFont>
#include <QtGui/QFontMetrics>


PrintTool::PrintTool()
 : mDocument( 0 ), mByteArrayView( 0 ), mByteArrayModel( 0 )
{
}

void PrintTool::setTargetModel( AbstractModel* model )
{
//     if( mByteArrayView ) mByteArrayView->disconnect( this );

    KByteArrayDisplay* view = model ? model->findBaseModel<KByteArrayDisplay*>() : 0;
    mByteArrayView = view ? static_cast<KHEUI::KByteArrayView *>( view->widget() ) : 0;

    mDocument = view ? qobject_cast<KByteArrayDocument*>( view->baseModel() ) : 0;
    mByteArrayModel = mDocument ? mDocument->content() : 0;

    const bool hasView = ( mByteArrayModel != 0 );
    emit viewChanged( hasView );
}

void PrintTool::print()
{
    const QString processTitle = i18nc( "@title:window", "Print Byte Array %1", mDocument->title() );

    QPrinter printer;

    // Not supported by Qt?
    //printer.setPageSelection( QPrinter::SystemSide );

//     LayoutDialogPage* layoutPage = new LayoutDialogPage();
    QList<QWidget*> customDialogPages;
//     customDialogPages << layoutPage;
    QPrintDialog *printDialog = KdePrint::createPrintDialog( &printer, customDialogPages, 0 );

    printDialog->setWindowTitle( processTitle );
    if( printDialog->exec() )
    {
        QString creator = QString::fromLatin1( "Print Plugin for Okteta " ); // no i18n(), keep space at end as separator
        creator += KDE_VERSION_STRING; // TODO: change to OKTETA_VERSION_STRING
        printer.setCreator( creator );

        FramesToPaperPrinter framesPrinter;
        framesPrinter.setPaperRect( printer.paperRect() );
        framesPrinter.setPageRect( printer.pageRect() );
        printer.setFullPage( true );


        PrintInfo info;
        const QRect pageRect = framesPrinter.pageRect();
        const int left = pageRect.left();
        const int width = pageRect.width();

        HeaderFooterFrameRenderer *headerFrameRenderer = new HeaderFooterFrameRenderer( &info );
        headerFrameRenderer->setTexts( QString("%d"),
                                       QString("%f"),
                                       i18nc("in the header of the printed page, e.g. Page 2 of 20","Page %p of %P") );
        headerFrameRenderer->setWidth( width );
        headerFrameRenderer->setPos( pageRect.topLeft() );

        HeaderFooterFrameRenderer *footerFrameRenderer = new HeaderFooterFrameRenderer( &info );
        footerFrameRenderer->setTexts( i18nc("in the footer of the printed page, e.g. Printed by: Joe User",
                                             "Printed by: %U"),
                                       i18nc("advertizer in the footer of the printed page","Okteta, built on KDE4"),
                                       QString("%F") );
        footerFrameRenderer->setWidth( width );
        const int footerTop = pageRect.bottom() - footerFrameRenderer->height();
        footerFrameRenderer->setPos( left, footerTop );

        const int contentHeight = pageRect.height() - footerFrameRenderer->height() - headerFrameRenderer->height();
        const int contentTop = pageRect.top() + headerFrameRenderer->height();
        ByteArrayFrameRenderer *byteArrayFrameRenderer = new ByteArrayFrameRenderer;
        byteArrayFrameRenderer->setPos( left, contentTop );
        byteArrayFrameRenderer->setWidth( width );
        byteArrayFrameRenderer->setHeight( contentHeight );

        KHE::KSection section = mByteArrayView->selection();
        if( !section.isValid() )
            section.setByWidth( 0, mByteArrayModel->size() );
        byteArrayFrameRenderer->setByteArrayModel( mByteArrayModel, section.start(), section.width() );

        // TODO: use noOfBytesPerLine of view, scale resolution down if it does not fit the page
        const int noOfBytesPerLine = mByteArrayView->noOfBytesPerLine();
//         byteArrayFrameRenderer->setNoOfBytesPerLine( mByteArrayView->noOfBytesPerLine() );

        const int startOffset = mByteArrayView->startOffset() + section.start();
        const int line = startOffset / noOfBytesPerLine;
        const int firstLineOffset = mByteArrayView->firstLineOffset() + line*noOfBytesPerLine;
        byteArrayFrameRenderer->setFirstLineOffset( firstLineOffset );
        byteArrayFrameRenderer->setStartOffset( startOffset );

        byteArrayFrameRenderer->setEncoding( mByteArrayView->encodingName() );
        byteArrayFrameRenderer->setBufferSpacing( mByteArrayView->byteSpacingWidth(),
                                                  mByteArrayView->noOfGroupedBytes(),
                                                  mByteArrayView->groupSpacingWidth() );
        byteArrayFrameRenderer->setBinaryGapWidth( mByteArrayView->binaryGapWidth() );

        byteArrayFrameRenderer->setCoding( (KHECore::KCoding)mByteArrayView->coding() );
        byteArrayFrameRenderer->setShowsNonprinting( mByteArrayView->showsNonprinting() );
        byteArrayFrameRenderer->setSubstituteChar( mByteArrayView->substituteChar() );
        byteArrayFrameRenderer->setUndefinedChar( mByteArrayView->undefinedChar() );

//     if( !confirmPrintPageNumber( byteArrayFrameRenderer->framesCount()) )
//         return;

        framesPrinter.addFrameRenderer( headerFrameRenderer );
        framesPrinter.addFrameRenderer( byteArrayFrameRenderer );
        framesPrinter.addFrameRenderer( footerFrameRenderer );

        info.setNoOfPages( byteArrayFrameRenderer->framesCount() );
        info.setUrl( mDocument->title() ); //TODO: get the url from synchronizer!!!

        QApplication::setOverrideCursor( Qt::WaitCursor );

        PrintJob *printJob = new PrintJob( &framesPrinter, 0, byteArrayFrameRenderer->framesCount()-1, &printer );
        const bool success = printJob->exec();

        QApplication::restoreOverrideCursor();

        if( !success )
        {
            const QString message = i18nc( "@info","Could not print." );

            KMessageBox::sorry( 0, message, processTitle );
        }
    }
    delete printDialog;
}

PrintTool::~PrintTool() {}

#include "printtool.moc"
