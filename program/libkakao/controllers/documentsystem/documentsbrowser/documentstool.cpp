/*
    This file is part of the Kakao Framework, part of the KDE project.

    Copyright 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "documentstool.h"

// Kakao core
#include <kdocumentmanager.h>
#include <kabstractdocument.h>
// KDE
#include <KLocale>


DocumentsTool::DocumentsTool( KDocumentManager* documentManager )
 : mDocumentManager( documentManager ), mFocussedDocument( 0 )
{
    setObjectName( "Documents" );

    connect( mDocumentManager, SIGNAL(added( KAbstractDocument* )), SIGNAL(documentAdded( KAbstractDocument* )) );
    connect( mDocumentManager, SIGNAL(closing( KAbstractDocument* )), SIGNAL(documentClosing( KAbstractDocument* )) );
}

QList<KAbstractDocument*> DocumentsTool::documents() const { return mDocumentManager->documents(); }

QString DocumentsTool::title() const { return i18nc("@title:window", "Documents"); }

// TODO: this is an abuse of setTargetModel. Find other way to get focused model
void DocumentsTool::setTargetModel( AbstractModel* model )
{
    mFocussedDocument = model ? model->findBaseModel<KAbstractDocument*>() : 0;
    emit focussedDocumentChanged( mFocussedDocument );
}


void DocumentsTool::setFocussedDocument( KAbstractDocument* document )
{
    mDocumentManager->requestFocus( document );
}

DocumentsTool::~DocumentsTool() {}
