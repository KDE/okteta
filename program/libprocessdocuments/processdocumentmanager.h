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

#ifndef PROCESSDOCUMENTMANAGER_H
#define PROCESSDOCUMENTMANAGER_H

// lib
#include "abstractprocessdocument.h"
// Qt
#include <QtCore/QList>
#include <QtCore/QObject>

class ProcessDocumentManager : public QObject
{
  Q_OBJECT

  public:
    static ProcessDocumentManager* self();

//   protected:
  public:
    ProcessDocumentManager();
  public:
    virtual ~ProcessDocumentManager();

  public:
    void addProcessDocument( AbstractProcessDocument *processDocument );
    void closeProcessDocument( AbstractProcessDocument* processDocument );

  public:
    QList<AbstractProcessDocument*> processDocumentList() const;
    AbstractProcessDocument* processDocumentById( const QString& id ) const;

  Q_SIGNALS:
    // document got added
    void added( AbstractProcessDocument* processDocument );
    /// document is about to be closed, cannot be stopped
    void closing( AbstractProcessDocument* processDocument );

  protected:
    QList<AbstractProcessDocument*> mList;
};

inline QList<AbstractProcessDocument*> ProcessDocumentManager::processDocumentList() const { return mList; }

#endif
