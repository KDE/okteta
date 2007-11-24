/***************************************************************************
                          kabstractdocumentselection.h  -  description
                             -------------------
    begin                : Thu Nov 22 2007
    copyright            : 2007 by Friedrich W. H. Kossebau
    email                : kossebau@kde.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU General Public License           *
 *   version 2 as published by the Free Software Foundation.               *
 *                                                                         *
 ***************************************************************************/


#ifndef KABSTRACTDOCUMENTSELECTION_H
#define KABSTRACTDOCUMENTSELECTION_H

// Qt
#include <QtCore/QObject>

class KAbstractDocument;

class KAbstractDocumentSelection : public QObject
{
    Q_OBJECT

  protected:
    KAbstractDocumentSelection();
  public:
    virtual ~KAbstractDocumentSelection();

  public: // API to be implemented
//     virtual bool isValid() const = 0;
    virtual KAbstractDocument *document() const = 0;

  protected:
    class Private;
    Private * const d;
};

#endif
