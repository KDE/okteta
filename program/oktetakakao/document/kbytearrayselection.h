/***************************************************************************
                          kbytearrayselection.h  -  description
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


#ifndef KBYTEAARRAYSELECTION_H
#define KBYTEAARRAYSELECTION_H

// kakao
#include <kabstractdocumentselection.h>
// Okteta core
#include <ksection.h>

class KByteArrayDocument;

class KByteArraySelection : public KAbstractDocumentSelection
{
    Q_OBJECT

  public:
    explicit KByteArraySelection( KByteArrayDocument *document );
    virtual ~KByteArraySelection();

  public: // API to be implemented
    virtual KAbstractDocument *document() const;

  public:
    void setSection( const KHE::KSection &section );

  public:
    KHE::KSection section() const;
    bool isValid() const;

  protected:
    KByteArrayDocument *mDocument;
    KHE::KSection mSection;
};

#endif
