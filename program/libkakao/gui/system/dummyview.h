/***************************************************************************
                          dummyview.h  -  description
                             -------------------
    begin                : Fri Aug 31 2007
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


#ifndef DUMMYVIEW_H
#define DUMMYVIEW_H


// lib
#include <kabstractview.h>

class QLabel;


class DummyView : public KAbstractView
{
    Q_OBJECT

  public:
    DummyView( KAbstractDocument *document );
    virtual ~DummyView();

  public:
//     KCursorObject *cursor() const;

  public: // KAbstractView API
    virtual QWidget *widget() const;
    virtual KAbstractDocument *document() const;
    virtual QString title() const;

  protected Q_SLOTS:
    void onTitleChange( const QString &newTitle );

  protected:
    QLabel *mLabel;
    KAbstractDocument *mDocument;
};

#endif
