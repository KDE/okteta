/***************************************************************************
                          versionview.h  -  description
                             -------------------
    begin                : Sat Feb 9 2008
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


#ifndef VERSIONVIEW_H
#define VERSIONVIEW_H


// Qt
#include <QtGui/QWidget>

class VersionTableModel;
class KAbstractDocument;
class QTreeView;


class VersionView : public QWidget
{
  Q_OBJECT

  public:
    explicit VersionView( QWidget *parent = 0 );
    virtual ~VersionView();

  public:
    void setDocument( KAbstractDocument *document );

  private:
    VersionTableModel *mVersionTableModel;

    QTreeView *mVersionTableView;
};

#endif
