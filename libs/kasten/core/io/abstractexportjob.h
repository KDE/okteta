/*
    This file is part of the Kasten Framework, part of the KDE project.

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

#ifndef ABSTRACTEXPORTJOB_H
#define ABSTRACTEXPORTJOB_H

// lib
#include "kastencore_export.h"
// KDE
#include <KJob>


namespace Kasten
{

class KAbstractDocument;


class KASTENCORE_EXPORT AbstractExportJob : public KJob
{
  Q_OBJECT

  public:
    AbstractExportJob();
    virtual ~AbstractExportJob();

  public:
    KAbstractDocument *document() const;

  Q_SIGNALS:
    void documentLoaded( Kasten::KAbstractDocument* document );

  protected:
    // emits documentLoaded()
    // TODO: or better name property LoadedDocument?
    virtual void setDocument( KAbstractDocument *document );

  protected:
    class Private;
    Private * const d;
};

}

#endif
