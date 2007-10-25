/***************************************************************************
                          testdocument.h  -  description
                             -------------------
    begin                : Thu Oct 25 2007
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


#ifndef TESTDOCUMENT_H
#define TESTDOCUMENT_H

// lib
#include "kabstractdocument.h"
// Qt
#include <QtCore/QString>


class TestDocument : public KAbstractDocument
{
    Q_OBJECT

  public:
    TestDocument();
    virtual ~TestDocument();

  public: // API to be implemented
    virtual QString title() const;
    virtual SynchronizationStates synchronizationStates() const;

  public: // instruction functions
    void setTitle( const QString &title );
    void setSynchronizationStates( SynchronizationStates synchronizationStates );

  protected:
    QString mTitle;
    SynchronizationStates mSynchronizationStates;
};

#endif
