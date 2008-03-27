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

#include "printtool.h"

// controller
#include "framestopaperprinter.h"
#include "headerfooterframerenderer.h"
#include "bytearrayframerenderer.h"
// lib
#include <kbytearraydocument.h>
// Okteta gui
#include <kbytearrayview.h>
// Okteta core
#include <kcharcodec.h>
#include <kbytearraymodel.h>
// KDE
#include <KLocale>
#include <KXmlGuiWindow>
#include <KMessageBox>
#include <kdeprintdialog.h>
#include <kdeversion.h>
// Qt
#include <QtGui/QPrintDialog>
#include <QtGui/QPrinter>
#include <QtGui/QFont>
#include <QtGui/QFontMetrics>


PrintTool::PrintTool( KXmlGuiWindow *window )
 : mWindow( window )
{
}

void PrintTool::print( KByteArrayDocument *document )
{
    const QString processTitle = i18nc( "@title:window", "Print Byte Array %1", document->title() );

    QPrinter printer;

    // Not supported by Qt?
    //printer.setPageSelection( QPrinter::SystemSide );

//     LayoutDialogPage* layoutPage = new LayoutDialogPage();
    QList<QWidget*> customDialogPages;
//     customDialogPages << layoutPage;
    QPrintDialog *printDialog = KdePrint::createPrintDialog( &printer, customDialogPages, mWindow );

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
        headerFrameRenderer->setTexts( QString("%y"),
                                       QString("%f"),
                                       QString("Page %p of %P") );
        headerFrameRenderer->setWidth( width );
        headerFrameRenderer->setPos( pageRect.topLeft() );

        HeaderFooterFrameRenderer *footerFrameRenderer = new HeaderFooterFrameRenderer( &info );
        footerFrameRenderer->setTexts( QString("Printed by: %U"),
                                       QString("Okteta, built on KDE4"),
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
        byteArrayFrameRenderer->setByteArrayModel( document->content() );

//     if( !confirmPrintPageNumber( byteArrayFrameRenderer->framesCount()) )
//         return;

        framesPrinter.addFrameRenderer( headerFrameRenderer );
        framesPrinter.addFrameRenderer( byteArrayFrameRenderer );
        framesPrinter.addFrameRenderer( footerFrameRenderer );

        info.setNoOfPages( byteArrayFrameRenderer->framesCount() );
        info.setUrl( document->title() ); //TODO: get the url from synchronizer!!!

        const bool success = framesPrinter.print( &printer, 0, byteArrayFrameRenderer->framesCount()-1 );

        if( !success )
        {
            const QString message = i18nc( "@info","Could not print." );

            KMessageBox::sorry( mWindow, message, processTitle );
        }
    }
}

PrintTool::~PrintTool() {}
