/***************************************************************************
                          kdocumentversiondata.cpp  -  description
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


#include "kdocumentversiondata.h"


KDocumentVersionData::KDocumentVersionData( KDocumentVersionId id, const QString &changeComment )
 : mId( id ), mChangeComment( changeComment )
{}

KDocumentVersionId KDocumentVersionData::id() const { return mId; }

QString KDocumentVersionData::changeComment() const { return mChangeComment; }
