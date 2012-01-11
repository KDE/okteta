/*
    This file is part of the Okteta Kasten module, made within the KDE community.

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

#ifndef EXTRACTSTRINGSJOB_H
#define EXTRACTSTRINGSJOB_H

// tool
#include "containedstring.h"
// Okteta core
#include <addressrange.h>
// Qt
#include <QtCore/QObject>
#include <QtCore/QList>

namespace Okteta {
class AbstractByteArrayModel;
class CharCodec;
}


namespace Kasten2
{

class ExtractStringsJob : public QObject // not yet: KJob
{
  Q_OBJECT

  public:
    ExtractStringsJob( const Okteta::AbstractByteArrayModel* model,
                       const Okteta::AddressRange& selection,
                       const Okteta::CharCodec* charCodec,
                       int minLength,
                       QList<ContainedString>* containedStringList );

  public:
    void exec();

  protected:
    const Okteta::AbstractByteArrayModel *mByteArrayModel;
    const Okteta::AddressRange mSelection;
    const Okteta::CharCodec *mCharCodec;
    const int mMinLength;

    QList<ContainedString> *mContainedStringList;
};


inline ExtractStringsJob::ExtractStringsJob( const Okteta::AbstractByteArrayModel* model,
                                             const Okteta::AddressRange& selection,
                                             const Okteta::CharCodec* charCodec,
                                             int minLength,
                                             QList<ContainedString>* containedStringList )
 : mByteArrayModel( model ), mSelection( selection ), mCharCodec( charCodec ), mMinLength( minLength ),
   mContainedStringList( containedStringList )
{}

}

#endif
