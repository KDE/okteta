/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    Copyright 2008-2009 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#ifndef SEARCHJOB_H
#define SEARCHJOB_H

// Okteta core
#include <address.h>
// Qt
#include <QtCore/QObject>
#include <QtCore/QByteArray>

namespace Okteta {
class AbstractByteArrayModel;
class CharCodec;
}


namespace Kasten2
{

class SearchJob : public QObject // not yet: KJob
{
  Q_OBJECT

  public:
    SearchJob( const Okteta::AbstractByteArrayModel* model,
               const QByteArray& searchData,
               Okteta::Address startIndex, Okteta::Address endIndex,
               Qt::CaseSensitivity caseSensitivity, const QString& charCodecName );

    virtual ~SearchJob();

  public:
    Okteta::Address exec();

  protected:
    int indexOfIgnoreCase();
    int lastIndexOfIgnoreCase();

  protected Q_SLOTS:
    void onBytesSearched();

  protected:
    const Okteta::AbstractByteArrayModel* mByteArrayModel;

    QByteArray mSearchData;
    Okteta::Address mStartIndex;
    Okteta::Address mEndIndex;
    Qt::CaseSensitivity mCaseSensitivity;
    const Okteta::CharCodec* mCharCodec;
};

}

#endif
