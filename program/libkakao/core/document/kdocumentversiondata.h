/***************************************************************************
                          kdocumentversiondata.h  -  description
                             -------------------
    begin                : Fri Feb 8 2008
    copyright            : 2008 by Friedrich W. H. Kossebau
    email                : kossebau@kde.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU General Public License           *
 *   version 2 as published by the Free Software Foundation.               *
 *                                                                         *
 ***************************************************************************/


#ifndef KDOCUMENTVERSIONDATA_H
#define KDOCUMENTVERSIONDATA_H

// lib
#include "kdocumentversionid.h"
// Qt
#include <QtCore/QString>

class KDocumentVersionData
{
  public:
    KDocumentVersionData( KDocumentVersionId id, const QString &changeComment );

  public:
    KDocumentVersionId id() const;
    QString changeComment() const;

  protected:
    KDocumentVersionId mId;
    QString mChangeComment;
};

#endif
