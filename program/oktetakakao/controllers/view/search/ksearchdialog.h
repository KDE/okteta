/***************************************************************************
                          ksearchdialog.h  -  description
                             -------------------
    begin                : Tue Nov 14 2006
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


#ifndef KSEARCHDIALOG_H
#define KSEARCHDIALOG_H


// 
#include <kabstractfinddialog.h>


class KSearchDialog : public KAbstractFindDialog
{
  Q_OBJECT

  public:
    explicit KSearchDialog( QWidget *parent = 0 );
    virtual ~KSearchDialog();
};

#endif
