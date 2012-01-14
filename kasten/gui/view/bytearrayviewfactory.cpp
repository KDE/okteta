/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    Copyright 2006,2009 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "bytearrayviewfactory.h"

// lib
#include "bytearrayview.h"
#include <bytearrayviewprofilemanager.h>
#include <bytearrayviewprofilesynchronizer.h>
#include <bytearraydocument.h>


namespace Kasten2
{

ByteArrayViewFactory::ByteArrayViewFactory( ByteArrayViewProfileManager* byteArrayViewProfileManager )
  : AbstractViewFactory()
  , mByteArrayViewProfileManager( byteArrayViewProfileManager )
{
}

AbstractView* ByteArrayViewFactory::createViewFor( AbstractDocument* _document )
{
    ByteArrayView* result = 0;

    ByteArrayDocument* document = static_cast<ByteArrayDocument*>( _document );
    if( document )
    {
        result = new ByteArrayView( document );

        const ByteArrayViewProfile::Id defaultViewProfileId =
            mByteArrayViewProfileManager->defaultViewProfileId();
        if( ! defaultViewProfileId.isEmpty() )
        {
            ByteArrayViewProfileSynchronizer* synchronizer =
                new ByteArrayViewProfileSynchronizer( mByteArrayViewProfileManager );
            synchronizer->setViewProfileId( defaultViewProfileId );
            synchronizer->setView( result );

            result->setSynchronizer( synchronizer );
        }

    }

    return result;
}

AbstractView* ByteArrayViewFactory::createCopyOfView( AbstractView* _view, Qt::Alignment alignment )
{
    ByteArrayView* result = 0;

    ByteArrayView* view = qobject_cast<ByteArrayView*>( _view );
    if( view )
        result = new ByteArrayView( view, alignment );

    return result;
}

}
