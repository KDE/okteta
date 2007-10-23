/***************************************************************************
                          kbytearraydocumentfactory.h  -  description
                             -------------------
    begin                : Thu Nov 2 2006
    copyright            : 2006 by Friedrich W. H. Kossebau
    email                : kossebau@kde.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU General Public License           *
 *   version 2 as published by the Free Software Foundation.               *
 *                                                                         *
 ***************************************************************************/


#ifndef KBYTEARRAYDOCUMENTFACTORY_H
#define KBYTEARRAYDOCUMENTFACTORY_H


//
#include <kifileloader.h>
// kakao
#include <kdocumentabstractfactory.h>


class KByteArrayDocumentFactory : public KDocumentAbstractFactory, public KDE::If::FileLoader
{
    Q_OBJECT
    Q_INTERFACES(KDE::If::FileLoader)

  public: // KDocumentAbstractFactory API
    virtual KAbstractDocument *create();

  public: // KDE::If::FileLoader API
    virtual KAbstractDocument *load( const QString &tmpFileName, const QString &originUrl );
};

#endif
