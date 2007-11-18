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
#include <QtCore/QByteArray>
#include <QtCore/QString>


class TestDocument : public KAbstractDocument
{
    Q_OBJECT

  public:
    TestDocument();
    explicit TestDocument( const QByteArray &data );
    virtual ~TestDocument();

  public: // API to be implemented
    virtual QString title() const;
    virtual QString typeName() const;
    virtual QString mimeType() const;
    virtual SynchronizationStates synchronizationStates() const;

  public:
    const QByteArray *data() const;
    void setData( const QByteArray &data );

  public: // instruction functions
    void setTitle( const QString &title );
    void setSynchronizationStates( SynchronizationStates synchronizationStates );

  protected:
    QString mTitle;
    QByteArray mData;
    SynchronizationStates mSynchronizationStates;
};

#endif
