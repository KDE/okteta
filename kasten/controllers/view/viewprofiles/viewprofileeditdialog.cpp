/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    Copyright 2010,2012 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "viewprofileeditdialog.h"

// lib
#include "viewprofileedit.h"
// Okteta Gui Kasten
#include <bytearrayviewprofile.h>


namespace Kasten2
{

ViewProfileEditDialog::ViewProfileEditDialog( QWidget* parent )
  : KDialog( parent )
{
    mViewProfileEdit = new ViewProfileEdit( this );
    setMainWidget( mViewProfileEdit );
}

ByteArrayViewProfile ViewProfileEditDialog::viewProfile() const
{
    ByteArrayViewProfile result = mViewProfileEdit->viewProfile();
    result.setId( mId );
    return result;
}

void ViewProfileEditDialog::setViewProfile( const ByteArrayViewProfile& viewProfile )
{
    mViewProfileEdit->setViewProfile( viewProfile );
    mId = viewProfile.id();
}

ViewProfileEditDialog::~ViewProfileEditDialog()
{
}

}
