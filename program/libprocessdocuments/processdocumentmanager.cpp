/*
    This file is part of the Kakao Framework, part of the KDE project.

    Copyright 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "processdocumentmanager.h"

// KDE
#include <KGlobal>
// Qt
#include <QtCore/QMutableListIterator>
#include <QtCore/QStringList>


K_GLOBAL_STATIC(ProcessDocumentManager, managerInstance)

ProcessDocumentManager* ProcessDocumentManager::self()
{
    return managerInstance;
}

ProcessDocumentManager::ProcessDocumentManager()
{}


AbstractProcessDocument* ProcessDocumentManager::processDocumentById( const QString& id ) const
{
    AbstractProcessDocument* result = 0;

    foreach( AbstractProcessDocument* document, mList )
    {
        if( document->id() == id )
        {
            result = document;
            break;
        }
    }
    return result;
}

void ProcessDocumentManager::addProcessDocument( AbstractProcessDocument* processDocument )
{
    mList.append( processDocument );
    // TODO: only emit if processDocument was not included before
    emit added( processDocument );
}

void ProcessDocumentManager::closeProcessDocument( AbstractProcessDocument* processDocument )
{
    QMutableListIterator<AbstractProcessDocument*> iterator( mList );

    if( iterator.findNext(processDocument) )
    {
    // TODO: first check if unsaved and ask, only then close

        iterator.remove();
        emit closing( processDocument );
        delete processDocument;
    }
}


ProcessDocumentManager::~ProcessDocumentManager()
{
}
