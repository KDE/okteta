/***************************************************************************
                          kabstractdocumentfactory.h  -  description
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


#ifndef KABSTRACTDOCUMENTFACTORY_H
#define KABSTRACTDOCUMENTFACTORY_H

// Qt
#include <QtCore/QObject>

class KAbstractDocument;


class KAbstractDocumentFactory : public QObject
{
  Q_OBJECT

  public:
    virtual ~KAbstractDocumentFactory();

  public: // API to be implemented
    virtual KAbstractDocument *create() = 0;
};

#endif
