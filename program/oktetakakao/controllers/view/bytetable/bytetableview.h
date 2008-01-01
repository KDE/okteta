/***************************************************************************
                          bytetableview.h  -  description
                             -------------------
    begin                : Mon Dec 31 2007
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


#ifndef BYTETABLEVIEW_H
#define BYTETABLEVIEW_H


// Qt
#include <QtGui/QWidget>

class ByteTableTool;
class KPushButton;
class QSpinBox;
class QTreeView;
class QModelIndex;


class ByteTableView : public QWidget
{
  Q_OBJECT

  public:
    explicit ByteTableView( ByteTableTool *tool, QWidget *parent = 0 );
    virtual ~ByteTableView();

  public Q_SLOTS:
    void onInsertClicked();
    void onDoubleClicked( const QModelIndex &index );

  private:
    ByteTableTool *mTool;
    QSpinBox  *mInsertCountSpinBox;
    QTreeView *mByteTableView;
    KPushButton *mInsertButton;
};

#endif
