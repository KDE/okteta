/***************************************************************************
                          kview.h  -  description
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


#ifndef KABSTRACTVIEW_H
#define KABSTRACTVIEW_H


// lib
#include <kabstractdocument.h>
// Qt
#include <QtCore/QObject>

class QString;
class QWidget;

// TODO: is there a common base for view and document?
class KAbstractView : public QObject
{
    Q_OBJECT
  public:
    virtual ~KAbstractView();

  public: // API to be implemented
    virtual QWidget *widget() const = 0;
    virtual KAbstractDocument *document() const = 0;

    virtual QString title() const = 0;

  public:
//     virtual bool setDocument( KAbstractDocument *document ) = 0;
    bool hasLocalChanges() const;

  Q_SIGNALS:
    // TODO: should be signal the diff? how to say then remote is in synch again?
    void titleChanged( const QString &newTitel );
    void modified( KAbstractDocument::SynchronizationStates newStates );
};

// TODO: hack!!!! remove me!
inline bool KAbstractView::hasLocalChanges() const { return document()->hasLocalChanges(); }


inline KAbstractView::~KAbstractView() {}

#endif
